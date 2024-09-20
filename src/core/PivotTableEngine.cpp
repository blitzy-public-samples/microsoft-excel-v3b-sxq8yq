#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include "PivotTableEngine.h"
#include "WorksheetEngine.h"
#include "CellManager.h"
#include "DataConnectivity.h"

// Constructor implementation
PivotTableEngine::PivotTableEngine(std::shared_ptr<WorksheetEngine> worksheetEngine,
                                   std::shared_ptr<CellManager> cellManager,
                                   std::shared_ptr<DataConnectivity> dataConnectivity)
    : m_worksheetEngine(std::move(worksheetEngine)),
      m_cellManager(std::move(cellManager)),
      m_dataConnectivity(std::move(dataConnectivity)) {
    // Set up any necessary internal data structures
}

PivotTable PivotTableEngine::CreatePivotTable(const std::string& sourceRange, const std::string& destinationCell) {
    // Validate source range and destination cell
    if (!m_worksheetEngine->IsValidRange(sourceRange) || !m_worksheetEngine->IsValidCell(destinationCell)) {
        throw std::invalid_argument("Invalid source range or destination cell");
    }

    // Extract data from source range using WorksheetEngine
    auto sourceData = m_worksheetEngine->GetCellRange(sourceRange);

    // Create a new PivotTable object
    PivotTable newPivotTable(sourceRange, destinationCell);

    // Initialize pivot table structure in the destination area
    auto destinationRange = m_worksheetEngine->GetRangeFromTopLeftCell(destinationCell, sourceData.size(), sourceData[0].size());
    for (size_t i = 0; i < sourceData.size(); ++i) {
        for (size_t j = 0; j < sourceData[i].size(); ++j) {
            m_cellManager->SetCellValue(destinationRange[i][j], sourceData[i][j]);
        }
    }

    // Add the new pivot table to m_pivotTables
    m_pivotTables.push_back(newPivotTable);

    // Return the created PivotTable object
    return newPivotTable;
}

bool PivotTableEngine::UpdatePivotTable(const PivotTable& pivotTable) {
    // Find the existing pivot table in m_pivotTables
    auto it = std::find(m_pivotTables.begin(), m_pivotTables.end(), pivotTable);
    if (it == m_pivotTables.end()) {
        return false;
    }

    // Refresh data from the source range
    auto sourceData = m_worksheetEngine->GetCellRange(pivotTable.GetSourceRange());

    // Apply new pivot table settings (if any)
    it->ApplySettings(pivotTable.GetSettings());

    // Recalculate pivot table results
    CalculatePivotTableResults(*it);

    // Update cells in the destination area with new results
    auto destinationRange = m_worksheetEngine->GetRangeFromTopLeftCell(pivotTable.GetDestinationCell(), 
                                                                       it->GetResultData().size(), 
                                                                       it->GetResultData()[0].size());
    for (size_t i = 0; i < it->GetResultData().size(); ++i) {
        for (size_t j = 0; j < it->GetResultData()[i].size(); ++j) {
            m_cellManager->SetCellValue(destinationRange[i][j], it->GetResultData()[i][j]);
        }
    }

    // Apply formatting
    ApplyPivotTableFormatting(*it);

    return true;
}

bool PivotTableEngine::DeletePivotTable(const PivotTable& pivotTable) {
    // Find the pivot table in m_pivotTables
    auto it = std::find(m_pivotTables.begin(), m_pivotTables.end(), pivotTable);
    if (it == m_pivotTables.end()) {
        return false;
    }

    // Clear the cells in the pivot table's destination area
    m_worksheetEngine->ClearRange(m_worksheetEngine->GetRangeFromTopLeftCell(pivotTable.GetDestinationCell(), 
                                                                             it->GetResultData().size(), 
                                                                             it->GetResultData()[0].size()));

    // Remove the pivot table from m_pivotTables
    m_pivotTables.erase(it);

    return true;
}

void PivotTableEngine::CalculatePivotTableResults(PivotTable& pivotTable) {
    // Group data based on row and column fields
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<double>>> groupedData;
    auto sourceData = m_worksheetEngine->GetCellRange(pivotTable.GetSourceRange());
    
    for (const auto& row : sourceData) {
        std::string rowKey = row[pivotTable.GetRowField()];
        std::string colKey = row[pivotTable.GetColumnField()];
        double value = std::stod(row[pivotTable.GetValueField()]);
        
        groupedData[rowKey][colKey].push_back(value);
    }

    // Apply aggregation functions (sum, count, average, etc.) to value fields
    std::vector<std::vector<std::string>> resultData;
    for (const auto& rowGroup : groupedData) {
        std::vector<std::string> resultRow;
        resultRow.push_back(rowGroup.first);  // Row header
        
        for (const auto& colGroup : rowGroup.second) {
            double aggregatedValue = 0;
            switch (pivotTable.GetAggregationFunction()) {
                case AggregationFunction::Sum:
                    aggregatedValue = std::accumulate(colGroup.second.begin(), colGroup.second.end(), 0.0);
                    break;
                case AggregationFunction::Average:
                    aggregatedValue = std::accumulate(colGroup.second.begin(), colGroup.second.end(), 0.0) / colGroup.second.size();
                    break;
                case AggregationFunction::Count:
                    aggregatedValue = colGroup.second.size();
                    break;
                // Add other aggregation functions as needed
            }
            resultRow.push_back(std::to_string(aggregatedValue));
        }
        
        resultData.push_back(resultRow);
    }

    // Update pivot table object with calculated results
    pivotTable.SetResultData(resultData);
}

void ApplyPivotTableFormatting(const PivotTable& pivotTable) {
    // Apply header formatting to row and column headers
    auto headerStyle = CellStyle().SetBold(true).SetBackgroundColor(Color::LightGray);
    m_cellManager->SetCellStyle(pivotTable.GetDestinationCell(), headerStyle);

    // Apply data cell formatting to value cells
    auto dataCellStyle = CellStyle().SetNumberFormat("#,##0.00");
    auto dataRange = m_worksheetEngine->GetRangeFromTopLeftCell(
        m_worksheetEngine->GetCellBelow(m_worksheetEngine->GetCellRight(pivotTable.GetDestinationCell())),
        pivotTable.GetResultData().size() - 1,
        pivotTable.GetResultData()[0].size() - 1
    );
    m_cellManager->SetRangeStyle(dataRange, dataCellStyle);

    // Apply total row and column formatting if enabled
    if (pivotTable.HasTotals()) {
        auto totalStyle = CellStyle().SetBold(true).SetBackgroundColor(Color::LightBlue);
        // Apply to total row
        m_cellManager->SetRangeStyle(
            m_worksheetEngine->GetRangeFromTopLeftCell(
                m_worksheetEngine->GetCellBelow(pivotTable.GetDestinationCell(), pivotTable.GetResultData().size() - 1),
                1,
                pivotTable.GetResultData()[0].size()
            ),
            totalStyle
        );
        // Apply to total column
        m_cellManager->SetRangeStyle(
            m_worksheetEngine->GetRangeFromTopLeftCell(
                m_worksheetEngine->GetCellRight(pivotTable.GetDestinationCell(), pivotTable.GetResultData()[0].size() - 1),
                pivotTable.GetResultData().size(),
                1
            ),
            totalStyle
        );
    }
}

// Human tasks:
// TODO: Implement advanced pivot table features such as calculated fields and items
// TODO: Optimize performance for large datasets
// TODO: Add support for external data sources through DataConnectivity
// TODO: Implement caching mechanism for faster pivot table updates
// TODO: Add error handling and validation for edge cases
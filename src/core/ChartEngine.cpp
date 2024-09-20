#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "ChartEngine.h"
#include "WorksheetEngine.h"
#include "CellManager.h"
#include "DataSeries.h"
#include "ChartTypes.h"
#include "RenderingEngine.h"

// Global constants
const int MAX_CHART_SERIES = 10;
const int DEFAULT_CHART_WIDTH = 500;
const int DEFAULT_CHART_HEIGHT = 300;

// Helper function to parse data range
std::pair<CellCoordinate, CellCoordinate> ParseDataRange(const std::string& dataRange) {
    // Split the data range string into start and end parts
    size_t colonPos = dataRange.find(':');
    std::string startRange = dataRange.substr(0, colonPos);
    std::string endRange = dataRange.substr(colonPos + 1);

    // Convert each part into a CellCoordinate
    CellCoordinate start = CellCoordinate::FromString(startRange);
    CellCoordinate end = CellCoordinate::FromString(endRange);

    // Validate the resulting coordinates
    if (!start.IsValid() || !end.IsValid() || start > end) {
        throw std::invalid_argument("Invalid data range");
    }

    // Return the pair of coordinates
    return std::make_pair(start, end);
}

// ChartEngine implementation
ChartEngine::ChartEngine(WorksheetEngine* worksheetEngine, CellManager* cellManager, RenderingEngine* renderingEngine)
    : m_worksheetEngine(worksheetEngine), m_cellManager(cellManager), m_renderingEngine(renderingEngine) {
    // Initialize member variables with provided dependencies
    if (!m_worksheetEngine || !m_cellManager || !m_renderingEngine) {
        throw std::invalid_argument("Invalid dependencies provided to ChartEngine");
    }

    // Set up any necessary internal data structures
    m_charts.clear();
}

int ChartEngine::CreateChart(ChartType type, const std::string& dataRange, int width, int height) {
    // Validate input parameters
    if (width <= 0) width = DEFAULT_CHART_WIDTH;
    if (height <= 0) height = DEFAULT_CHART_HEIGHT;

    // Parse the data range and extract data using CellManager
    auto [startCell, endCell] = ParseDataRange(dataRange);
    std::vector<DataSeries> dataSeries = m_cellManager->GetDataSeries(startCell, endCell);

    // Create a new Chart object with the extracted data
    auto chart = std::make_unique<Chart>(type, dataSeries, width, height);

    // Assign a unique identifier to the chart
    int chartId = GenerateUniqueChartId();

    // Store the chart in m_charts
    m_charts[chartId] = std::move(chart);

    // Return the chart identifier
    return chartId;
}

bool ChartEngine::UpdateChart(int chartId, const std::string& dataRange, int width, int height) {
    // Check if the chart with the given ID exists
    auto it = m_charts.find(chartId);
    if (it == m_charts.end()) {
        return false;
    }

    // Parse the new data range and extract data
    auto [startCell, endCell] = ParseDataRange(dataRange);
    std::vector<DataSeries> dataSeries = m_cellManager->GetDataSeries(startCell, endCell);

    // Update the chart's data series
    it->second->UpdateDataSeries(dataSeries);

    // Resize the chart if new dimensions are provided
    if (width > 0 && height > 0) {
        it->second->Resize(width, height);
    }

    // Trigger a re-render of the chart
    return RenderChart(chartId);
}

bool ChartEngine::RenderChart(int chartId) {
    // Retrieve the chart object from m_charts
    auto it = m_charts.find(chartId);
    if (it == m_charts.end()) {
        return false;
    }

    try {
        // Prepare the chart data for rendering
        ChartRenderData renderData = it->second->PrepareRenderData();

        // Call the appropriate rendering function in RenderingEngine
        m_renderingEngine->RenderChart(renderData);

        return true;
    }
    catch (const std::exception& e) {
        // Handle any rendering errors or exceptions
        // Log the error and return false
        return false;
    }
}

bool ChartEngine::DeleteChart(int chartId) {
    // Check if the chart with the given ID exists
    auto it = m_charts.find(chartId);
    if (it == m_charts.end()) {
        return false;
    }

    // Remove the chart from m_charts
    m_charts.erase(it);

    // Clean up any associated resources
    // (In this case, the unique_ptr will automatically clean up the Chart object)

    return true;
}

// Human tasks (commented out):
/*
TODO: Implement data validation for different chart types
TODO: Add support for custom color schemes
TODO: Implement undo/redo functionality for chart updates
TODO: Optimize rendering performance for large datasets
TODO: Implement caching mechanism for frequently rendered charts
TODO: Implement a confirmation mechanism for chart deletion
TODO: Add support for named ranges in data range parsing
*/
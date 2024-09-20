#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include "CellManager.h"
#include "FormulaEngine.h"
#include "Cell.h"
#include "CellRange.h"
#include "Style.h"

CellManager::CellManager(std::shared_ptr<FormulaEngine> formulaEngine)
    : formulaEngine(formulaEngine) {
    // Initialize the cells unordered_map
    cells = std::unordered_map<std::string, std::shared_ptr<Cell>>();
    // Store the reference to the FormulaEngine
    this->formulaEngine = formulaEngine;
}

void CellManager::setCellValue(const std::string& cellAddress, const std::string& value) {
    // Get or create the cell at the given address
    auto& cell = cells[cellAddress];
    if (!cell) {
        cell = std::make_shared<Cell>(cellAddress);
    }

    // Set the cell's value
    cell->setValue(value);

    // If the value starts with '=', mark it as a formula
    if (!value.empty() && value[0] == '=') {
        cell->setIsFormula(true);
    } else {
        cell->setIsFormula(false);
    }

    // Trigger recalculation of dependent cells
    recalculateDependents(cellAddress);
}

std::string CellManager::getCellValue(const std::string& cellAddress) {
    // Check if the cell exists
    auto it = cells.find(cellAddress);
    
    // If it exists, return its value
    if (it != cells.end()) {
        return it->second->getValue();
    }
    
    // If it doesn't exist, return an empty string
    return "";
}

void CellManager::setCellStyle(const std::string& cellAddress, const Style& style) {
    // Get or create the cell at the given address
    auto& cell = cells[cellAddress];
    if (!cell) {
        cell = std::make_shared<Cell>(cellAddress);
    }

    // Set the cell's style
    cell->setStyle(style);
}

CellRange CellManager::getCellRange(const std::string& startAddress, const std::string& endAddress) {
    // Validate the start and end addresses
    // TODO: Implement address validation

    // Create a CellRange object
    CellRange range(startAddress, endAddress);

    // Populate the CellRange with cells from the given range
    // TODO: Implement logic to determine all cells within the range
    for (const auto& [address, cell] : cells) {
        if (/* cell is within range */) {
            range.addCell(cell);
        }
    }

    // Return the CellRange
    return range;
}

void CellManager::recalculateDependents(const std::string& cellAddress) {
    // Get the cell at the given address
    auto it = cells.find(cellAddress);
    if (it == cells.end()) {
        return;
    }

    // Get the list of dependent cells from the FormulaEngine
    auto dependents = formulaEngine->getDependentCells(cellAddress);

    // For each dependent cell, recalculate its value
    for (const auto& dependentAddress : dependents) {
        auto dependentCell = cells[dependentAddress];
        if (dependentCell && dependentCell->isFormula()) {
            std::string formula = dependentCell->getValue();
            std::string newValue = formulaEngine->evaluateFormula(formula);
            dependentCell->setValue(newValue);

            // Recursively recalculate dependents of updated cells
            recalculateDependents(dependentAddress);
        }
    }
}

// Human tasks:
// TODO: Implement error handling for invalid cell addresses
// TODO: Add support for different data types (numbers, dates, etc.)
// TODO: Implement caching mechanism for frequently accessed cells
// TODO: Implement style inheritance for efficiency
// TODO: Optimize for large ranges by implementing lazy loading
// TODO: Implement circular reference detection and handling
// TODO: Optimize recalculation order for efficiency
// TODO: Implement unit tests for all CellManager methods
// TODO: Add support for merging cells
// TODO: Implement undo/redo functionality for cell operations
// TODO: Add support for cell comments and annotations
// TODO: Implement cell validation rules
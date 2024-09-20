#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "WorkbookManager.h"
#include "WorksheetEngine.h"
#include "CellManager.h"
#include "FormulaEngine.h"
#include "DataConnectivity.h"
#include "FileIO.h"

class SpreadsheetEngine {
private:
    std::shared_ptr<WorkbookManager> workbookManager;
    std::shared_ptr<FormulaEngine> formulaEngine;
    std::shared_ptr<DataConnectivity> dataConnectivity;
    std::shared_ptr<FileIO> fileIO;

public:
    // Constructor: Initializes the SpreadsheetEngine with necessary components
    SpreadsheetEngine() {
        // Initialize workbookManager
        workbookManager = std::make_shared<WorkbookManager>();

        // Initialize formulaEngine
        formulaEngine = std::make_shared<FormulaEngine>();

        // Initialize dataConnectivity
        dataConnectivity = std::make_shared<DataConnectivity>();

        // Initialize fileIO
        fileIO = std::make_shared<FileIO>();
    }

    // Creates a new workbook
    std::shared_ptr<Workbook> createWorkbook(const std::string& name) {
        // Call workbookManager->createWorkbook(name)
        auto workbook = workbookManager->createWorkbook(name);

        // Return the created workbook
        return workbook;
    }

    // Opens an existing workbook
    std::shared_ptr<Workbook> openWorkbook(const std::string& filePath) {
        // Call fileIO->readWorkbook(filePath) to get workbook data
        auto workbookData = fileIO->readWorkbook(filePath);

        // Call workbookManager->loadWorkbook(workbookData)
        auto workbook = workbookManager->loadWorkbook(workbookData);

        // Return the loaded workbook
        return workbook;
    }

    // Saves a workbook to a file
    bool saveWorkbook(std::shared_ptr<Workbook> workbook, const std::string& filePath) {
        // Call workbookManager->serializeWorkbook(workbook) to get serialized data
        auto serializedData = workbookManager->serializeWorkbook(workbook);

        // Call fileIO->writeWorkbook(serializedData, filePath)
        bool success = fileIO->writeWorkbook(serializedData, filePath);

        // Return the success status
        return success;
    }

    // Calculates all formulas in a workbook
    void calculateFormulas(std::shared_ptr<Workbook> workbook) {
        // Call formulaEngine->calculateAll(workbook)
        formulaEngine->calculateAll(workbook);
    }

    // Imports data from an external source into a worksheet
    bool importData(std::shared_ptr<Worksheet> worksheet, const std::string& dataSource, const std::string& range) {
        // Call dataConnectivity->importData(dataSource) to get imported data
        auto importedData = dataConnectivity->importData(dataSource);

        // Call worksheet->updateRange(range, importedData)
        bool success = worksheet->updateRange(range, importedData);

        // Return the success status
        return success;
    }
};
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "WorkbookManager.h"
#include "Workbook.h"
#include "Worksheet.h"
#include "CellManager.h"
#include "FormulaEngine.h"
#include "FileIO.h"
#include "CollaborationServices.h"
#include "AddInManager.h"

// Constructor implementation
WorkbookManager::WorkbookManager(std::shared_ptr<FileIO> fileIO,
                                 std::shared_ptr<FormulaEngine> formulaEngine,
                                 std::shared_ptr<CollaborationServices> collaborationServices,
                                 std::shared_ptr<AddInManager> addInManager)
    : m_fileIO(fileIO),
      m_formulaEngine(formulaEngine),
      m_collaborationServices(collaborationServices),
      m_addInManager(addInManager) {
    // Set up any necessary event listeners or callbacks
    // TODO: Implement event listeners and callbacks
}

std::shared_ptr<Workbook> WorkbookManager::createWorkbook(const std::string& name) {
    // Create a new Workbook object
    auto workbook = std::make_shared<Workbook>(name);

    // Add the workbook to m_workbooks map
    m_workbooks[name] = workbook;

    // Initialize the workbook with a default worksheet
    workbook->addWorksheet("Sheet1");

    // Notify CollaborationServices about the new workbook
    m_collaborationServices->notifyWorkbookCreated(workbook);

    // Return the pointer to the new workbook
    return workbook;
}

std::shared_ptr<Workbook> WorkbookManager::loadWorkbook(const std::string& filePath) {
    // Use FileIO to read the workbook file
    auto fileContents = m_fileIO->readFile(filePath);

    // Parse the file contents and create a Workbook object
    auto workbook = m_fileIO->parseWorkbook(fileContents);

    if (workbook) {
        // Add the workbook to m_workbooks map
        m_workbooks[workbook->getName()] = workbook;

        // Notify CollaborationServices about the loaded workbook
        m_collaborationServices->notifyWorkbookLoaded(workbook);
    }

    // Return the pointer to the loaded workbook
    return workbook;
}

bool WorkbookManager::saveWorkbook(std::shared_ptr<Workbook> workbook, const std::string& filePath) {
    // Serialize the Workbook object to the appropriate file format
    auto serializedData = workbook->serialize();

    // Use FileIO to write the serialized data to the specified file path
    bool saveResult = m_fileIO->writeFile(filePath, serializedData);

    if (saveResult) {
        // Update the workbook's saved state
        workbook->markAsSaved();

        // Notify CollaborationServices about the saved workbook
        m_collaborationServices->notifyWorkbookSaved(workbook);
    }

    // Return the result of the save operation
    return saveResult;
}

bool WorkbookManager::closeWorkbook(std::shared_ptr<Workbook> workbook) {
    // Check if the workbook has unsaved changes
    if (workbook->hasUnsavedChanges()) {
        // TODO: Prompt user to save changes if necessary
        // For now, we'll just save the changes automatically
        saveWorkbook(workbook, workbook->getFilePath());
    }

    // Remove the workbook from m_workbooks map
    m_workbooks.erase(workbook->getName());

    // Notify CollaborationServices about the closed workbook
    m_collaborationServices->notifyWorkbookClosed(workbook);

    // Clean up any resources associated with the workbook
    workbook->cleanup();

    // Return the result of the close operation
    return true;
}

std::shared_ptr<Workbook> WorkbookManager::getWorkbook(const std::string& name) {
    // Search for the workbook in m_workbooks map
    auto it = m_workbooks.find(name);

    // Return the pointer to the workbook if found, or nullptr otherwise
    return (it != m_workbooks.end()) ? it->second : nullptr;
}

// Human tasks (commented out):
/*
TODO: Implement workbook version control and history tracking
TODO: Add support for concurrent editing of shared workbooks
TODO: Implement workbook encryption for sensitive data protection
TODO: Develop a plugin system for extending workbook functionality
*/
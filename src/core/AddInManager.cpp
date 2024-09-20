#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "AddInManager.h"
#include "AddIn.h"
#include "WorkbookManager.h"
#include "ExcelException.h"

// Constructor for AddInManager
AddInManager::AddInManager(std::shared_ptr<WorkbookManager> workbookManager)
    : m_workbookManager(workbookManager) {
    // Initialize an empty m_loadedAddIns map
    m_loadedAddIns = std::unordered_map<std::string, std::shared_ptr<AddIn>>();
}

bool AddInManager::loadAddIn(const std::string& path) {
    // Check if the add-in is already loaded
    if (m_loadedAddIns.find(path) != m_loadedAddIns.end()) {
        return false;  // Add-in already loaded
    }

    // Attempt to create an AddIn object from the specified path
    try {
        auto addIn = std::make_shared<AddIn>(path);
        
        // If successful, add the AddIn to m_loadedAddIns
        m_loadedAddIns[addIn->getName()] = addIn;
        return true;
    } catch (const std::exception& e) {
        // Add-in loading failed
        return false;
    }
}

bool AddInManager::unloadAddIn(const std::string& addInName) {
    // Check if the add-in is loaded
    auto it = m_loadedAddIns.find(addInName);
    if (it == m_loadedAddIns.end()) {
        return false;  // Add-in not found
    }

    // If loaded, remove the add-in from m_loadedAddIns
    m_loadedAddIns.erase(it);
    return true;
}

std::string AddInManager::executeAddInFunction(const std::string& addInName, const std::string& functionName, const std::vector<std::string>& arguments) {
    // Check if the specified add-in is loaded
    auto it = m_loadedAddIns.find(addInName);
    if (it == m_loadedAddIns.end()) {
        throw ExcelException("Add-in not found: " + addInName);
    }

    // If loaded, call the specified function on the add-in object
    auto addIn = it->second;
    try {
        // Pass the provided arguments to the function
        return addIn->executeFunction(functionName, arguments);
    } catch (const std::exception& e) {
        throw ExcelException("Error executing add-in function: " + std::string(e.what()));
    }
}

std::vector<std::string> AddInManager::getLoadedAddIns() {
    // Create an empty vector to store add-in names
    std::vector<std::string> addInNames;

    // Iterate through m_loadedAddIns
    for (const auto& pair : m_loadedAddIns) {
        // Add each add-in name to the vector
        addInNames.push_back(pair.first);
    }

    // Return the vector of add-in names
    return addInNames;
}

// Human tasks:
// TODO: Implement error handling for invalid add-in files
// TODO: Add logging for add-in loading events
// TODO: Implement cleanup of add-in resources
// TODO: Add notification to workbooks using the unloaded add-in
// TODO: Implement sandboxing for add-in function execution
// TODO: Add performance monitoring for add-in functions
// TODO: Implement version checking for add-ins to ensure compatibility
// TODO: Add support for add-in dependencies and conflict resolution
// TODO: Implement a mechanism for add-ins to register custom ribbon UI elements
// TODO: Create a comprehensive error handling and reporting system for add-in related issues
// TODO: Develop a testing framework for add-ins to ensure reliability and performance
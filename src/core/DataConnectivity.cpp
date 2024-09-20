#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "DataConnectivity.h"
#include "WorkbookManager.h"
#include "ExternalDataSource.h"
#include "DatabaseConnector.h"
#include "WebServiceConnector.h"
#include "FileSystemConnector.h"
#include "DataTransformer.h"
#include "Logger.h"

const int MAX_CONCURRENT_CONNECTIONS = 10;

DataConnectivity::DataConnectivity(WorkbookManager* wbManager)
    : workbookManager(wbManager), dataTransformer(std::make_unique<DataTransformer>()), logger() {
    // Initialize the DataConnectivity object with the provided WorkbookManager
    // Create a new DataTransformer object
    // Initialize the logger
}

bool DataConnectivity::connectToDataSource(const std::string& sourceType, const std::string& connectionString) {
    // Check if the number of existing connections is less than MAX_CONCURRENT_CONNECTIONS
    if (dataSources.size() >= MAX_CONCURRENT_CONNECTIONS) {
        logger.log("Error: Maximum number of concurrent connections reached.");
        return false;
    }

    // Create appropriate ExternalDataSource object based on sourceType
    std::unique_ptr<ExternalDataSource> dataSource;
    if (sourceType == "database") {
        dataSource = std::make_unique<DatabaseConnector>();
    } else if (sourceType == "webservice") {
        dataSource = std::make_unique<WebServiceConnector>();
    } else if (sourceType == "filesystem") {
        dataSource = std::make_unique<FileSystemConnector>();
    } else {
        logger.log("Error: Unknown data source type.");
        return false;
    }

    // Attempt to connect using the connectionString
    bool connectionStatus = dataSource->connect(connectionString);

    // If successful, add the data source to the dataSources map
    if (connectionStatus) {
        std::string sourceId = sourceType + "_" + std::to_string(dataSources.size() + 1);
        dataSources[sourceId] = std::move(dataSource);
        logger.log("Successfully connected to data source: " + sourceId);
    } else {
        logger.log("Failed to connect to data source.");
    }

    // Return the connection status
    return connectionStatus;
}

bool DataConnectivity::importData(const std::string& sourceId, const std::string& query, const std::string& destinationRange) {
    // Check if the sourceId exists in the dataSources map
    auto it = dataSources.find(sourceId);
    if (it == dataSources.end()) {
        logger.log("Error: Data source not found: " + sourceId);
        return false;
    }

    // Execute the query on the data source
    std::vector<std::vector<std::string>> rawData;
    bool querySuccess = it->second->executeQuery(query, rawData);
    if (!querySuccess) {
        logger.log("Error: Failed to execute query on data source: " + sourceId);
        return false;
    }

    // Transform the received data using DataTransformer
    std::vector<std::vector<std::string>> transformedData = dataTransformer->transform(rawData);

    // Use WorkbookManager to insert the transformed data into the destinationRange
    bool insertSuccess = workbookManager->insertData(transformedData, destinationRange);
    if (!insertSuccess) {
        logger.log("Error: Failed to insert data into workbook.");
        return false;
    }

    // Log the import operation result
    logger.log("Successfully imported data from source: " + sourceId);

    // Return the import status
    return true;
}

bool DataConnectivity::exportData(const std::string& sourceId, const std::string& sourceRange, const std::string& destinationTable) {
    // Check if the sourceId exists in the dataSources map
    auto it = dataSources.find(sourceId);
    if (it == dataSources.end()) {
        logger.log("Error: Data source not found: " + sourceId);
        return false;
    }

    // Retrieve data from the sourceRange using WorkbookManager
    std::vector<std::vector<std::string>> sourceData;
    bool retrieveSuccess = workbookManager->retrieveData(sourceRange, sourceData);
    if (!retrieveSuccess) {
        logger.log("Error: Failed to retrieve data from workbook.");
        return false;
    }

    // Transform the data using DataTransformer
    std::vector<std::vector<std::string>> transformedData = dataTransformer->transform(sourceData);

    // Execute the export operation on the data source
    bool exportSuccess = it->second->exportData(transformedData, destinationTable);
    if (!exportSuccess) {
        logger.log("Error: Failed to export data to data source: " + sourceId);
        return false;
    }

    // Log the export operation result
    logger.log("Successfully exported data to source: " + sourceId);

    // Return the export status
    return true;
}

bool DataConnectivity::refreshConnection(const std::string& sourceId) {
    // Check if the sourceId exists in the dataSources map
    auto it = dataSources.find(sourceId);
    if (it == dataSources.end()) {
        logger.log("Error: Data source not found: " + sourceId);
        return false;
    }

    // Attempt to reconnect to the data source
    bool refreshStatus = it->second->reconnect();

    // Log the refresh attempt result
    if (refreshStatus) {
        logger.log("Successfully refreshed connection to data source: " + sourceId);
    } else {
        logger.log("Failed to refresh connection to data source: " + sourceId);
    }

    // Return the refresh status
    return refreshStatus;
}

bool DataConnectivity::closeConnection(const std::string& sourceId) {
    // Check if the sourceId exists in the dataSources map
    auto it = dataSources.find(sourceId);
    if (it == dataSources.end()) {
        logger.log("Error: Data source not found: " + sourceId);
        return false;
    }

    // Close the connection to the data source
    bool closeStatus = it->second->disconnect();

    // Remove the data source from the dataSources map
    dataSources.erase(it);

    // Log the connection closure
    if (closeStatus) {
        logger.log("Successfully closed connection to data source: " + sourceId);
    } else {
        logger.log("Failed to close connection to data source: " + sourceId);
    }

    // Return the closure status
    return closeStatus;
}

// Human tasks:
// TODO: Implement error handling for network failures during data import/export
// TODO: Add support for additional data source types (e.g., NoSQL databases, cloud storage services)
// TODO: Implement data caching mechanism to improve performance for frequently accessed data
// TODO: Add support for incremental data updates to minimize data transfer
// TODO: Implement data source connection pooling for improved efficiency
// TODO: Add support for custom data transformations defined by users
// TODO: Implement data source authentication methods (e.g., OAuth, API keys)
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include "CollaborationServices.h"
#include "WorkbookManager.h"
#include "CellManager.h"
#include "User.h"
#include "WebSocketManager.h"
#include "ConflictResolver.h"
#include "Logger.h"

const int MAX_RECONNECT_ATTEMPTS = 5;
const std::chrono::milliseconds RECONNECT_DELAY = std::chrono::milliseconds(1000);

CollaborationServices::CollaborationServices(WorkbookManager* workbookManager, CellManager* cellManager)
    : m_isRunning(false), m_workbookManager(workbookManager), m_cellManager(cellManager) {
    // Initialize member variables
    m_webSocketManager = std::make_unique<WebSocketManager>();
    m_conflictResolver = std::make_unique<ConflictResolver>();
}

bool CollaborationServices::startCollaboration(const std::string& workbookId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // Set m_isRunning to true
    m_isRunning = true;

    // Attempt to connect to the collaboration server
    if (!m_webSocketManager->connect(workbookId)) {
        Logger::error("Failed to connect to collaboration server");
        m_isRunning = false;
        return false;
    }

    // Start the message processing thread
    std::thread messageThread(&CollaborationServices::processIncomingMessages, this);
    messageThread.detach();

    Logger::info("Collaboration started for workbook: " + workbookId);
    return true;
}

void CollaborationServices::stopCollaboration() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // Set m_isRunning to false
    m_isRunning = false;

    // Disconnect from the collaboration server
    m_webSocketManager->disconnect();

    // Join the message processing thread (if it's joinable)
    // Note: In this implementation, we detached the thread, so we don't need to join it here.
    
    Logger::info("Collaboration stopped");
}

void CollaborationServices::broadcastChange(const std::string& cellAddress, const std::string& newValue) {
    // Create a change message
    std::string changeMessage = "CHANGE|" + cellAddress + "|" + newValue;

    // Send the message through WebSocketManager
    m_webSocketManager->sendMessage(changeMessage);

    Logger::debug("Broadcasted change: " + changeMessage);
}

void CollaborationServices::processIncomingMessages() {
    while (m_isRunning) {
        // Continuously receive messages while m_isRunning is true
        std::string message = m_webSocketManager->receiveMessage();

        if (!message.empty()) {
            // Parse incoming messages
            // This is a simplified parsing. In a real implementation, you'd want more robust parsing.
            size_t pos = message.find("|");
            std::string messageType = message.substr(0, pos);
            std::string messageContent = message.substr(pos + 1);

            if (messageType == "CHANGE") {
                // Resolve conflicts if necessary
                if (m_conflictResolver->resolveConflict(messageContent)) {
                    // Apply changes to the workbook
                    size_t separatorPos = messageContent.find("|");
                    std::string cellAddress = messageContent.substr(0, separatorPos);
                    std::string newValue = messageContent.substr(separatorPos + 1);
                    m_cellManager->updateCell(cellAddress, newValue);

                    // Update UI if needed
                    // Note: This would typically be done through a callback or signal to the UI layer
                    Logger::info("Cell updated: " + cellAddress + " = " + newValue);
                }
            }
            // Handle other message types as needed
        }
    }
}

void CollaborationServices::updateUserPresence(const User& user, bool isOnline) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Update user status in m_activeUsers
    auto it = std::find_if(m_activeUsers.begin(), m_activeUsers.end(),
                           [&user](const User& u) { return u.getId() == user.getId(); });
    
    if (it != m_activeUsers.end()) {
        if (isOnline) {
            it->setStatus("online");
        } else {
            m_activeUsers.erase(it);
        }
    } else if (isOnline) {
        m_activeUsers.push_back(user);
    }

    // Broadcast presence update to all collaborators
    std::string presenceMessage = "PRESENCE|" + user.getId() + "|" + (isOnline ? "online" : "offline");
    m_webSocketManager->sendMessage(presenceMessage);

    Logger::info("User presence updated: " + user.getId() + " is " + (isOnline ? "online" : "offline"));
}

bool reconnectToServer(WebSocketManager& wsManager) {
    for (int attempt = 0; attempt < MAX_RECONNECT_ATTEMPTS; ++attempt) {
        if (wsManager.connect()) {
            Logger::info("Reconnected to server successfully");
            return true;
        }
        std::this_thread::sleep_for(RECONNECT_DELAY);
    }
    Logger::error("Failed to reconnect after " + std::to_string(MAX_RECONNECT_ATTEMPTS) + " attempts");
    return false;
}

// Human tasks:
// TODO: Implement robust error handling for network failures
// TODO: Add support for offline mode and syncing when connection is restored
// TODO: Optimize performance for large numbers of simultaneous collaborators
// TODO: Implement end-to-end encryption for enhanced security
// TODO: Add support for selective sharing of worksheet ranges
// TODO: Implement undo/redo functionality for collaborative changes
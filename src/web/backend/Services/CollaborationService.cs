using System;
using System.Collections.Concurrent;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;
using Microsoft.Extensions.Logging;
using ExcelWebApp.Models;
using ExcelWebApp.Interfaces;
using ExcelWebApp.Hubs;

namespace ExcelWebApp.Services
{
    public class CollaborationService
    {
        private const string HubName = "collaborationHub";

        private readonly IHubContext<CollaborationHub> _hubContext;
        private readonly IWorkbookService _workbookService;
        private readonly ILogger<CollaborationService> _logger;
        private readonly ConcurrentDictionary<string, HashSet<string>> _activeUsers;

        public CollaborationService(IHubContext<CollaborationHub> hubContext, IWorkbookService workbookService, ILogger<CollaborationService> logger)
        {
            // Initialize dependencies
            _hubContext = hubContext;
            _workbookService = workbookService;
            _logger = logger;
            _activeUsers = new ConcurrentDictionary<string, HashSet<string>>();
        }

        public async Task JoinWorkbook(string userId, string workbookId)
        {
            // Add the user to the _activeUsers dictionary for the given workbookId
            _activeUsers.AddOrUpdate(workbookId, 
                new HashSet<string> { userId }, 
                (key, existingSet) => { existingSet.Add(userId); return existingSet; });

            // Notify other users in the workbook about the new user joining
            await _hubContext.Clients.Group(workbookId).SendAsync("UserJoined", userId);

            // Add the user to the SignalR group for the workbook
            await _hubContext.Groups.AddToGroupAsync(userId, workbookId);

            _logger.LogInformation($"User {userId} joined workbook {workbookId}");
        }

        public async Task LeaveWorkbook(string userId, string workbookId)
        {
            // Remove the user from the _activeUsers dictionary for the given workbookId
            if (_activeUsers.TryGetValue(workbookId, out var users))
            {
                users.Remove(userId);
                if (users.Count == 0)
                {
                    _activeUsers.TryRemove(workbookId, out _);
                }
            }

            // Notify other users in the workbook about the user leaving
            await _hubContext.Clients.Group(workbookId).SendAsync("UserLeft", userId);

            // Remove the user from the SignalR group for the workbook
            await _hubContext.Groups.RemoveFromGroupAsync(userId, workbookId);

            _logger.LogInformation($"User {userId} left workbook {workbookId}");
        }

        public async Task BroadcastCellUpdate(string workbookId, string worksheetId, string cellId, string newValue, string updatedBy)
        {
            // Update the cell value using _workbookService
            await _workbookService.UpdateCellValue(workbookId, worksheetId, cellId, newValue);

            // Broadcast the cell update to all users in the workbook's SignalR group
            await _hubContext.Clients.Group(workbookId).SendAsync("CellUpdated", worksheetId, cellId, newValue, updatedBy);

            // Log the cell update event
            _logger.LogInformation($"Cell {cellId} in worksheet {worksheetId} of workbook {workbookId} updated by {updatedBy}");
        }

        public async Task<string> HandleConflict(string workbookId, string worksheetId, string cellId, string newValue1, string newValue2)
        {
            // Implement a conflict resolution strategy (e.g., last write wins)
            string resolvedValue = newValue2; // Simple last-write-wins strategy

            // Update the cell with the resolved value
            await _workbookService.UpdateCellValue(workbookId, worksheetId, cellId, resolvedValue);

            // Notify all users about the conflict resolution
            await _hubContext.Clients.Group(workbookId).SendAsync("ConflictResolved", worksheetId, cellId, resolvedValue);

            // Log the conflict resolution
            _logger.LogWarning($"Conflict resolved for cell {cellId} in worksheet {worksheetId} of workbook {workbookId}");

            // Return the resolved value
            return resolvedValue;

            // TODO: Implement a more sophisticated conflict resolution strategy if needed
        }

        public async Task<Workbook> SynchronizeWorkbook(string userId, string workbookId)
        {
            // Retrieve the latest workbook state from _workbookService
            Workbook latestWorkbookState = await _workbookService.GetWorkbook(workbookId);

            // Send the full workbook state to the requesting user
            await _hubContext.Clients.User(userId).SendAsync("WorkbookSynchronized", latestWorkbookState);

            // Log the synchronization event
            _logger.LogInformation($"Workbook {workbookId} synchronized for user {userId}");

            return latestWorkbookState;
        }
    }
}
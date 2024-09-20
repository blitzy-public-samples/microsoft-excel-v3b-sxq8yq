-- Migration: 002_AddCollaborationSupport

-- Human tasks:
-- TODO: Review and validate the migration script for potential conflicts with existing data
-- TODO: Test the migration on a staging environment before applying to production
-- TODO: Update application code to utilize new collaboration features after migration

-- Add a new table for real-time collaboration sessions
CREATE TABLE CollaborationSessions (
    session_id UUID PRIMARY KEY,
    workbook_id INT REFERENCES Workbooks(workbook_id),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    expires_at TIMESTAMP
);

-- Add a table for tracking user presence in collaboration sessions
CREATE TABLE UserPresence (
    presence_id UUID PRIMARY KEY,
    session_id UUID REFERENCES CollaborationSessions(session_id),
    user_id INT REFERENCES Users(user_id),
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_active TIMESTAMP,
    UNIQUE(session_id, user_id)
);

-- Add a table for collaboration events
CREATE TABLE CollaborationEvents (
    event_id UUID PRIMARY KEY,
    session_id UUID REFERENCES CollaborationSessions(session_id),
    user_id INT REFERENCES Users(user_id),
    event_type VARCHAR(50),
    event_data JSONB,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Add a column to Workbooks table to track collaboration status
ALTER TABLE Workbooks ADD COLUMN is_collaborative BOOLEAN DEFAULT FALSE;

-- Add a column to Cells table to track the last user who modified the cell
ALTER TABLE Cells ADD COLUMN last_modified_by INT REFERENCES Users(user_id);

-- Add indexes to improve query performance
CREATE INDEX idx_collaboration_sessions_workbook ON CollaborationSessions(workbook_id);
CREATE INDEX idx_user_presence_session ON UserPresence(session_id);
CREATE INDEX idx_collaboration_events_session ON CollaborationEvents(session_id);
CREATE INDEX idx_collaboration_events_created_at ON CollaborationEvents(created_at);
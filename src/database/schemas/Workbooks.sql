-- Create the Workbooks table
CREATE TABLE Workbooks (
    -- Primary key for the table
    workbook_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- Foreign key referencing the Users table
    user_id INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES Users(user_id) ON DELETE CASCADE,
    
    -- Name of the workbook
    name VARCHAR(255) NOT NULL,
    
    -- Timestamp for when the workbook was created
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    
    -- Timestamp for when the workbook was last modified
    last_modified DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    -- Size of the workbook file in bytes
    file_size BIGINT NOT NULL DEFAULT 0,
    
    -- Version number of the workbook
    version INT NOT NULL DEFAULT 1,
    
    -- Flag to indicate if the workbook is a template
    is_template BOOLEAN NOT NULL DEFAULT FALSE,
    
    -- Timestamp for when the workbook was last accessed
    last_accessed DATETIME NULL,
    
    -- Storage location of the workbook file
    storage_location VARCHAR(255) NOT NULL
);

-- Create indexes for improved query performance
CREATE INDEX idx_user_id ON Workbooks(user_id);
CREATE INDEX idx_name ON Workbooks(name);
CREATE INDEX idx_created_at ON Workbooks(created_at);
CREATE INDEX idx_last_modified ON Workbooks(last_modified);

-- Human tasks:
-- TODO: Review and optimize index choices based on query patterns
-- TODO: Consider adding additional columns for metadata or collaboration features
-- TODO: Implement proper data retention policies and archiving strategies
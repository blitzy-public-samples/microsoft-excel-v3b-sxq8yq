-- Create the Worksheets table
CREATE TABLE Worksheets (
    -- Primary key for the worksheet
    worksheet_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- Foreign key referencing the Workbooks table
    workbook_id INT NOT NULL,
    
    -- Name of the worksheet
    name VARCHAR(255) NOT NULL,
    
    -- Index of the worksheet within the workbook
    `index` INT NOT NULL,
    
    -- Visibility status of the worksheet
    visible BOOLEAN NOT NULL DEFAULT TRUE,
    
    -- Tab color of the worksheet (in hex format)
    tab_color VARCHAR(7) DEFAULT NULL,
    
    -- Zoom level of the worksheet
    zoom_level DECIMAL(5,2) NOT NULL DEFAULT 100.00,
    
    -- Timestamp for when the worksheet was created
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    
    -- Timestamp for when the worksheet was last updated
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    -- Foreign key constraint
    FOREIGN KEY (workbook_id) REFERENCES Workbooks(workbook_id) ON DELETE CASCADE,
    
    -- Unique index on workbook_id and index
    UNIQUE INDEX idx_workbook_worksheet (workbook_id, `index`)
);

-- Human tasks:
-- 1. Review and validate the schema design with the database team
-- 2. Ensure the schema aligns with the latest Excel worksheet features
-- 3. Consider adding any additional columns for future extensibility
-- 4. Verify that the index and foreign key constraints are optimal for expected query patterns
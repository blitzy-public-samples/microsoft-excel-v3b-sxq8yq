-- Create the Cells table to store individual cell data for Excel worksheets
CREATE TABLE Cells (
    -- Primary key for the cell
    cell_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- Foreign key referencing the Worksheets table
    worksheet_id INT NOT NULL,
    FOREIGN KEY (worksheet_id) REFERENCES Worksheets(worksheet_id),
    
    -- Row and column coordinates of the cell
    `row` INT NOT NULL,
    `column` INT NOT NULL,
    
    -- Actual and formatted values of the cell
    value TEXT,
    formatted_value TEXT,
    
    -- Data type of the cell (e.g., string, number, date)
    data_type VARCHAR(20) NOT NULL,
    
    -- Formula used in the cell, if any
    formula TEXT,
    
    -- Foreign key referencing the Styles table
    style_id INT,
    FOREIGN KEY (style_id) REFERENCES Styles(style_id),
    
    -- Flag to indicate if the cell is locked
    is_locked BOOLEAN DEFAULT FALSE,
    
    -- Timestamps for creation and last update
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Create a unique index on worksheet_id, row, and column to ensure cell uniqueness within a worksheet
CREATE UNIQUE INDEX idx_worksheet_row_column ON Cells (worksheet_id, `row`, `column`);

-- Create an index on worksheet_id for faster queries on worksheet level
CREATE INDEX idx_worksheet_id ON Cells (worksheet_id);

-- Human tasks:
-- TODO: Review and optimize index choices based on query patterns
-- TODO: Consider adding partitioning strategy for large worksheets
-- TODO: Implement proper error handling and constraints for data validation
-- TODO: Ensure compatibility with chosen database system (e.g., SQL Server, PostgreSQL)
-- Create the PivotTables table to store information about pivot tables in Excel workbooks
CREATE TABLE PivotTables (
    -- Primary key for the table
    pivottable_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- Foreign key referencing the Worksheets table
    worksheet_id INT NOT NULL,
    FOREIGN KEY (worksheet_id) REFERENCES Worksheets(worksheet_id),
    
    -- Name of the pivot table
    name VARCHAR(255) NOT NULL,
    
    -- Source range of data for the pivot table
    source_range VARCHAR(255) NOT NULL,
    
    -- Range where the pivot table is located
    pivot_range VARCHAR(255) NOT NULL,
    
    -- JSON field to store row fields configuration
    row_fields JSON NOT NULL,
    
    -- JSON field to store column fields configuration
    column_fields JSON NOT NULL,
    
    -- JSON field to store value fields configuration
    value_fields JSON NOT NULL,
    
    -- JSON field to store filter fields configuration
    filter_fields JSON NOT NULL,
    
    -- Timestamp for when the record was created
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    
    -- Timestamp for when the record was last updated
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Create an index on the worksheet_id column for faster lookups
CREATE INDEX idx_worksheet_id ON PivotTables(worksheet_id);

-- Create an index on the name column for faster searches by pivot table name
CREATE INDEX idx_name ON PivotTables(name);

-- Human tasks:
-- TODO: Review and optimize index choices based on query patterns
-- TODO: Consider adding additional columns for pivot table styling and formatting options
-- TODO: Implement proper error handling and constraints for JSON fields
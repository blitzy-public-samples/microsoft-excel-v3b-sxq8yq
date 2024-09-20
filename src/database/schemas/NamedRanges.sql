-- Create the NamedRanges table to store information about named ranges within Excel workbooks
CREATE TABLE NamedRanges (
    -- Primary key for the table
    namedrange_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- Foreign key referencing the Workbooks table
    workbook_id INT NOT NULL,
    
    -- Name of the named range
    name VARCHAR(255) NOT NULL,
    
    -- Range definition (e.g., 'Sheet1!A1:B10')
    range VARCHAR(1024) NOT NULL,
    
    -- Scope of the named range (default is 'Workbook')
    scope VARCHAR(50) NOT NULL DEFAULT 'Workbook',
    
    -- Optional comment for the named range
    comment TEXT,
    
    -- Timestamp for when the record was created
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    
    -- Timestamp for when the record was last updated
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    -- Foreign key constraint to ensure referential integrity with the Workbooks table
    FOREIGN KEY (workbook_id) REFERENCES Workbooks(workbook_id) ON DELETE CASCADE ON UPDATE CASCADE,
    
    -- Unique constraint to prevent duplicate named ranges within the same workbook and scope
    UNIQUE (workbook_id, name, scope)
);

-- Create an index on the workbook_id column for faster lookups
CREATE INDEX idx_namedranges_workbook ON NamedRanges(workbook_id);

-- Create an index on the name column for faster searches by name
CREATE INDEX idx_namedranges_name ON NamedRanges(name);

-- Human tasks:
-- TODO: Review and approve the schema design
-- TODO: Ensure the schema aligns with the specific requirements of the Excel application
-- TODO: Consider adding any additional columns or constraints based on the application's needs
-- TODO: Verify that the index choices are appropriate for expected query patterns
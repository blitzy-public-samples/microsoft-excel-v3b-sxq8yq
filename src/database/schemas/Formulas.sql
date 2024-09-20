-- Create the Formulas table to store formula expressions used in Excel cells
CREATE TABLE Formulas (
    -- Primary key for the table, auto-incrementing
    formula_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- The actual formula expression, stored as TEXT to accommodate long formulas
    expression TEXT NOT NULL,
    
    -- Timestamp for when the formula was created
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    
    -- Timestamp for when the formula was last updated, automatically updates on change
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Create an index on the expression column for faster searches
-- Note: We're indexing only the first 255 characters due to MySQL limitations on index key size
CREATE INDEX idx_expression ON Formulas (expression(255)) USING BTREE;

-- Human tasks:
-- TODO: Review and optimize index creation for frequently used queries
-- TODO: Consider adding a column for formula complexity or computation time for performance tracking
-- TODO: Implement proper error handling and validation for formula expressions
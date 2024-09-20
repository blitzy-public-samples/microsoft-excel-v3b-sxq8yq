-- Sample data for the Users table
INSERT INTO Users (username, email, password_hash, last_login)
VALUES 
    ('john_doe', 'john.doe@example.com', '<hashed_password>', CURRENT_TIMESTAMP),
    ('jane_smith', 'jane.smith@example.com', '<hashed_password>', CURRENT_TIMESTAMP);

-- Sample data for the Workbooks table
INSERT INTO Workbooks (user_id, name, created_at, last_modified)
VALUES 
    (1, 'Sample Budget', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP),
    (2, 'Project Timeline', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);

-- Sample data for the Worksheets table
INSERT INTO Worksheets (workbook_id, name, index)
VALUES 
    (1, 'Monthly Budget', 0),
    (1, 'Annual Summary', 1),
    (2, 'Tasks', 0),
    (2, 'Resources', 1);

-- Sample data for the Cells table
INSERT INTO Cells (worksheet_id, address, value, style_id)
VALUES 
    (1, 'A1', 'Category', 1),
    (1, 'B1', 'Amount', 1),
    (1, 'A2', 'Income', 2),
    (1, 'B2', '5000', 3),
    (3, 'A1', 'Task', 1),
    (3, 'B1', 'Due Date', 1);

-- Sample data for the Styles table
INSERT INTO Styles (font, color, background, border)
VALUES 
    ('Arial', '#000000', '#FFFFFF', 'thin'),
    ('Arial', '#000000', '#E0E0E0', 'thin'),
    ('Arial', '#000000', '#FFFFFF', 'thin');

-- Sample data for the Formulas table
INSERT INTO Formulas (expression)
VALUES 
    ('SUM(B2:B10)'),
    ('AVERAGE(B2:B10)');

-- Sample data for the Charts table
INSERT INTO Charts (worksheet_id, type, data_range)
VALUES 
    (1, 'pie', 'A2:B10');

-- Sample data for the PivotTables table
INSERT INTO PivotTables (worksheet_id, source_range, pivot_range)
VALUES 
    (2, 'Sheet1!A1:D100', 'A1:C20');

-- Sample data for the NamedRanges table
INSERT INTO NamedRanges (workbook_id, name, range)
VALUES 
    (1, 'MonthlyIncome', 'Sheet1!B2');

-- Sample data for the Comments table
INSERT INTO Comments (cell_id, user_id, content, created_at)
VALUES 
    (4, 1, 'This is the total monthly income', CURRENT_TIMESTAMP);

-- Human tasks (commented out):
-- TODO: Review and adjust sample data to match specific testing scenarios
-- TODO: Ensure sample data covers all edge cases and potential user inputs
-- TODO: Add more diverse and realistic data sets for different use cases (e.g., financial modeling, project management, inventory tracking)
-- TODO: Implement data generation scripts for creating larger volumes of sample data
-- TODO: Create sample data for demonstrating Excel's advanced features (e.g., conditional formatting, data validation, complex formulas)
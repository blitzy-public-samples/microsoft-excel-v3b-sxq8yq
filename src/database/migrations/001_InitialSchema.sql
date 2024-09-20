-- Create Users table
CREATE TABLE Users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    last_login DATETIME
);

-- Create Workbooks table
CREATE TABLE Workbooks (
    workbook_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    name VARCHAR(255) NOT NULL,
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    last_modified DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
);

-- Create Worksheets table
CREATE TABLE Worksheets (
    worksheet_id INT PRIMARY KEY AUTO_INCREMENT,
    workbook_id INT NOT NULL,
    name VARCHAR(255) NOT NULL,
    index INT NOT NULL,
    FOREIGN KEY (workbook_id) REFERENCES Workbooks(workbook_id)
);

-- Create Cells table
CREATE TABLE Cells (
    cell_id INT PRIMARY KEY AUTO_INCREMENT,
    worksheet_id INT NOT NULL,
    address VARCHAR(10) NOT NULL,
    value TEXT,
    style_id INT,
    formula_id INT,
    FOREIGN KEY (worksheet_id) REFERENCES Worksheets(worksheet_id)
);

-- Create Styles table
CREATE TABLE Styles (
    style_id INT PRIMARY KEY AUTO_INCREMENT,
    font VARCHAR(50),
    font_size INT,
    font_color VARCHAR(7),
    background_color VARCHAR(7),
    border VARCHAR(20)
);

-- Create Formulas table
CREATE TABLE Formulas (
    formula_id INT PRIMARY KEY AUTO_INCREMENT,
    expression TEXT NOT NULL
);

-- Create Charts table
CREATE TABLE Charts (
    chart_id INT PRIMARY KEY AUTO_INCREMENT,
    worksheet_id INT NOT NULL,
    type VARCHAR(50) NOT NULL,
    data_range VARCHAR(255) NOT NULL,
    FOREIGN KEY (worksheet_id) REFERENCES Worksheets(worksheet_id)
);

-- Create PivotTables table
CREATE TABLE PivotTables (
    pivottable_id INT PRIMARY KEY AUTO_INCREMENT,
    worksheet_id INT NOT NULL,
    source_range VARCHAR(255) NOT NULL,
    pivot_range VARCHAR(255) NOT NULL,
    FOREIGN KEY (worksheet_id) REFERENCES Worksheets(worksheet_id)
);

-- Create NamedRanges table
CREATE TABLE NamedRanges (
    namedrange_id INT PRIMARY KEY AUTO_INCREMENT,
    workbook_id INT NOT NULL,
    name VARCHAR(255) NOT NULL,
    range VARCHAR(255) NOT NULL,
    FOREIGN KEY (workbook_id) REFERENCES Workbooks(workbook_id)
);

-- Create Comments table
CREATE TABLE Comments (
    comment_id INT PRIMARY KEY AUTO_INCREMENT,
    cell_id INT NOT NULL,
    user_id INT NOT NULL,
    content TEXT NOT NULL,
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (cell_id) REFERENCES Cells(cell_id),
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
);

-- Create VersionHistory table
CREATE TABLE VersionHistory (
    version_id INT PRIMARY KEY AUTO_INCREMENT,
    workbook_id INT NOT NULL,
    delta LONGBLOB NOT NULL,
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (workbook_id) REFERENCES Workbooks(workbook_id)
);

-- Human tasks:
-- 1. Review and validate the initial schema design
-- 2. Consider adding indexes for frequently queried columns
-- 3. Evaluate the need for additional constraints (e.g., CHECK constraints)
-- 4. Assess the data types and sizes for optimal performance
-- 5. Consider adding audit columns (created_by, updated_by) if needed
-- 6. Review naming conventions for consistency across the schema
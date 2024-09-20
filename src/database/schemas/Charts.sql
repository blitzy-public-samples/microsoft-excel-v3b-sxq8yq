-- Create the Charts table
CREATE TABLE Charts (
    chart_id INT PRIMARY KEY AUTO_INCREMENT,
    worksheet_id INT NOT NULL,
    name VARCHAR(255) NOT NULL,
    type VARCHAR(50) NOT NULL,
    data_range VARCHAR(255) NOT NULL,
    title VARCHAR(255),
    x_axis_label VARCHAR(255),
    y_axis_label VARCHAR(255),
    legend_position VARCHAR(50),
    configuration TEXT NOT NULL COMMENT 'Stores JSON or XML representation of chart configuration',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (worksheet_id) REFERENCES Worksheets(worksheet_id)
);

-- Create index on worksheet_id for faster lookups
CREATE INDEX idx_worksheet_id ON Charts(worksheet_id);

-- Create index on chart type for potential filtering and aggregation
CREATE INDEX idx_chart_type ON Charts(type);

-- Human tasks:
-- TODO: Review and optimize indexes for query performance
-- TODO: Consider adding additional metadata fields for chart customization
-- TODO: Implement proper data type for storing chart configuration (e.g., JSON or XML)
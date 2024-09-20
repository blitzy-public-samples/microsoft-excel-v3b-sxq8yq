-- Create the Styles table to store formatting information for cells in Excel workbooks
CREATE TABLE Styles (
    -- Primary key for the table
    style_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- Font properties
    font_name VARCHAR(50) NOT NULL,
    font_size DECIMAL(4,2) NOT NULL,
    font_color VARCHAR(7) NOT NULL,
    is_bold BOOLEAN NOT NULL DEFAULT FALSE,
    is_italic BOOLEAN NOT NULL DEFAULT FALSE,
    is_underline BOOLEAN NOT NULL DEFAULT FALSE,
    
    -- Cell background
    background_color VARCHAR(7) NOT NULL,
    
    -- Text alignment
    horizontal_alignment ENUM('left', 'center', 'right', 'justify') NOT NULL DEFAULT 'left',
    vertical_alignment ENUM('top', 'middle', 'bottom') NOT NULL DEFAULT 'bottom',
    
    -- Text wrapping
    wrap_text BOOLEAN NOT NULL DEFAULT FALSE,
    
    -- Border styles
    border_top VARCHAR(20) DEFAULT NULL,
    border_right VARCHAR(20) DEFAULT NULL,
    border_bottom VARCHAR(20) DEFAULT NULL,
    border_left VARCHAR(20) DEFAULT NULL,
    
    -- Number format
    number_format VARCHAR(50) DEFAULT NULL,
    
    -- Timestamps for record keeping
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- Create indexes for improved query performance
CREATE INDEX idx_font_name ON Styles(font_name);
CREATE INDEX idx_background_color ON Styles(background_color);

-- Human tasks:
-- 1. Review and optimize index choices based on query patterns
-- 2. Consider adding a composite index for frequently used style combinations
-- 3. Evaluate the need for additional columns based on Excel's full range of styling options
-- 4. Implement proper data validation and constraints for color values (e.g., ensure they are valid hex codes)
-- 5. Consider adding a 'style_hash' column for quick lookups of identical styles to reduce duplication
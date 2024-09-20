-- Create the Comments table to store user comments associated with specific cells in Excel worksheets
CREATE TABLE Comments (
    -- Unique identifier for each comment
    comment_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- Reference to the cell this comment is associated with
    cell_id INT NOT NULL,
    FOREIGN KEY (cell_id) REFERENCES Cells(cell_id),
    
    -- User who created the comment
    user_id INT NOT NULL,
    FOREIGN KEY (user_id) REFERENCES Users(user_id),
    
    -- The actual content of the comment
    content TEXT NOT NULL,
    
    -- Timestamp for when the comment was created
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    
    -- Timestamp for when the comment was last updated
    updated_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    -- Flag to indicate if the comment has been resolved
    is_resolved BOOLEAN NOT NULL DEFAULT FALSE,
    
    -- User who resolved the comment (if resolved)
    resolved_by INT,
    FOREIGN KEY (resolved_by) REFERENCES Users(user_id),
    
    -- Timestamp for when the comment was resolved
    resolved_at DATETIME
);

-- Create an index on cell_id for faster lookups of comments by cell
CREATE INDEX idx_cell_id ON Comments(cell_id);

-- Create an index on user_id for faster lookups of comments by user
CREATE INDEX idx_user_id ON Comments(user_id);

-- Create an index on created_at for faster sorting and filtering by creation date
CREATE INDEX idx_created_at ON Comments(created_at);

-- Human tasks:
-- TODO: Review and optimize index choices based on query patterns
-- TODO: Consider adding a parent_comment_id for nested comments if required
-- TODO: Implement proper cascading delete rules for related cells and users
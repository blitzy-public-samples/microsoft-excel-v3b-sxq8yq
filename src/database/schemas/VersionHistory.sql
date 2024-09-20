-- Create the VersionHistory table to track changes to workbooks over time
CREATE TABLE VersionHistory (
    version_id INT PRIMARY KEY AUTO_INCREMENT,
    workbook_id INT NOT NULL,
    user_id INT NOT NULL,
    delta BLOB NOT NULL,
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    version_number INT NOT NULL,
    comment VARCHAR(255),
    FOREIGN KEY (workbook_id) REFERENCES Workbooks(workbook_id),
    FOREIGN KEY (user_id) REFERENCES Users(user_id),
    UNIQUE INDEX idx_workbook_version (workbook_id, version_number),
    INDEX idx_created_at (created_at)
);

-- Function to retrieve the latest version number for a given workbook
DELIMITER //
CREATE FUNCTION GetLatestVersion(p_workbook_id INT) RETURNS INT
BEGIN
    DECLARE latest_version INT;
    SELECT MAX(version_number) INTO latest_version
    FROM VersionHistory
    WHERE workbook_id = p_workbook_id;
    RETURN COALESCE(latest_version, 0);
END//
DELIMITER ;

-- Procedure to insert a new version record for a workbook
DELIMITER //
CREATE PROCEDURE CreateNewVersion(
    IN p_workbook_id INT,
    IN p_user_id INT,
    IN p_delta BLOB,
    IN p_comment VARCHAR(255),
    OUT p_new_version INT
)
BEGIN
    SET p_new_version = GetLatestVersion(p_workbook_id) + 1;
    INSERT INTO VersionHistory (workbook_id, user_id, delta, version_number, comment)
    VALUES (p_workbook_id, p_user_id, p_delta, p_new_version, p_comment);
END//
DELIMITER ;

-- Human tasks:
-- TODO: Review and optimize the indexes for query performance based on actual usage patterns
-- TODO: Implement a data retention policy for old versions to manage storage growth
-- TODO: Consider adding a trigger to automatically create an initial version when a new workbook is created
-- TODO: Evaluate the need for additional metadata columns such as file size or change type
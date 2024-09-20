-- SQL schema definition for the Users table in the Microsoft Excel database

-- Create the Users table
CREATE TABLE Users (
    -- Primary key for the table, auto-incrementing
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    
    -- Unique username field
    username VARCHAR(50) NOT NULL UNIQUE,
    
    -- Unique email field
    email VARCHAR(100) NOT NULL UNIQUE,
    
    -- Hashed password field
    password_hash VARCHAR(255) NOT NULL,
    
    -- User's first name
    first_name VARCHAR(50),
    
    -- User's last name
    last_name VARCHAR(50),
    
    -- Timestamp for when the user was created
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    
    -- Timestamp for the user's last login
    last_login TIMESTAMP,
    
    -- Boolean flag to indicate if the user is active
    is_active BOOLEAN DEFAULT TRUE,
    
    -- User role, defaulting to 'Editor'
    role ENUM('Viewer', 'Editor', 'Reviewer', 'Owner', 'Admin') DEFAULT 'Editor',
    
    -- JSON field for storing user preferences
    preferences JSON,
    
    -- Index on the email field for faster lookups
    INDEX idx_email (email),
    
    -- Index on the username field for faster lookups
    INDEX idx_username (username)
);

-- Human tasks:
-- 1. Review and approve the user roles defined in the ENUM
-- 2. Confirm the length of VARCHAR fields is appropriate for the application's needs
-- 3. Discuss and finalize the structure of the 'preferences' JSON field
-- 4. Consider adding additional fields for user profile information if needed
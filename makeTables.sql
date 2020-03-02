/*
DROP TABLE IF EXISTS invite;
DROP TABLE IF EXISTS user;
*/

CREATE TABLE IF NOT EXISTS user
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(255) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    upload DOUBLE,
    download DOUBLE,
    privilege INT,
    createdAt DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS invite
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    recipientEmail VARCHAR(255) NOT NULL UNIQUE,
    sender INT NOT NULL,
    inviteKey VARCHAR(255) NOT NULL UNIQUE,
    expDate DATETIME NOT NULL,
    FOREIGN KEY (sender) REFERENCES user(id)
);

CREATE TABLE IF NOT EXISTS text
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    page VARCHAR(255) NOT NULL,
    text VARCHAR(8000),
    createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
    lastEditedAt DATETIME NOT NULL,
    lastEditedBy INT NOT NULL,
    FOREIGN KEY (lastEditedBy) REFERENCES user(id)
);

INSERT INTO text (page, text, lastEditedAt, lastEditedBy)
VALUES ("rules", "The database works!", CURRENT_TIMESTAMP, 1);

DROP TABLE IF EXISTS text;
DROP TABLE IF EXISTS invite;
DROP TABLE IF EXISTS scrapeLog;
DROP TABLE IF EXISTS filesUsers;
DROP TABLE IF EXISTS files;
DROP TABLE IF EXISTS announceLog;
DROP TABLE IF EXISTS user;

CREATE TABLE IF NOT EXISTS user
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(191) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    upload DOUBLE UNSIGNED NOT NULL DEFAULT 0,
    download DOUBLE UNSIGNED NOT NULL DEFAULT 0,
    points INT UNSIGNED,
    privilege INT,
    createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
    canLeech TINYINT NOT NULL DEFAULT 1,
    peersLimit INT NOT NULL DEFAULT 0,
    torrentsLimit INT NOT NULL DEFAULT 0,
    torrentPass CHAR(32) NOT NULL
);

CREATE TABLE IF NOT EXISTS invite
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    recipientEmail VARCHAR(191) NOT NULL UNIQUE,
    sender INT NOT NULL,
    inviteKey VARCHAR(191) NOT NULL UNIQUE,
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

CREATE TABLE IF NOT EXISTS scrapeLog
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    ipa INT UNSIGNED NOT NULL,
    infoHash BINARY(20),
    userId INT NOT NULL,
    modifiedTime DATETIME NOT NULL,
    FOREIGN KEY (userId) REFERENCES user(id)
);

CREATE TABLE IF NOT EXISTS files
(
    fileId INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    infoHash VARCHAR(191) NOT NULL UNIQUE,
    leechers INT NOT NULL DEFAULT 0,
    seeders INT NOT NULL DEFAULT 0,
    completed INT NOT NULL DEFAULT 0,
    flags INT NOT NULL DEFAULT 0,
    modifiedTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    createdTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY (infoHash)
);


CREATE TABLE IF NOT EXISTS filesUsers
(
    fileId INT NOT NULL,
    userId INT NOT NULL,
    isActive TINYINT NOT NULL DEFAULT 1,
    announced INT NOT NULL DEFAULT 1,
    completed INT NOT NULL DEFAULT 0,
    downloaded BIGINT UNSIGNED NOT NULL,
    `left` BIGINT UNSIGNED NOT NULL,
    uploaded BIGINT UNSIGNED NOT NULL,
    modifiedTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    timeSeeded INT DEFAULT 0,
    UNIQUE KEY (fileId, userId),
    FOREIGN KEY (userId) REFERENCES user(id),
    FOREIGN KEY (fileId) REFERENCES files(fileId)
);

CREATE TABLE IF NOT EXISTS denyFromHosts
(
    `begin` INT UNSIGNED NOT NULL,
    `end` INT UNSIGNED NOT NULL
);

CREATE TABLE IF NOT EXISTS denyFromClients
(
    peerId char(20) NOT NULL
);

CREATE TABLE IF NOT EXISTS announceLog
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    ipa VARCHAR(255) NOT NULL,
    port INT NOT NULL,
    event INT NOT NULL,
    infoHash VARCHAR(255) NOT NULL,
    peerId VARCHAR(255) NOT NULL,
    downloaded BIGINT UNSIGNED NOT NULL,
    `left` BIGINT UNSIGNED NOT NULL,
    uploaded BIGINT UNSIGNED NOT NULL,
    userId INT NOT NULL,
    modifiedTime DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (userId) REFERENCES user(id)
);

/* password:123 */
INSERT INTO user (username, email, password, upload, download, points, privilege, torrentPass)
VALUES ("taraldqBit", "taraldqBit", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "taraldqBit"),
("stianqBit", "stianqBit", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "stianqBit"),
("stianDeluge", "stianDeluge", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "stianDeluge"),
("stianrTorrent", "stianrTorrent", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "stianrTorrent"),
 ("taraldDeluge", "taraldDeluge", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "taraldDeluge");

INSERT INTO text (page, text, lastEditedAt, lastEditedBy)
VALUES ("rules", "The database works!", CURRENT_TIMESTAMP, 1);

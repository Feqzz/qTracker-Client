DROP TABLE IF EXISTS clientTorrents;
DROP TABLE IF EXISTS client;
DROP TABLE IF EXISTS userTorrentTotals;
DROP TABLE IF EXISTS ipAddress;
DROP TABLE IF EXISTS torrentFilePaths;
DROP TABLE IF EXISTS torrentFiles;
DROP TABLE IF EXISTS torrent;
DROP TABLE IF EXISTS text;
DROP TABLE IF EXISTS invite;
DROP TABLE IF EXISTS user;

CREATE TABLE IF NOT EXISTS user
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(191) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    points DOUBLE NOT NULL DEFAULT 0,
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

CREATE TABLE IF NOT EXISTS torrent
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    uploader INT NOT NULL,
    infoHash VARCHAR(191) NOT NULL UNIQUE,
    flags INT NOT NULL DEFAULT 0,
    title VARCHAR(255) NOT NULL,
    name VARCHAR(255) NOT NULL,
    uploadDate DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    encoding VARCHAR(255),
    comment VARCHAR(255),
    createdByClient VARCHAR(255),
    createdDate DATETIME,
    pieceLength BIGINT UNSIGNED NOT NULL,
    piece LONGBLOB NOT NULL,
    private TINYINT,
    FOREIGN KEY (uploader) REFERENCES user(id)
);

/*
CREATE TABLE IF NOT EXISTS torrentAnnounce
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    announce VARCHAR(255) NOT NULL,
    FOREIGN KEY (torrentId) REFERENCES torrent(id)
);
*/
/* 
If a torrent only has 1 file, it is 'Single File Mode'
*/

CREATE TABLE IF NOT EXISTS torrentFiles
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    torrentId INT NOT NULL,
    length BIGINT UNSIGNED NOT NULL,

    FOREIGN KEY (torrentId) REFERENCES torrent(id)
);

CREATE TABLE IF NOT EXISTS torrentFilePaths
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    torrentFilesId INT NOT NULL,
    path VARCHAR(255) NOT NULL,

    FOREIGN KEY (torrentFilesId) REFERENCES torrentFiles(id)
);

CREATE TABLE IF NOT EXISTS ipAddress
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    ipa VARCHAR(255) NOT NULL UNIQUE,
    userId INT NOT NULL,
    isBanned TINYINT NOT NULL DEFAULT 0,
    FOREIGN KEY (userId) REFERENCES user(id)
);

CREATE TABLE IF NOT EXISTS userTorrentTotals
(
    torrentId INT NOT NULL,
    userId INT NOT NULL,
    totalDownloaded BIGINT UNSIGNED NOT NULL,
    totalUploaded BIGINT UNSIGNED NOT NULL,
    timeActive INT NOT NULL DEFAULT 0,
    FOREIGN KEY (torrentId) REFERENCES torrent(id),
    FOREIGN KEY (userId) REFERENCES user(id),
    PRIMARY KEY (torrentId,userId)
);

CREATE TABLE IF NOT EXISTS client
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    peerId VARCHAR(191) NOT NULL,
    port INT NOT NULL,
    ipaId INT NOT NULL,
    UNIQUE KEY (port, ipaId),
    FOREIGN KEY (ipaId) REFERENCES ipAddress(id)
);

CREATE TABLE IF NOT EXISTS clientTorrents
(
    torrentId INT NOT NULL,
    clientId INT NOT NULL,
    isActive TINYINT NOT NULL DEFAULT 1,
    announced INT NOT NULL DEFAULT 1,
    completed INT NOT NULL DEFAULT 0,
    downloaded BIGINT UNSIGNED NOT NULL,
    `left` BIGINT UNSIGNED NOT NULL,
    uploaded BIGINT UNSIGNED NOT NULL,
    lastEvent INT NOT NULL,
    timeCreated DATETIME DEFAULT CURRENT_TIMESTAMP,
    lastActivity DATETIME DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY (torrentId, clientId),
    FOREIGN KEY (torrentId) REFERENCES torrent(id),
    FOREIGN KEY (clientId) REFERENCES client(id)
);

/* password:123 */
INSERT INTO user (username, email, password, points, privilege, torrentPass)
VALUES ("test", "test", "ICy5YqxZB1uWSwcVLSNLcA==", 50000, 1, "test"),
("stian", "stian", "ICy5YqxZB1uWSwcVLSNLcA==", 0, 1, "b3769d6c84e5cd97d799dc79f5e43afd");

INSERT INTO text (page, text, lastEditedAt, lastEditedBy)
VALUES ("rules", "The database works!", CURRENT_TIMESTAMP, 1);

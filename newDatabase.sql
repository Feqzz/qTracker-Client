DROP TABLE IF EXISTS text;
DROP TABLE IF EXISTS invite;
DROP TABLE IF EXISTS clientTorrents;
DROP TABLE IF EXISTS client;
DROP TABLE IF EXISTS ipAddress;
DROP TABLE IF EXISTS userTorrentTotals;
DROP TABLE IF EXISTS torrent;
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

CREATE TABLE IF NOT EXISTS torrent
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    uploader INT NOT NULL,
    infoHash VARCHAR(191) NOT NULL UNIQUE,
    leechers INT NOT NULL DEFAULT 0,
    seeders INT NOT NULL DEFAULT 0,
    completed INT NOT NULL DEFAULT 0,
    flags INT NOT NULL DEFAULT 0,
    /*modifiedTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,*/
    createdTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (uploader) REFERENCES user(id)
);

CREATE TABLE IF NOT EXISTS ipAddress
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    ipa VARCHAR(255) NOT NULL,
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
    FOREIGN KEY (torrentId) REFERENCES torrent(id),
    FOREIGN KEY (userId) REFERENCES user(id)
);

CREATE TABLE IF NOT EXISTS client
(
    id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    peerId VARCHAR(191) NOT NULL UNIQUE,
    port INT NOT NULL,
    ipaId INT NOT NULL,
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
    lastActivity DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (torrentId) REFERENCES torrent(id),
    FOREIGN KEY (clientId) REFERENCES client(id)
);

/* password:123 */
INSERT INTO user (username, email, password, upload, download, points, privilege, torrentPass)
VALUES ("taraldqBit", "taraldqBit", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "taraldqBit"),
("stianqBit", "stianqBit", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "stianqBit"),
("stianDeluge", "stianDeluge", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "stianDeluge"),
("stianrTorrent", "stianrTorrent", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "stianrTorrent"),
 ("taraldDeluge", "taraldDeluge", "ICy5YqxZB1uWSwcVLSNLcA==",10, 100, 50, 1, "taraldDeluge");

/* taralds 100gb empty torrent */
INSERT INTO torrent(infoHash,uploader)
VALUES ("0cc80ba9a73ffc5d6ff51dacf3803bfce00",5),
("a5f4630af890cf0151086fbf2813f6cca2ab2b0",2);

INSERT INTO text (page, text, lastEditedAt, lastEditedBy)
VALUES ("rules", "The database works!", CURRENT_TIMESTAMP, 1);

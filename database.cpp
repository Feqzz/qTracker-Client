#include "database.h"
#include "config.cpp"

Database::Database()
{
    this->isConnected = this->connect();
}

bool Database::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(dbHostName);
    db.setDatabaseName(dbDatabaseName);
    db.setUserName(dbUserName);
    db.setPassword(dbPassword);
    this->db = db;
    return db.open();
}

void Database::query(QString str)
{
    QSqlQuery q(this->db);
    q.exec(str);
}

void Database::makeTables()
{
    QString query =
        "CREATE TABLE IF NOT EXISTS user("
        "id INT AUTO_INCREMENT PRIMARY KEY,"
        "username VARCHAR(50) NOT NULL UNIQUE,"
        "password VARCHAR(255) NOT NULL,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")";
    this->query(query);
}

bool Database::login(QString username, QString password)
{
    QString query = "SELECT id, password FROM user WHERE username = ?";

}

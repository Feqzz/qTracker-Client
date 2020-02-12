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
    return db.open();
}

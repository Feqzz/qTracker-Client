#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

class Database
{
public:
    Database();
    void makeTables();
    bool login(QString, QString);
private:
    bool connect();
    void query(QString);
    QSqlDatabase db;
    bool isConnected;
};

#endif // DATABASE_H

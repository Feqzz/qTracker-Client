#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>

class Database
{
public:
    Database();
    void makeTables();
    void login(QString, QString);
    void register_(QString, QString); //Illegal to name it register
    bool isConnected;
private:
    bool uniqueUsername(QString);
    bool connect();
    void query(QString);
    QString hash(QString);
    QSqlDatabase db;
};

#endif // DATABASE_H

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
    QSqlQuery query();
    QString hash(QString);
    void updatePassword(int, QString);
    bool isConnected;
private:
    bool connect();
    QSqlDatabase db;
};

#endif // DATABASE_H

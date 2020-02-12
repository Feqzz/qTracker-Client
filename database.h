#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql/QSqlDatabase>


class Database
{
public:
    Database();
    bool connect();
private:
    bool isConnected;
};

#endif // DATABASE_H

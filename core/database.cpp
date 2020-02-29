#include "database.h"
#include "config.cpp"

Database::Database()
{
    this->isConnected = this->connect();
}

Database::~Database()
{
    db.close();
}

bool Database::connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(dbHostName);
    db.setDatabaseName(dbDatabaseName);
    db.setUserName(dbUserName);
    db.setPassword(dbPassword);
    qDebug() << db.lastError();
    return db.open();
}

QString Database::hash(QString str)
{
    QCryptographicHash *cryp = new QCryptographicHash(QCryptographicHash::Md5);
    cryp->addData(str.toUtf8());
    return cryp->result().toBase64();
}

void Database::updatePassword(int id, QString password)
{
    QSqlQuery q(this->db);
    q.prepare("UPDATE user SET password = :password WHERE id = :id");
    q.bindValue(":password", password);
    q.bindValue(":id", id);
    if (q.exec())
    {
        qDebug() << "Changed password for user with id: " << id;
    }
    else
    {
        qDebug() << "Failed to change password for user with id: " << id;
    }
}

QSqlQuery Database::query()
{
    return QSqlQuery(db);
}

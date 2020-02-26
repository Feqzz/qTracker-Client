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
    qDebug() << db.lastError();
    return db.open();
}

void Database::query(QString str)
{
    QSqlQuery q(this->db);
    q.exec(str);
}

bool Database::uniqueUsername(QString str)
{
    QSqlQuery q(this->db);
    q.prepare("SELECT username FROM user WHERE username = :username");
    q.bindValue(":username", str);
    if (q.exec())
    {
        return (q.size() > 0) ? false : true;
    }
    else
    {
        qDebug("Failed to execute query while checking for unique username");
        return false;
    }
}

void Database::login(QString username, QString password)
{
     QSqlQuery q(this->db);
     q.prepare("SELECT id, password FROM user WHERE username = :username");
     q.bindValue(":username", username);
     if (q.exec())
     {
        QString hashedPassword = hash(password);

        q.next();
        QString dbHashedPassword = q.value(1).toString();
        if(hashedPassword == dbHashedPassword)
        {
            qDebug() << "User logged in!";
            //Redirect user from here.. and use the user id somwhow?
        }
        else
        {
            qDebug() << "Password did not match!";
        }
     }

}

void Database::register_(QString username, QString password)
{
    if (this->uniqueUsername(username))
    {
        QString hashedPassword = hash(password);

        QSqlQuery q(this->db);
        q.prepare("INSERT IGNORE INTO user (username, password)"
                  "VALUES (:username, :password)");
        q.bindValue(":username", username);
        q.bindValue(":password", hashedPassword);
        if (q.exec())
        {
            qDebug() << "User registered";
            login(username, password);
        }
    }
    else
    {
        qDebug() << "Username already taken";
    }
}

QString Database::hash(QString str)
{
    QCryptographicHash *cryp = new QCryptographicHash(QCryptographicHash::Md5);
    cryp->addData(str.toUtf8());
    return cryp->result().toBase64();
}

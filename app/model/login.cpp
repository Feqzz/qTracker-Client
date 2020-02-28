#include "login.h"

Login::Login()
{

}

void Login::login(QString username, QString password)
{
     QSqlQuery q = db->query();
     q.prepare("SELECT id, password FROM user WHERE username = :username");
     q.bindValue(":username", username);
     if (q.exec())
     {
        QString hashedPassword = db->hash(password);

        q.next();
        QString dbHashedPassword = q.value(1).toString();
        if(hashedPassword == dbHashedPassword)
        {
            qDebug() << "User logged in!";
            int id = q.value(0).toInt();
            //Somehow make the User object from the application class :/
            //Redirect user from here.. and use the user id somwhow?
        }
        else
        {
            qDebug() << "Password did not match!";
        }
     }
}

void Login::register_(QString username, QString password, QString email)
{
    if (uniqueUsername(username) && uniqueEmail(email))
    {
        QString hashedPassword = db->hash(password);
        QSqlQuery q = db->query();
        q.prepare("INSERT INTO user (username, password, email)"
                  "VALUES (:username, :password, :email)");
        q.bindValue(":username", username);
        q.bindValue(":password", hashedPassword);
        q.bindValue(":email", email);
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

bool Login::uniqueUsername(QString str)
{
    QSqlQuery q = db->query();
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

bool Login::uniqueEmail(QString str)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT email FROM user WHERE email = :email");
    q.bindValue(":email", str);
    if (q.exec())
    {
        return (q.size() > 0) ? false : true;
    }
    else
    {
        qDebug("Failed to execute query while checking for unique email");
        return false;
    }
}







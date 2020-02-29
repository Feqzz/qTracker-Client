#include "login.h"

Login::Login(QObject *parent) : QObject(parent)
{

}

void Login::test(QString str)
{
    qDebug() << "Hello " << str;
}

bool Login::login(QString username, QString password)
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
            sessionUser = new User(id);
            qDebug() << sessionUser->getId();
            return true;
        }
        else
        {
            qDebug() << "Password did not match!";
        }
     }
     return false;

}

void Login::register_(QString username, QString password, QString email, QString key)
{
    if (uniqueEmail(email))
    {
        if (validInviteKey(key, email))
        {
            if (uniqueUsername(username))
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
        else
        {
            qDebug() << "Not a valid invite key";
        }
    }
    else
    {
        qDebug() << "Email already in use";
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

bool Login::validInviteKey(QString key, QString email)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT inviteKey FROM invite WHERE inviteKey = :key AND recipientEmail = :email AND expDate > now()");
    q.bindValue(":key", key);
    q.bindValue(":email", email);
    if (q.exec())
    {
        q.next();
        return (key != q.value(0).toString()) ? false : true;
    }
    else
    {
        qDebug("Failed to execute query while checking for vaild Invite");
        return false;
    }
}

QString Login::generateKey(QString email)
{
    if(sessionUser->getId())
    {
        QString key = QUuid::createUuid().toString(QUuid::Id128).left(12);
        qDebug() << "Key: " << key;

        QSqlQuery q = db->query();
        q.prepare("INSERT INTO invite (recipientEmail, sender, InviteKey, expDate) "
                  "VALUES (:email, :sender, :key, DATE_ADD( NOW(), INTERVAL 48 HOUR ))");
        q.bindValue(":email", email);
        q.bindValue(":sender", sessionUser->getId());
        q.bindValue(":key", key);
        if (q.exec())
        {
            return key;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        qDebug() << "No user is logged in";
        return NULL;
    }
}

#include "login.h"

Login::Login(QObject *parent) : QObject(parent)
{

}

Login::Login(User* a)
{
    user = a;
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
            user = new User(id);
            return true;
        }
        else
        {
            qDebug() << "Password did not match!";
            errorMessage = "Password did not match!";
        }
     }
     return false;

}

bool Login::registerUser(QString username, QString password, QString email, QString key)
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
                    return true;
                }
            }
            else
            {
                errorMessage = "Username already taken";
                qDebug() << errorMessage;
            }
        }
        else
        {
            errorMessage = "Not a valid invite key";
            qDebug() << errorMessage;
        }
    }
    else
    {
        errorMessage = "Email already in use";
        qDebug() << errorMessage;
    }
    return false;
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
    if (key != "")
    {
        QSqlQuery q = db->query();
        q.prepare("SELECT inviteKey FROM invite WHERE inviteKey = :key AND recipientEmail = :email AND expDate > now()");
        q.bindValue(":key", key);
        q.bindValue(":email", email);
        if (q.exec())
        {
            q.next();
            return (key == q.value(0).toString()) ? true : false;
        }
        else
        {
            qDebug("Failed to execute query while checking for vaild Invite");
            return false;
        }
    }
    else
    {
        return false;
    }
}

QString Login::getErrorMessage()
{
    return errorMessage;
}

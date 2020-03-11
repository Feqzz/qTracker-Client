#include "loginHandler.h"

LoginHandler::LoginHandler(QObject* parent) : QObject(parent)
{

}

QString LoginHandler::resetPassword(QString email)
{
    QString password = QUuid::createUuid().toString(QUuid::Id128).left(12);
    qDebug() << email;
    QSqlQuery q = db->query();
    QString hashedPassword = db->hash(password);
    qDebug() << hashedPassword;
    q.prepare("UPDATE user SET password = :password WHERE email = :email");
    q.bindValue(":email", email);
    q.bindValue(":password", hashedPassword);
    if (q.exec())
    {
        int rowsAffected = q.numRowsAffected();
        if(rowsAffected > 0)
        {
            return password;
        }
        else
        {
            qDebug() << "User does not exist!";
            errorMessage = "User does not exist!";
            return NULL;
        }
    }
    else
    {
        errorMessage = "SQL error!";
        return NULL;
    }
}

bool LoginHandler::loginUser(User* user, QString username, QString password)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT id, password, privilege FROM user WHERE username = :username");
    q.bindValue(":username", username);
    if (q.exec())
    {
       QString hashedPassword = db->hash(password);
       q.next();
       QString dbHashedPassword = q.value(1).toString();
       int userPrivilege = q.value(2).toInt();
       if(hashedPassword == dbHashedPassword)
       {
           if (userPrivilege > -1)
           {
               qDebug() << "User logged in!";
               if(fillUser(user, username))
               {
                   return true;
               }
               else
               {
                   errorMessage = "An unknown error has occured";
               }
           }
           else
           {
               errorMessage = "This account has been banned";
           }
       }
       else
       {
           qDebug() << "Password did not match!";
           errorMessage = "Password did not match!";
       }
    }
    return false;
}

bool LoginHandler::fillUser(User *user, QString username)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT id, download, upload, privilege, email, createdAt, "
              "password, points, torrentPass FROM user WHERE username = :username");
    q.bindValue(":username", username);
    if(q.exec())
    {
        user->setUsername(username);
        q.next();

        user->setId(q.value(0).toInt());

        user->setDownload(q.value(1).toInt());

        user->setUpload(q.value(2).toInt());

        user->setprivilege(q.value(3).toInt());

        user->setEmail(q.value(4).toString());

        user->setDateJoined(q.value(5).toString());

        user->setPassword(q.value(6).toString());

        user->setPoints(q.value(7).toInt());

        user->setTorrentPass(q.value(8).toString());

        return true;
    }
    return false;
}

QString LoginHandler::getErrorMessage()
{
    return errorMessage;
}

bool LoginHandler::logoutUser(User *user)
{
    user->setUsername("");
    user->setId(0);
    user->setDownload(0);
    user->setUpload(0);
    user->setprivilege(0);
    return true;
}

bool LoginHandler::registerUser(User* user, QString username, QString password, QString email, QString key)
{
    if (uniqueEmail(email))
    {
        if (validInviteKey(key, email))
        {
            if (uniqueUsername(username))
            {
                QString hashedPassword = db->hash(password);
                QSqlQuery q = db->query();
                q.prepare("INSERT INTO user (username, password, email, points, torrentPass)"
                          "VALUES (:username, :password, :email, 0, substring(MD5(RAND()),1,32))");
                q.bindValue(":username", username);
                q.bindValue(":password", hashedPassword);
                q.bindValue(":email", email);
                if (q.exec())
                {
                    qDebug() << "User registered";
                    loginUser(user, username, password);
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

bool LoginHandler::uniqueUsername(QString str)
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

bool LoginHandler::uniqueEmail(QString str)
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

bool LoginHandler::validInviteKey(QString key, QString email)
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

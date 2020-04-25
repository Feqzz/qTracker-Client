#include "user.h"

User::User(QObject *parent) : QObject(parent)
{

}

User::User(int id) : id(id)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT username, password, "
              "privilege, email, createdAt, "
              "torrentPass FROM user WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec())
    {
        q.next();
        username = q.value(0).toString();
        password = q.value(1).toString();
        privilege = q.value(2).toInt();
        email = q.value(3).toString();
        dateJoined = q.value(4).toString();
        torrentPass = q.value(5).toString();
    }
    else
    {
        qDebug() << "Failed to get user data for id: " << id;
    }
}

bool User::refreshUserData()
{
    QSqlQuery q = db->query();
    q.prepare("SELECT points FROM user WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec())
    {
        q.first();
        points = q.value(0).toInt();
    }
    else
    {
        qDebug() << "Failed to retrieve points for user";
        return false;
    }

    q.prepare
    (
        "SELECT "
                "IFNULL(SUM(totalDownloaded), 0) AS 'downloaded', "
                "IFNULL(SUM(totalUploaded), 0) AS 'uploaded' "
        "FROM "
                "userTorrentTotals "
        "WHERE "
                "userId = :id"
    );
    q.bindValue(":id", id);
    if (q.exec())
    {
        q.next();
        download = q.value(0).toULongLong();
        upload = q.value(1).toULongLong();
    }
    else
    {
        qDebug() << "Failed to retrieve total uploaded and downloaded for user";
        return false;
    }
    return true;
}



double User::getRatio()
{
    return double(upload/1000)/double(download/1000);
}

int User::getUpload()
{
    return upload;
}

int User::getDownload()
{
    return download;
}

QString User::getUsername()
{
    return username;
}

int User::getPrivilege()
{
    return privilege;
}

QString User::getEmail()
{
    return email;
}

QString User::getDateJoined()
{
    return dateJoined;
}

int User::getInvitedCount()
{
    QSqlQuery q = db->query();
    q.prepare("SELECT * FROM invite WHERE sender = :sender");
    q.bindValue(":sender", id);
    int count = 0;
    if (q.exec())
    {
        while (q.next())
        {
            count++;
        }
    }
    else
    {
        qDebug() << "Failed to execute getInvitedCount query";
    }
    return count;
}

int User::getId()
{
    return id;
}

int User::getPoints()
{
    return points;
}

QString User::getTorrentPass()
{
    return torrentPass;
}

bool User::changePassword(QString oldPassword, QString newPassword)
{
    if (db->hash(oldPassword) == password)
    {
        db->updatePassword(id, newPassword);
        password = db->hash(newPassword);
        return true;
    }
    else
    {
        qDebug() << "Old password did not match current password";
        return false;
    }
}

void User::setUsername(QString _username)
{
    username = _username;
}
void User::setDownload(uint64_t _download)
{
    download = _download;
}
void User::setUpload(uint64_t _upload)
{
    upload = _upload;
}
void User::setprivilege(int _privilege)
{
    privilege = _privilege;
}
void User::setId(int _id)
{
    id = _id;
}

void User::setEmail(QString _email)
{
    email = _email;
}

void User::setDateJoined(QString _dateJoined)
{
    dateJoined = _dateJoined;
}

void User::setPassword(QString _password)
{
    password = _password;
}

void User::setPoints(double _points)
{
    points = _points;
}

void User::setTorrentPass(QString _torrentPass)
{
    torrentPass = _torrentPass;
}

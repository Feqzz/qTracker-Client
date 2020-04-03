#include "user.h"

User::User(QObject *parent) : QObject(parent)
{

}

User::User(int id) : id(id)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT username, password, upload,"
              " download, privilege, email, createdAt, "
              "torrentPass FROM user WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec())
    {
        q.next();
        username = q.value(0).toString();
        password = q.value(1).toString();
        upload = q.value(2).toUInt();
        download = q.value(3).toUInt();
        privilege = q.value(4).toInt();
        email = q.value(5).toString();
        dateJoined = q.value(6).toString();
        torrentPass = q.value(7).toString();
    }
    else
    {
        qDebug() << "Failed to get user data for id: " << id;
    }
}

bool User::updateUsersData()
{
    QSqlQuery q = db->query();
    q.prepare("UPDATE user AS u "
              "SET "
              "download = (SELECT IFNULL(SUM(totalDownloaded), 0) FROM userTorrentTotals WHERE userId = u.id), "
              "upload = (SELECT IFNULL(SUM(totalUploaded), 0) FROM userTorrentTotals WHERE userId = u.id)");
   if (q.exec())
   {
       return refreshUserData();
   }
   else
   {
       qDebug() << "Failed to update download/upload for all users";
       return false;
   }
}

bool User::refreshUserData()
{
    QSqlQuery q = db->query();
    q.prepare("SELECT download, upload, points FROM user WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec())
    {
        q.next();
        download = q.value(0).toULongLong();
        upload = q.value(1).toULongLong();
        points = q.value(2).toInt();
        return true;
    }
    else
    {
        qDebug() << "Failed to retrieve updated download/upload for user";
        return false;
    }
}



double User::getRatio()
{
    return double(upload/1000)/double(download/1000);
}

int User::getUpload()
{
    return upload/1000000;
}

int User::getDownload()
{
    return download/1000000;
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

void User::setPoints(int _points)
{
    points = _points;
}

void User::setTorrentPass(QString _torrentPass)
{
    torrentPass = _torrentPass;
}

#include "user.h"

User::User(QObject *parent) : QObject(parent)
{

}

User::User(int id) : id(id)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT username, password, upload,"
              " download, privilege, email, createdAt FROM user WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec())
    {
        q.next();
        username = q.value(0).toString();
        password = q.value(1).toString();
        upload = q.value(2).toDouble();
        download = q.value(3).toDouble();
        privilege = q.value(4).toInt();
        email = q.value(5).toString();
        dateJoined = q.value(6).toString();
    }
    else
    {
        qDebug() << "Failed to get user data for id: " << id;
    }
}

double User::getRatio()
{
    return upload/download;
}

double User::getUpload()
{
    return upload;
}

double User::getDownload()
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
void User::setDownload(int _download)
{
    download = _download;
}
void User::setUpload(int _upload)
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

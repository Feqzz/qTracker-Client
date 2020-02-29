#include "user.h"

User::User(int id) : id(id)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT username, password, upload,"
              " download, privilege FROM user WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec())
    {
        username = q.value(0).toString();
        password = q.value(1).toString();
        upload = q.value(2).toDouble();
        download = q.value(3).toDouble();
        privilege = q.value(4).toInt();
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

void User::changePassword(QString oldPassword, QString newPassword)
{
    if (oldPassword == password)
    {
        db->updatePassword(id, newPassword);
    }
    else
    {
        qDebug() << "Old password did not match current password";
    }
}

int User::getId()
{
    return id;
}

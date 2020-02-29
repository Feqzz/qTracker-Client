#include "invite.h"

Invite::Invite(QObject *parent) : QObject(parent)
{

}

QString Invite::generateKey(QString email, int id)
{
    if(id)
    {
        QString key = QUuid::createUuid().toString(QUuid::Id128).left(12);
        qDebug() << "Key: " << key;

        QSqlQuery q = db->query();
        q.prepare("INSERT INTO invite (recipientEmail, sender, InviteKey, expDate) "
                  "VALUES (:email, :sender, :key, DATE_ADD( NOW(), INTERVAL 48 HOUR ))");
        q.bindValue(":email", email);
        q.bindValue(":sender", id);
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

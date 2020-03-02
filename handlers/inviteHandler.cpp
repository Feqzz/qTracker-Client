#include "inviteHandler.h"

InviteHandler::InviteHandler(QObject* parent): QObject(parent)
{

}

QString InviteHandler::inviteUser(int inviterId, QString inviteeEmail)
{
    QString key = QUuid::createUuid().toString(QUuid::Id128).left(12);
    //qDebug() << "Key: " << key;
    //qDebug() << "inviteUser id: " << inviterId;
    QSqlQuery q = db->query();
    q.prepare("INSERT INTO invite (recipientEmail, sender, InviteKey, expDate) "
              "VALUES (:email, :sender, :key, DATE_ADD( NOW(), INTERVAL 48 HOUR ))");
    q.bindValue(":email", inviteeEmail);
    q.bindValue(":sender", inviterId);
    q.bindValue(":key", key);
    if (q.exec())
    {
        return key;
    }
    else
    {
        errorMessage = "Email already in use.";
        return NULL;
    }
}

QString InviteHandler::getErrorMessage()
{
    return errorMessage;
}


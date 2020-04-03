#include "inviteHandler.h"

InviteHandler::InviteHandler(QObject* parent): QObject(parent)
{

}

bool InviteHandler::inviteUser(int inviterId, QString inviteeEmail, int points)
{
    QString key = QUuid::createUuid().toString(QUuid::Id128).left(12);
    qDebug() << "Key: " << key;
    qDebug() << "inviteUser id: " << inviterId;
    QSqlQuery q = db->query();
    q.prepare("INSERT INTO invite (recipientEmail, sender, InviteKey, expDate) "
              "VALUES (:email, :sender, :key, DATE_ADD( NOW(), INTERVAL 48 HOUR ))");
    q.bindValue(":email", inviteeEmail);
    q.bindValue(":sender", inviterId);
    q.bindValue(":key", key);
    if (q.exec())
    {
        return subtractPoints(inviterId, points);
    }
    else
    {
        errorMessage = "Email already in use.";
        return false;
    }
}

QString InviteHandler::getErrorMessage()
{
    return errorMessage;
}

bool InviteHandler::subtractPoints(int inviterId, int points)
{
    QSqlQuery q = db->query();
    q.prepare("UPDATE user SET points = (points - :points) WHERE id = :userId");
    q.bindValue(":userId", inviterId);
    q.bindValue(":points", points);
    if (q.exec())
    {
        return true;
    }
    else
    {
        qDebug() << "Failed to subtract user points when inviting a user";
        return false;
    }
}


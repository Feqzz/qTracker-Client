#include "adminHandler.h"

AdminHandler::AdminHandler(QObject* parent): QObject(parent)
{

}

bool AdminHandler::changeUserPrivilege(int userId, int priv)
{
    QSqlQuery q = db->query();
    q.prepare("UPDATE user SET privilege = :priv WHERE id = :id");
    q.bindValue(":priv", priv);
    q.bindValue(":id", userId);
    if (q.exec())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool AdminHandler::removeUser(int userId)
{
    QSqlQuery q = db->query();
    q.prepare("DELETE FROM user WHERE id = :id");
    q.bindValue(":id", userId);
    if (q.exec())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool AdminHandler::removeTorrent(int torrentId)
{
    return false;
}

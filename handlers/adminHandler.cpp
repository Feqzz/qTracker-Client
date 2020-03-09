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

QVariantMap AdminHandler::getUsersByName(QString string)
{
    QMap<QString, QVariant> map;
    QSqlQuery q = db->query();
    string = "%"+string+"%";
    if (string.isEmpty())
    {
        q.prepare("SELECT id, username, privilege FROM user");
    }
    else
    {
        q.prepare("SELECT id, username, privilege FROM user WHERE username LIKE :string");
        q.bindValue(":string", string);
    }

    if(q.exec()&&q.size()>0)
    {
        while (q.next()) {
            QVariantList values;
            values <<q.value(1).toString()<<q.value(2).toString();
            map[q.value(0).toString()] = values;
        }
    }
    return map;
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

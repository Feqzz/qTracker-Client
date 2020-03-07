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

std::vector<QString> AdminHandler::getUsersByName(QString string)
{
    std::vector<QString> names;
    QSqlQuery q = db->query();
    string = "%"+string+"%";
    q.prepare("SELECT username FROM user WHERE username LIKE :string");
    q.bindValue(":string", string);

    if(q.exec()&&q.size()>0)
    {
        while (q.next()) {
            QString name = q.value(0).toString();
            names.push_back(name);
        }
    }
    return names;
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

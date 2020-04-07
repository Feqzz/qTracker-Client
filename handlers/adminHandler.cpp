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
    return q.exec() ? true : false;
}

bool AdminHandler::changeLeechingPrivilege(int userId, bool canLeech)
{
    QSqlQuery q = db->query();
    q.prepare("UPDATE user SET canLeech = :canLeech WHERE id = :id");
    q.bindValue(":canLeech", canLeech);
    q.bindValue(":id", userId);
    return q.exec() ? true : false;
}

QVariantMap AdminHandler::getUsersByName(QString string)
{
    QMap<QString, QVariant> map;
    QSqlQuery q = db->query();
    string = "%"+string+"%";
    if (string.isEmpty())
    {
        q.prepare("SELECT id, username, privilege, canLeech FROM user");
    }
    else
    {
        q.prepare("SELECT id, username, privilege, canLeech FROM user WHERE username LIKE :string");
        q.bindValue(":string", string);
    }

    if(q.exec() && q.size() > 0)
    {
        while (q.next()) {
            QVariantList values;
            values << q.value(1).toString() << q.value(2).toString() << q.value(3).toString();
            map[q.value(0).toString()] = values;
        }
    }
    return map;
}

QVariantMap AdminHandler::getTorrentsByTitle(QString string)
{
    QMap<QString, QVariant> map;
    QSqlQuery q = db->query();
    string = "%"+string+"%";
    if (string.isEmpty())
    {
        q.prepare
                (
                    "SELECT "
                    "t.id, "
                    "title, "
                    "infoHash, "
                    "completed, "
                    "seeders, "
                    "leechers, "
                    "uploadDate, "
                    "username "
                    "FROM "
                    "torrent AS t, "
                    "user AS u "
                    "WHERE "
                    "t.uploader = u.id"
                    );
    }
    else
    {
        q.prepare
                (
                    "SELECT "
                    "t.id, "
                    "title, "
                    "infoHash, "
                    "completed, "
                    "seeders, "
                    "leechers, "
                    "uploadDate, "
                    "username "
                    "FROM "
                    "torrent AS t, "
                    "user AS u "
                    "WHERE "
                    "t.uploader = u.id AND "
                    "t.title LIKE :title"
                    );
        q.bindValue(":title", string);
    }
    if(q.exec() && q.size() > 0)
    {
        while (q.next()) {
            QVariantList values;
            values << q.value(1).toString() << q.value(2).toString() << q.value(3).toInt()
                   << q.value(4).toInt() << q.value(5).toInt() << q.value(6).toString()
                   << q.value(7).toString();
            map[q.value(0).toString()] = values;
        }
    }
    else
    {
        qDebug() << "Failed AdminHandler::getTorrentsByTitle query";
    }
    return map;
}

bool AdminHandler::removeUser(int userId)
{
    QSqlQuery q = db->query();
    q.prepare("DELETE FROM user WHERE id = :id");
    q.bindValue(":id", userId);
    return q.exec() ? true : false;
}

bool AdminHandler::removeTorrent(int torrentId)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT id FROM torrentFiles WHERE torrentId = :id");
    q.bindValue(":id", torrentId);
    if(q.exec()&&q.size()>0)
    {
        db->startTransaction();
        while (q.next()) {
            int torrentFilesId = q.value(0).toInt();
            q.prepare("DELETE FROM torrentFilePaths WHERE torrentFilesId = :id");
            q.bindValue(":id", torrentFilesId);
            bool torrentFilePathDeleteSuccess = q.exec();
            if(!torrentFilePathDeleteSuccess)
            {
                qDebug() << q.lastError();
                db->rollBack();
                return false;
            }
        }
        q.prepare("DELETE FROM torrentFiles WHERE torrentId = :id");
        q.bindValue(":id", torrentId);
        bool torrentFileDeleteSuccess = q.exec();
        if(!torrentFileDeleteSuccess)
        {
            qDebug() << q.lastError();
            db->rollBack();
            return false;
        }
        q.prepare("DELETE FROM torrent WHERE id = :id");
        q.bindValue(":id", torrentId);
        bool torrentDeleteSuccess = q.exec();
        if(!torrentDeleteSuccess)
        {
            qDebug() << q.lastError();
            db->rollBack();
            return false;
        }


    }
    else
    {
        qDebug() << q.lastError();
        //db->rollBack();
        return false;
    }
    db->commit();
    return true;
}

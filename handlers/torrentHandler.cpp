#include "torrentHandler.h"

TorrentHandler::TorrentHandler(QObject *parent) : QObject(parent)
{

}

QVariantMap TorrentHandler::getTorrentsByName(QString string)
{
    QMap<QString, QVariant> map;
    QSqlQuery q = db->query();
    string = "%" + string + "%";
    if (string.isEmpty())
    {
        q.prepare
        (
            "SELECT "
                    "title, "
                    "username, "
                    "leechers, "
                    "seeders, "
                    "completed, "
                    "uploadDate "
            "FROM "
                    "torrent, "
                    "user "
            "WHERE "
                    "uploader = user.id "
            "ORDER BY completed"
        );
    }
    else
    {
        q.prepare
        (
            "SELECT "
                    "title, "
                    "username, "
                    "leechers, "
                    "seeders, "
                    "completed, "
                    "uploadDate "
            "FROM "
                    "torrent, "
                    "user "
            "WHERE "
                    "uploader = user.id AND "
                    "title LIKE :string "
            "ORDER BY completed"
        );
        q.bindValue(":string", string);
    }

    if(q.exec() && q.size() > 0)
    {
        while (q.next()) {
            QVariantList values;
            values << q.value(1).toString() << q.value(2).toString() <<
                      q.value(3).toString() << q.value(4).toString();
            map[q.value(0).toString()] = values;
            qDebug() << q.value(4).toString();
        }
    }
    else
    {
        qDebug() << "Failed query";
    }
    return map;
}

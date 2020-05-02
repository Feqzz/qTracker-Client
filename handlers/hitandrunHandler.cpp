#include "hitandrunHandler.h"

HitAndRunHandler::HitAndRunHandler(QObject *parent) : QObject(parent)
{

}

int HitAndRunHandler::getNumberOfHnRs(int userId)
{
    QSqlQuery q = db->query();
    q.prepare
    (
        "SELECT "
            "COUNT(*) "
        "FROM "
            "clientTorrents AS ct, "
            "client AS c, "
            "ipAddress AS ip, "
            "user AS u "
        "WHERE "
            "ct.clientId = c.id AND "
            "c.ipaId = ip.id AND "
            "ip.userId = :userId AND "
            "TIMESTAMPDIFF(MINUTE, timeCreated, NOW()) > :maxTime AND "
            "timeActive < :minTime "
    );
    q.bindValue(":userId", userId);
    q.bindValue(":maxTime", maxTime);
    q.bindValue(":minTime", minTime);
    if (q.exec())
    {
        q.first();
        return q.value(0).toInt();
    }
    else
    {
        qDebug() << "Failed HitAndRunHandler::getNumberOfHnRs query";
        return 0;
    }
}

QVariantMap HitAndRunHandler::getTorrents(int userId)
{
    QMap<QString, QVariant> map;
    QSqlQuery q = db->query();
    q.prepare
    (
        "SELECT "
                "t.title, "
                "ct.uploaded, "
                "ct.downloaded, "
                "(ct.uploaded/ct.downloaded) AS ratio, "
                "(SELECT IFNULL(SUM(isActive), 0) FROM clientTorrents AS ct WHERE ct.torrentId = t.id "
                "AND (TIMESTAMPDIFF(MINUTE, ct.lastActivity, NOW()) < 60)) AS 'seeders', "
                "(SELECT IFNULL(SUM(isActive), 0) FROM clientTorrents AS ct WHERE ct.torrentId = t.id "
                "AND ct.left > 0) AS 'leechers', "
                "ct.lastActivity, "
                "IF(ct.isActive AND (TIMESTAMPDIFF(MINUTE, ct.lastActivity, NOW()) < 60), 1, 0), "
                "(:minTime - ct.timeActive) AS seedTimeLeft, "
                "ct.timeActive, "
                "IF(TIMESTAMPDIFF(MINUTE, timeCreated, NOW()) > :maxTime && "
                "timeActive < :minTime, 1, 0) AS HnR "
        "FROM "
                "torrent AS t, "
                "clientTorrents AS ct, "
                "client AS c, "
                "ipAddress AS ip, "
                "user AS u "
        "WHERE "
                "t.id = ct.torrentId AND "
                "ct.clientId = c.id AND "
                "c.ipaId = ip.id AND "
                "ip.userId = :userId "
                "ct.timeActive < 2880"
    );
    q.bindValue(":minTime", minTime);
    q.bindValue(":maxTime", maxTime);
    q.bindValue(":userId", userId);

    if(q.exec() && q.size() > 0)
    {
        while (q.next())
        {
            QVariantList values;
            values << q.value(1).toULongLong() << q.value(2).toULongLong() <<
                      q.value(3).toDouble() << q.value(4).toInt() <<
                      q.value(5).toInt() << q.value(6).toString() <<
                      q.value(7).toBool() << q.value(8).toInt() <<
                      q.value(9).toInt() << q.value(10).toBool();
            map[q.value(0).toString()] = values;
        }
    }
    else
    {
        qDebug() << "Failed HitAndRunHandler::getTorrents query";
    }
    return map;
}

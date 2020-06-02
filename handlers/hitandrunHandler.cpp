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
            "userTorrentTotals AS utt, "
            "clientTorrents AS ct, "
            "client AS c, "
            "ipAddress as ip "
        "WHERE "
            "utt.userId = :userId "
            "AND utt.timeActive < :minTime "
            "AND ct.isActive = 0 "
            "AND ct.torrentId = utt.torrentID "
            "AND ct.clientId = c.id "
            "AND c.ipaId = ip.Id "
            "AND ip.userId = utt.userId"
    );
    q.bindValue(":userId", userId);
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
                "utt.totalUploaded, "
                "utt.totalDownloaded, "
                "(utt.totalUploaded/utt.totalDownloaded) AS ratio, "
                "(SELECT IFNULL(SUM(isActive), 0) FROM clientTorrents AS ct WHERE ct.torrentId = t.id "
                "AND (TIMESTAMPDIFF(MINUTE, ct.lastActivity, NOW()) < 60) AND ct.downloaded > 0) AS 'seeders', "
                "(SELECT IFNULL(SUM(isActive), 0) FROM clientTorrents AS ct WHERE ct.torrentId = t.id "
                "AND ct.left > 0 AND (TIMESTAMPDIFF(MINUTE, ct.lastActivity, NOW()) < 60)) AS 'leechers', "
                "ct.lastActivity, "
                "IF(ct.isActive AND (TIMESTAMPDIFF(MINUTE, ct.lastActivity, NOW()) < 60), 1, 0), "
                "(:minTime - utt.timeActive) AS seedTimeLeft, "
                "utt.timeActive, "
                "IF(!ct.isActive AND utt.timeActive < :minTime, 1, 0) AS HnR "
        "FROM "
                "torrent AS t, "
                "clientTorrents AS ct, "
                "client AS c, "
                "ipAddress AS ip, "
                "user AS u, "
                "userTorrentTotals AS utt "
        "WHERE "
                "t.id = ct.torrentId "
                "AND ct.clientId = c.id "
                "AND c.ipaId = ip.id "
                "AND ip.userId = :userId "
                "AND utt.timeActive < :minTime "
                "AND ip.userId = utt.userId "
                "AND utt.torrentId = ct.torrentId"
    );
    q.bindValue(":minTime", minTime);
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

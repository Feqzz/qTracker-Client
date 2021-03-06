#include "torrentHandler.h"

TorrentHandler::TorrentHandler(QObject *parent) : QObject(parent)
{

}

bool TorrentHandler::downloadFile(int torrentId, QString torrentPass, QString url)
{
    QString title;
    QString encoding;
    QString comment;
    QString createdByClient;
    qlonglong createdtime;
    qlonglong pieceLength;
    QByteArray pieces;
    int privateTorrent;

    QString torrentQuery = "SELECT name,encoding,comment,createdByClient,"
                           "UNIX_TIMESTAMP(createdDate) AS createdtime,"
                           "pieceLength,piece,"
                           "IF(ISNULL(private),2,private) AS private FROM torrent WHERE id = ?;";
    QSqlQuery q = db->query();
    q.prepare(torrentQuery);
    q.bindValue(0, torrentId);
    if (q.exec())
    {
        if(q.next())
        {
            title = q.value(0).toString();
            encoding = q.value(1).toString();
            comment = q.value(2).toString();
            createdByClient = q.value(3).toString();
            createdtime = q.value(4).toLongLong();
            pieceLength = q.value(5).toLongLong();
            pieces = q.value(6).toByteArray();
            privateTorrent = q.value(7).toInt();
            std::vector<TorrentHandler::FileStruct> t = getTorrentFiles(torrentId);
            std::map<std::string, bencode::data> all;
            QString announceURL = "http://www."+trackerHostName+":"+trackerPort+"/"+torrentPass+"/announce";
            all.insert(std::pair<std::string,bencode::data>("announce",announceURL.toStdString()));
            if(encoding.length()>0)
            {
                all.insert(std::pair<std::string,bencode::data>("encoding",encoding.toStdString()));
            }
            if(comment.length()>0)
            {
                all.insert(std::pair<std::string,bencode::data>("comment",comment.toStdString()));
            }
            if(createdByClient.length()>0)
            {
                all.insert(std::pair<std::string,bencode::data>("created by",createdByClient.toStdString()));
            }
            if(createdtime>0)
            {
                all.insert(std::pair<std::string,bencode::data>("creation date",createdtime));
            }

            std::map<std::string, bencode::data> info;
            info.insert(std::pair<std::string,bencode::data>("piece length",pieceLength));
            info.insert(std::pair<std::string,bencode::data>("pieces",pieces.toStdString()));
            info.insert(std::pair<std::string,bencode::data>("name",title.toStdString()));
            if(privateTorrent <= 1)
            {
                info.insert(std::pair<std::string,bencode::data>("private",privateTorrent==1));
            }

            if(t.size()>1)
            {
                std::vector<bencode::data> fileList;
                for(size_t y=0;y<t.size();y++)
                {
                    std::map<std::string, bencode::data> fileDict;
                    fileDict.insert(std::pair<std::string,bencode::data>("length",t.at(y).length));
                    std::vector<QString> paths = t.at(y).path;
                    std::vector<bencode::data> pathList;
                    for(size_t x=0;x<paths.size();x++)
                    {
                        pathList.push_back(paths.at(x).toStdString());
                    }
                    fileDict.insert(std::pair<std::string,bencode::data>("path",pathList));
                    fileList.push_back(fileDict);
                }
                info.insert(std::pair<std::string,bencode::data>("files",fileList));
            }
            else
            {
                info.insert(std::pair<std::string,bencode::data>("length",t.at(0).length));
            }
            all.insert(std::pair<std::string,bencode::data>("info",info));
            std::ofstream ofs;
            ofs.open(url.mid(7).toUtf8());
            bencode::encode(ofs, all);
            ofs.close();
            return true;
        }
    }
    else
    {
        qDebug() << q.lastError();
    }
    return false;

}

std::vector<TorrentHandler::FileStruct> TorrentHandler::getTorrentFiles(int torrentId)
{
    std::vector<TorrentHandler::FileStruct> fileVector;
    QString torrentFileQuery = "SELECT id,length FROM torrentFiles WHERE torrentId = ?;";
    QSqlQuery q = db->query();
    q.prepare(torrentFileQuery);
    q.bindValue(0, torrentId);
    if (q.exec())
    {
        while(q.next())
        {
            FileStruct f;
            f.length = q.value(1).toLongLong();
            int fileId = q.value(0).toInt();
            QString torrentFilePathQuery = "SELECT path FROM torrentFilePaths WHERE torrentFilesId = ?;";
            QSqlQuery q2 = db->query();
            q2.prepare(torrentFilePathQuery);
            q2.bindValue(0, fileId);
            if (q2.exec())
            {
                while(q2.next())
                {
                    f.path.push_back(q2.value(0).toString());
                }
            }
            else
            {
                qDebug() << q2.lastError();
            }
            fileVector.push_back(f);

        }
    }
    else
    {
        qDebug() << q.lastError();
    }
    return fileVector;

}
QVariantList TorrentHandler::getTorrentsByName(QString string, int userId, int orderType, bool isAccending, QString orderByUsername)
{
    QList<QVariant> list;
    QSqlQuery q = db->query();
    string = "%" + string + "%";
    QString queryString = "SELECT "
                          "title, "
                          "username, "
                          "(SELECT IFNULL(SUM(isActive), 0) FROM clientTorrents AS ct WHERE ct.torrentId = torrent.id "
                          "AND ct.left > 0 AND (TIMESTAMPDIFF(MINUTE, ct.lastActivity, NOW()) < 60)) AS 'leechers', "
                          "(SELECT IFNULL(SUM(isActive), 0) FROM clientTorrents AS ct WHERE ct.torrentId = torrent.id "
                          "AND (TIMESTAMPDIFF(MINUTE, ct.lastActivity, NOW()) < 60) AND (ct.left = 0)) AS 'seeders', "
                          "(SELECT IFNULL(SUM(ct.completed), 0) FROM clientTorrents AS ct WHERE ct.torrentId = torrent.id) "
                          "AS 'completed', "
                          "uploadDate, "
                          "torrent.id, "
                          "IF((SELECT EXISTS(SELECT * FROM clientTorrents AS ct, client AS c, "
                          "ipAddress AS ip WHERE ct.clientId = c.id AND c.ipaID = ip.id AND ip.userId = :userId "
                          "AND ct.torrentId = torrent.id)), 1, 0) AS 'downloaded', "
                          "IF((SELECT EXISTS(SELECT * FROM clientTorrents AS ct, "
                          "client AS c, ipAddress AS ip WHERE ct.clientId = c.id AND c.ipaID = ip.id AND ip.userId = :userId "
                          "AND ct.torrentId = torrent.id AND ct.isActive = 1 "
                          "AND (TIMESTAMPDIFF(MINUTE, ct.lastActivity, NOW()) < 60))), 1, 0) AS 'seeding', "
                          "(SELECT SUM(length) FROM torrentFiles WHERE torrentId = torrent.id) AS 'size' "
                  "FROM "
                          "torrent, "
                          "user "
                  "WHERE "
                          "uploader = user.id AND "
                          "title LIKE :string ";
    if (!orderByUsername.isNull())
    {
        queryString += "AND username = :orderByUsername ";
    }
    queryString += "ORDER BY ";
    QString arrangedOrder = isAccending ? "" : " DESC";
    switch (orderType)
    {
        case 0: queryString += "completed";
                break;
        case 1: queryString += "seeders";
                break;
        case 2: queryString += "leechers";
                break;
        case 3: queryString += "size";
                break;
        case 4: queryString += "title";
                break;
        default: queryString += "uploadDate";
                break;
    }
    queryString += arrangedOrder;
    q.prepare(queryString);
    q.bindValue(":string", string);
    q.bindValue(":userId", userId);
    if (!orderByUsername.isNull()) q.bindValue(":orderByUsername", orderByUsername);
    if(q.exec() && q.size() > 0)
    {
        int i = 0;
        while (q.next()) {
            list.append(i);
            QList<QVariant> values;
            values << q.value(0).toString() << q.value(1).toString() <<
                      q.value(2).toInt() <<
                      q.value(3).toInt() << q.value(4).toInt() <<
                      q.value(5).toString() << q.value(6).toInt() <<
                      q.value(7).toBool() << q.value(8).toBool() <<
                      q.value(9).toULongLong();
            list[i] = values;
            i++;
        }
    }
    else
    {
        qDebug() << "Failed get Torrents query";
    }
    return list;
}

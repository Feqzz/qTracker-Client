#include "torrentHandler.h"

TorrentHandler::TorrentHandler(QObject *parent) : QObject(parent)
{

}
void TorrentHandler::downloadFile(int torrentId, QString torrentPass)
{
    QString title;
    QString encoding;
    QString comment;
    QString createdByClient;
    qlonglong createdtime;
    qlonglong pieceLength;
    QByteArray pieces;
    bool privateTorrent;

    QString torrentQuery = "SELECT title,encoding,comment,createdByClient,UNIX_TIMESTAMP(createdDate) as createdtime,pieceLength,piece,private FROM torrent WHERE id = ?;";
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
            privateTorrent = q.value(7).toBool();
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
            if(privateTorrent)
            {
                info.insert(std::pair<std::string,bencode::data>("private",privateTorrent));
            }

            //bencode::dict all;
            //Multiple files mode
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
                info.insert(std::pair<std::string,bencode::data>("name",t.at(0).path.at(0).toStdString()));
            }
            all.insert(std::pair<std::string,bencode::data>("info",info));
            std::ofstream ofs;
            ofs.open((title+".torrent").toUtf8());
            bencode::encode(ofs, all);
            ofs.close();
        }
    }
    else
    {
        qDebug() << q.lastError();
    }


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
                    "torrent.id, "
                    "title, "
                    "username, "
                    "leechers, "
                    "seeders, "
                    "completed, "
                    "uploadDate, "
                    "torrent.id "
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
                    "uploadDate, "
                    "torrent.id "
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
                      q.value(3).toString() << q.value(4).toString() <<
                      q.value(5).toString() << q.value(6).toString();
            map[q.value(0).toString()] = values;
        }
    }
    else
    {
        qDebug() << "Failed get Torrents query";
    }
    return map;
}

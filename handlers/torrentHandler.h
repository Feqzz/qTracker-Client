#ifndef TORRENTHANDLER_H
#define TORRENTHANDLER_H

#include "core/handler.h"
#include "core/bencode.hpp"
#include <fstream>
#include "config.cpp"
#include <QObject>


class TorrentHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    explicit TorrentHandler(QObject *parent = nullptr);
    Q_INVOKABLE QVariantMap getTorrentsByName(QString); //Empty String for the top 10 torrents
    Q_INVOKABLE void downloadFile(int,QString);
private:
    struct FileStruct
    {
        std::vector<QString> path;
        qlonglong length;
    };
    std::vector<FileStruct> getTorrentFiles(int);
};

#endif // TORRENTHANDLER_H

#ifndef TORRENTHANDLER_H
#define TORRENTHANDLER_H

#include "core/handler.h"
#include <QObject>

class TorrentHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    explicit TorrentHandler(QObject *parent = nullptr);
    Q_INVOKABLE QVariantMap getTorrentsByName(QString); //Empty String for the top 10 torrents

};

#endif // TORRENTHANDLER_H

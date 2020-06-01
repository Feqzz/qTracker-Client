#ifndef ADMINHANDLER_H
#define ADMINHANDLER_H

#include <QObject>
#include "core/handler.h"

class AdminHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    explicit AdminHandler(QObject* parent = nullptr);
    Q_INVOKABLE bool changeUserPrivilege(int userId, int priv);
    Q_INVOKABLE bool removeUser(int userId);
    Q_INVOKABLE bool removeTorrent(int torrentId);
    Q_INVOKABLE bool changeLeechingPrivilege(int userId, bool canLeech);
    Q_INVOKABLE QVariantMap getUsersByName(QString); //Empty string for all users
    Q_INVOKABLE QVariantMap getTorrentsByTitle(QString);
    Q_INVOKABLE QVariantMap getIPsByIP(QString);
    Q_INVOKABLE QVariantMap getIPsByUser(QString);
    Q_INVOKABLE bool changeIPPrivilege(int ipId, bool banned);
};

#endif // ADMINHANDLER_H

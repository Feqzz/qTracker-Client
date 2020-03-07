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
    Q_INVOKABLE QVariantMap getUsersByName(QString);
};

#endif // ADMINHANDLER_H

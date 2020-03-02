#ifndef INVITEHANDLER_H
#define INVITEHANDLER_H

#include <QObject>
#include "core/handler.h"

class InviteHandler : public QObject, public Handler
{
public:
    explicit InviteHandler(QObject* parent = nullptr);
    Q_INVOKABLE bool inviteUser(QString inviterName, QString inviteeEmail);
    Q_INVOKABLE QString getErrorMessage();
private:
    QString errorMessage;
};

#endif // INVITEHANDLER_H

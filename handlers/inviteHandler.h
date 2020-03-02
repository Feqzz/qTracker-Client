#ifndef INVITEHANDLER_H
#define INVITEHANDLER_H

#include <QObject>
#include "core/handler.h"

class InviteHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    explicit InviteHandler(QObject* parent = nullptr);
    Q_INVOKABLE QString inviteUser(int inviterId, QString inviteeEmail);
    Q_INVOKABLE QString getErrorMessage();
private:
    QString errorMessage;
};

#endif // INVITEHANDLER_H

#ifndef INVITEHANDLER_H
#define INVITEHANDLER_H

#include <QObject>
#include "core/handler.h"

class InviteHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    explicit InviteHandler(QObject* parent = nullptr);
    Q_INVOKABLE bool inviteUser(int inviterId, QString inviteeEmail, int points);
    Q_INVOKABLE QString getErrorMessage();
    Q_INVOKABLE bool subtractPoints(int inviterId, int points);
private:
    QString errorMessage;
};

#endif // INVITEHANDLER_H

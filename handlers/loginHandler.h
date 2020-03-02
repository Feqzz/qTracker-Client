#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include <QObject>
#include "core/handler.h"
#include "model/user.h"

class LoginHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    explicit LoginHandler(QObject *parent = nullptr);
    Q_INVOKABLE bool loginUser(User* user, QString username, QString password);
    Q_INVOKABLE bool logoutUser(User* user);
    Q_INVOKABLE bool registerUser(User* user, QString, QString, QString, QString);
    Q_INVOKABLE QString getErrorMessage();

private:
    QString errorMessage;
    bool fillUser(User *user, QString username);

    bool validInviteKey(QString, QString);
    bool uniqueUsername(QString);
    bool uniqueEmail(QString);
};


#endif // LOGINHANDLER_H

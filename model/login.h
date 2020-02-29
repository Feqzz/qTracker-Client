#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include "core/model.h"
#include "model/user.h"

class Login : public QObject, public Model
{
    Q_OBJECT
public:
    explicit Login(QObject *parent = nullptr);
    Login(User*);
    Q_INVOKABLE void test(QString);
    Q_INVOKABLE bool login(QString, QString);
    Q_INVOKABLE bool registerUser(QString, QString, QString, QString);
    //Q_INVOKABLE QString generateKey(QString);
private:
    bool validInviteKey(QString, QString);
    bool uniqueUsername(QString);
    bool uniqueEmail(QString);
    int sessionId;
    User* user;

signals:
};

#endif // LOGIN_H

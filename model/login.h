#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QDebug>
#include <QPoint>
#include <QUuid>
#include "core/model.h"
#include "core/core.h"

class Login : public QObject, public Model, public Core
{
    Q_OBJECT
public:
    explicit Login(QObject *parent = nullptr);
    Q_INVOKABLE void test(QString);
    Q_INVOKABLE bool login(QString, QString);
    Q_INVOKABLE bool registerUser(QString, QString, QString, QString);
    Q_INVOKABLE QString generateKey(QString);
private:
    bool validInviteKey(QString, QString);
    bool uniqueUsername(QString);
    bool uniqueEmail(QString);
    int tempUserId;
signals:
};

#endif // LOGIN_H

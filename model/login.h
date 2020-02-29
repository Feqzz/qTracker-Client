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
    Q_INVOKABLE void login(QString, QString);
    Q_INVOKABLE void register_(QString, QString, QString, QString); //Illegal to name it register
private:
    bool validInviteKey(QString, QString);
    bool uniqueUsername(QString);
    bool uniqueEmail(QString);
    QString generateKey(QString);
    int tempUserId;
signals:
};

#endif // LOGIN_H

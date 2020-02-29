#ifndef INVITE_H
#define INVITE_H

#include <QObject>
#include "core/model.h"

class Invite : public QObject, public Model
{
    Q_OBJECT
public:
    explicit Invite(QObject *parent = nullptr);
    Q_INVOKABLE QString generateKey(QString, int);
signals:
};

#endif // INVITE_H

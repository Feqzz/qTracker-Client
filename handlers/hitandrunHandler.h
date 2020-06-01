#ifndef HITANDRUNHANDLER_H
#define HITANDRUNHANDLER_H

#include <QObject>
#include "core/handler.h"

class HitAndRunHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    explicit HitAndRunHandler(QObject *parent = nullptr);
    Q_INVOKABLE int getNumberOfHnRs(int userId);
    Q_INVOKABLE QVariantMap getTorrents(int userId);
private:
    const int minTime = 2880; //Minimum seeding time required in minutes
    const int maxTime = 20160; //Maximum time in minutes the user is given to seed before registered as HnR
};

#endif // HITANDRUNHANDLER_H

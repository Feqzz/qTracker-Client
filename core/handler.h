#ifndef HANDLER_H
#define HANDLER_H
#include "database.h"
#include <QDebug>
#include <QPoint>
#include <QUuid>

class Handler
{
public:
    Handler();
    ~Handler();
    Database* db;
};

#endif // HANDLER_H

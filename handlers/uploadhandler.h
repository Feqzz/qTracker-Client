#ifndef UPLOADHANDLER_H
#define UPLOADHANDLER_H

#include "core/handler.h"
#include <QObject>

class uploadHandler:public QObject, public Handler
{
    Q_OBJECT
public:
    explicit uploadHandler(QObject *parent = nullptr);
};

#endif // UPLOADHANDLER_H

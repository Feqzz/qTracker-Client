#ifndef TEXTHANDLER_H
#define TEXTHANDLER_H

#include <QObject>
#include "core/handler.h"
#include "model/user.h"

class TextHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    explicit TextHandler(QObject *parent = nullptr);
    Q_INVOKABLE QString getText(int id);
    Q_INVOKABLE bool updateText(int userId, QString text, int id);
};

#endif // TEXTHANDLER_H

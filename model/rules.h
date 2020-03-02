#ifndef RULES_H
#define RULES_H
#include "core/model.h"
#include <QObject>

class Rules : public QObject, public Model
{
    Q_OBJECT
public:
    explicit Rules(QObject *parent = nullptr);
    Q_INVOKABLE QString getText();
    Q_INVOKABLE void updateText(QString);
};

#endif // RULES_H

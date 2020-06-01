#ifndef MODEL_H
#define MODEL_H
#include "database.h"
#include <QDebug>
#include <QPoint>
#include <QUuid>

class Model
{
public:
    Model();
    ~Model();
    Database* db;
};

#endif // MODEL_H

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "app/core/model.h"
#include "app/core/view.h"

class Controller
{
public:
    Controller();
    void setView(QString);
    void setModel(QString);
protected:
    View* view;
    Model* model;
};

#endif // CONTROLLER_H

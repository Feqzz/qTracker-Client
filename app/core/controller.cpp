#include "controller.h"

Controller::Controller()
{

}

void Controller::setView(QString name)
{
    view = new View(name);
}


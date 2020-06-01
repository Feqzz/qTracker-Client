#include "handler.h"

Handler::Handler()
{
    db = new Database();
}

Handler::~Handler()
{
    delete db;
}

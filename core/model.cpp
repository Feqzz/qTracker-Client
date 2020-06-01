#include "model.h"

Model::Model()
{
    //Kanskje fjerne denne? Kent.
    db = new Database();
}

Model::~Model()
{
    delete db;
}

#ifndef CORE_H
#define CORE_H
#include "model/user.h"

class Core
{
public:
    Core();
protected:
    User* sessionUser;
};

#endif // CORE_H

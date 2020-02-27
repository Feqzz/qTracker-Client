#ifndef LOGIN_H
#define LOGIN_H
#include "core/model.h"

class Login : public Model
{
public:
    Login();
    void login(QString, QString);
    void register_(QString, QString); //Illegal to name it register
private:
    bool uniqueUsername(QString);
};

#endif // LOGIN_H

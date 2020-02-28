#ifndef LOGIN_H
#define LOGIN_H
#include "app/core/model.h"

class Login : public Model
{
public:
    Login();
    void login(QString, QString);
    void register_(QString, QString, QString); //Illegal to name it register
private:
    bool uniqueUsername(QString);
    bool uniqueEmail(QString);
};

#endif // LOGIN_H

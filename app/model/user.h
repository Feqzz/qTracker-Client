#ifndef USER_H
#define USER_H
#include "app/core/model.h"

class User : public Model
{
public:
    User(int);
    double getRatio();
    void changePassword(QString, QString);
    int id;
private:
    QString username;
    QString password;
    double download;
    double upload;
    int privilege;
};

#endif // USER_H

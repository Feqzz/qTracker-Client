#ifndef USER_H
#define USER_H
#include "core/model.h"

class User : public Model
{
public:
    User(int);
    double getRatio();
    void changePassword(QString, QString);
private:
    QString username;
    QString password;
    double download;
    double upload;
    int privilege;
    int id;
};

#endif // USER_H

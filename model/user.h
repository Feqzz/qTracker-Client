#ifndef USER_H
#define USER_H
#include "core/model.h"
#include <QObject>

class User : public QObject, public Model
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    User(int);
    Q_INVOKABLE double getRatio();
    void changePassword(QString, QString);
    Q_INVOKABLE int getId();
    Q_INVOKABLE double getDownload();
    Q_INVOKABLE double getUpload();
    Q_INVOKABLE int getPrivilege();
private:
    QString username;
    QString password;
    double download;
    double upload;
    int privilege;
    int id;
signals:
};

#endif // USER_H

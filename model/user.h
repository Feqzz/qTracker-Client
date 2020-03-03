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
    Q_INVOKABLE QString getUsername();
    Q_INVOKABLE int getPrivilege();
    Q_INVOKABLE QString getEmail();
    Q_INVOKABLE QString getDateJoined();
    Q_INVOKABLE int getInvitedCount();
    void setUsername(QString _username);
    void setDownload(int _download);
    void setUpload(int _upload);
    void setprivilege(int _privilege);
    void setId(int _id);
    void setEmail(QString _email);
    void setDateJoined(QString _dateJoined);
private:
    QString username;
    QString password;
    QString email;
    QString dateJoined;
    double download;
    double upload;
    int privilege;
    int id;
signals:
};

#endif // USER_H

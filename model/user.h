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
    Q_INVOKABLE bool changePassword(QString, QString);
    Q_INVOKABLE int getId();
    Q_INVOKABLE int getDownload();
    Q_INVOKABLE int getUpload();
    Q_INVOKABLE QString getUsername();
    Q_INVOKABLE int getPrivilege();
    Q_INVOKABLE QString getEmail();
    Q_INVOKABLE QString getDateJoined();
    Q_INVOKABLE int getInvitedCount();
    Q_INVOKABLE int getPoints();
    Q_INVOKABLE QString getTorrentPass();
    Q_INVOKABLE bool updateUsersData();
    bool refreshUserData();
    void setUsername(QString _username);
    void setDownload(uint64_t _download);
    void setUpload(uint64_t _upload);
    void setprivilege(int _privilege);
    void setId(int _id);
    void setEmail(QString _email);
    void setDateJoined(QString _dateJoined);
    void setPassword(QString _password);
    void setPoints(int _points);
    void setTorrentPass(QString _torrentPass);
private:
    QString username;
    QString password;
    QString email;
    QString dateJoined;
    QString torrentPass;
    uint64_t download;
    uint64_t upload;
    int privilege;
    int id;
    int points;
signals:
};

#endif // USER_H

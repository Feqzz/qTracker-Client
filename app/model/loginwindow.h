#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "app/core/model.h"
#include "app/core/database.h"
#include <QDebug>
#include <QUuid>


QT_BEGIN_NAMESPACE
namespace Ui { class LogInWindow; }
QT_END_NAMESPACE

class LogInWindow : public QMainWindow, public Model
{
    Q_OBJECT
public:
    LogInWindow(QWidget *parent = nullptr);
    ~LogInWindow();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_2_clicked();

private:
    void login(QString, QString);
    void register_(QString, QString, QString, QString); //Illegal to name it register
    bool validInviteKey(QString, QString);
    bool uniqueUsername(QString);
    bool uniqueEmail(QString);
    QString generateKey(QString);
    int tempUserId;
    Ui::LogInWindow *ui;
};
#endif // LOGINWINDOW_H

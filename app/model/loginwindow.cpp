#include "loginwindow.h"
#include "ui_loginwindow.h"


LogInWindow::LogInWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LogInWindow)
{
    ui->setupUi(this);
}

LogInWindow::~LogInWindow()
{
    delete ui;
}

void LogInWindow::on_pushButton_clicked() //login button
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    login(username, password);
}

void LogInWindow::on_pushButton_2_clicked() //Register button
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    register_(username, password);
}

void LogInWindow::login(QString username, QString password)
{
     QSqlQuery q = db->query();
     q.prepare("SELECT id, password FROM user WHERE username = :username");
     q.bindValue(":username", username);
     if (q.exec())
     {
        QString hashedPassword = db->hash(password);

        q.next();
        QString dbHashedPassword = q.value(1).toString();
        if(hashedPassword == dbHashedPassword)
        {
            qDebug() << "User logged in!";
            int id = q.value(0).toInt();
            //Somehow make the User object from the application class :/
            //Redirect user from here.. and use the user id somwhow?
        }
        else
        {
            qDebug() << "Password did not match!";
        }
     }
}

void LogInWindow::register_(QString username, QString password)
{
    if (uniqueUsername(username))
    {
        QString hashedPassword = db->hash(password);
        QSqlQuery q = db->query();
        q.prepare("INSERT INTO user (username, password)"
                  "VALUES (:username, :password)");
        q.bindValue(":username", username);
        q.bindValue(":password", hashedPassword);
        if (q.exec())
        {
            qDebug() << "User registered";
            login(username, password);
        }
    }
    else
    {
        qDebug() << "Username already taken";
    }
}

bool LogInWindow::uniqueUsername(QString str)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT username FROM user WHERE username = :username");
    q.bindValue(":username", str);
    if (q.exec())
    {
        return (q.size() > 0) ? false : true;
    }
    else
    {
        qDebug("Failed to execute query while checking for unique username");
        return false;
    }
}


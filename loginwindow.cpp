#include "loginwindow.h"
#include "ui_loginwindow.h"


LogInWindow::LogInWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LogInWindow), db(new Database())
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
    db->login(username, password);
}

void LogInWindow::on_pushButton_2_clicked() //Register button
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    db->register_(username, password);
}

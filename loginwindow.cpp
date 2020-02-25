#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "database.h"

LogInWindow::LogInWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LogInWindow)
{
    ui->setupUi(this);
}

LogInWindow::~LogInWindow()
{
    delete ui;
}

void LogInWindow::on_pushButton_clicked() //login button
{
    Database* db = new Database();
    db->makeTables();
}

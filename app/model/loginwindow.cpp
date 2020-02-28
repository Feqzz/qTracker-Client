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
            tempUserId = id;
        }
        else
        {
            qDebug() << "Password did not match!";
        }
     }
}

void LogInWindow::register_(QString username, QString password, QString email, QString key)
{
    if (uniqueEmail(email))
    {
        if (validInviteKey(key, email))
        {
            if (uniqueUsername(username))
            {
                QString hashedPassword = db->hash(password);
                QSqlQuery q = db->query();
                q.prepare("INSERT INTO user (username, password, email)"
                          "VALUES (:username, :password, :email)");
                q.bindValue(":username", username);
                q.bindValue(":password", hashedPassword);
                q.bindValue(":email", email);
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
        else
        {
            qDebug() << "Not a valid invite key";
        }
    }
    else
    {
        qDebug() << "Email already in use";
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

bool LogInWindow::uniqueEmail(QString str)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT email FROM user WHERE email = :email");
    q.bindValue(":email", str);
    if (q.exec())
    {
        return (q.size() > 0) ? false : true;
    }
    else
    {
        qDebug("Failed to execute query while checking for unique email");
        return false;
    }
}

bool LogInWindow::validInviteKey(QString key, QString email)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT inviteKey FROM invite WHERE inviteKey = :key AND recipientEmail = :email AND expDate > now()");
    q.bindValue(":key", key);
    q.bindValue(":email", email);
    if (q.exec())
    {
        qDebug() << q.value(0).toString();
        return (key != q.value(0).toString()) ? false : true;
    }
    else
    {
        qDebug("Failed to execute query while checking for vaild Invite");
        return false;
    }
}

void LogInWindow::on_pushButton_6_clicked()
{
    qDebug() << "We got here";
    QString email = ui->lineEdit_8->text();
    QString username = ui->lineEdit_3->text();
    QString password = ui->lineEdit_7->text();
    QString key = ui->lineEdit_9->text();
    register_(username, password, email, key);
}

void LogInWindow::on_pushButton_2_clicked()
{
    QString email = ui->lineEdit_10->text();
    QString key = generateKey(email);
    if (key != NULL)
    {
        qDebug() << "Should be printing";
        ui->label_13->setText(key);
        ui->label_13->update();
    }
}



QString LogInWindow::generateKey(QString email)
{
    if(tempUserId)
    {
        /*QString key;
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < 12; ++i)
        {
            key[i] = alphanum[qrand() % (sizeof(alphanum) - 1)];
        }*/
        QString key = QUuid::createUuid().toString();
        qDebug() << "Key: " << key;


        QSqlQuery q = db->query();
        q.prepare("INSERT INTO invite (recipientEmail, sender, InviteKey, expDate) "
                  "VALUES (:email, :sender, :key, DATE_ADD( NOW(), INTERVAL 48 HOUR ))");
        q.bindValue(":email", email);
        q.bindValue(":sender", tempUserId);
        q.bindValue(":key", key);
        if (q.exec())
        {
            return key;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        qDebug() << "No user is logged in";
        return NULL;
    }
}





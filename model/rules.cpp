#include "rules.h"

Rules::Rules(QObject *parent) : QObject(parent)
{

}

QString Rules::getText()
{
    QSqlQuery q = db->query();
    q.prepare("SELECT text FROM text WHERE page = 'rules'");
    if (q.exec())
    {
        q.next();
        qDebug() << q.value(0).toString();
        return q.value(0).toString();
    }
    else
    {
        qDebug() << "Failed to query getText for the rules page";
        return "it did not work";
    }
}

void Rules::updateText(QString str)
{
    QSqlQuery q = db->query();
    q.prepare("UPDATE text SET text = :text WHERE page = 'rules'");
    q.bindValue(":text", str);
    if (q.exec())
    {
        qDebug() << "Updated text in database for the rules page.";
    }
    else
    {
        qDebug() << "Failed to update the text for the rules page";
    }
}

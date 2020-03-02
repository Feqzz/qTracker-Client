#include "texthandler.h"

TextHandler::TextHandler(QObject* parent) : QObject(parent)
{

}

QString TextHandler::getText(int id)
{
    QSqlQuery q = db->query();
    q.prepare("SELECT text FROM text WHERE id = :id");
    q.bindValue(":id", id);
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

bool TextHandler::updateText(int userId, QString text, int id)
{
    QSqlQuery q = db->query();
    q.prepare("UPDATE text SET text = :text, lastEditedBy = :user WHERE id = :id");
    q.bindValue(":text", text);
    q.bindValue(":user", userId);
    q.bindValue(":id", id);
    if (q.exec())
    {
        qDebug() << "Updated text in database for the rules page.";
        return true;
    }
    else
    {
        qDebug() << "Failed to update the text for the rules page";
        return false;
    }
}

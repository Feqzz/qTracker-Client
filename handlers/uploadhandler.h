#ifndef UPLOADHANDLER_H
#define UPLOADHANDLER_H

#include "core/handler.h"
#include <QObject>
#include "core/bencode.hpp"
#include <fstream>

class UploadHandler:public QObject, public Handler
{
    Q_OBJECT
public:
    explicit UploadHandler(QObject *parent = nullptr);
    Q_INVOKABLE void setUrl(QString);
    Q_INVOKABLE void setTitle(QString);
    Q_INVOKABLE bool uploadFile();
private:
    bool uploadDict(std::map<std::string, bencode::data>);
    QString getInfoHash(std::map<std::string, bencode::data>);
    QByteArray getPieces(bencode::string);
    QString url;
    QString title;
};

#endif // UPLOADHANDLER_H

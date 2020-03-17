#ifndef UPLOADHANDLER_H
#define UPLOADHANDLER_H

#include "core/handler.h"
#include <QObject>
#include "core/bencode.hpp"
#include <fstream>

class UploadHandler:public QObject, public Handler
{
    Q_OBJECT
    struct FileStruct
    {
        std::vector<QString> path;
        int length;
    };
public:
    explicit UploadHandler(QObject *parent = nullptr);
    Q_INVOKABLE void setUrl(QString);
    Q_INVOKABLE void setTitle(QString);
    Q_INVOKABLE void setId(int);
    Q_INVOKABLE bool uploadFile();
private:
    bool uploadDict(std::map<std::string, bencode::data>);
    bool insertTorrentData(std::vector<QVariant>,std::vector<FileStruct>,QString);
    QString getInfoHash(std::map<std::string, bencode::data>);
    QByteArray getPieces(bencode::string);
    QString url;
    QString title;
    int id;
};

#endif // UPLOADHANDLER_H

#ifndef UPLOADHANDLER_H
#define UPLOADHANDLER_H

#include "core/handler.h"
#include <QObject>
#include "core/bencode.hpp"
#include <fstream>

class uploadHandler:public QObject, public Handler
{
    Q_OBJECT
public:
    explicit uploadHandler(QObject *parent = nullptr);
    Q_INVOKABLE bool readFile(QString);
private:
    bool uploadDict(std::map<std::string, bencode::data>);
    QString getInfoHash(std::map<std::string, bencode::data>);
    QByteArray getPieces(bencode::string);
};

#endif // UPLOADHANDLER_H

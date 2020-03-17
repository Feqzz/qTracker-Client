#ifndef TORRENTFILEPARSER_H
#define TORRENTFILEPARSER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QCryptographicHash>
#include "core/bencode.hpp"
#include "core/handler.h"
#include <QTextCodec>
#include <QUrl>
#include <iostream>
#include <fstream>
#include "config.cpp"
#include <map>
#include <string>
#include <locale>
#include <codecvt>
#include <cstdio>
#include <vector>
#include <stdio.h>
#include <QCoreApplication>

class TorrentFileParser : public QObject, public Handler
{

    Q_OBJECT
public:
    explicit TorrentFileParser(QObject *parent = nullptr);
    Q_INVOKABLE void parse(QString);
    //Q_INVOKABLE void readFile(QString);
    //Q_INVOKABLE QByteArray getInfoHash(QString);
    Q_INVOKABLE void getInfoHashFromFile(QString);
    Q_INVOKABLE void createFile(QString);
    void getTorrentFile(int);
    void setTorrentPass(QString);
private:
    struct FileStruct
    {
        std::vector<QString> path;
        qlonglong length;
    };
    QString torrentPass;
    int recursiveParser(std::vector<char>*,int);
    QByteArray infoBytes;
    std::vector<std::string> keys;
    std::vector<FileStruct> getTorrentFiles(int);
};

#endif // TORRENTFILEPARSER_H

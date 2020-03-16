#ifndef TORRENTFILEPARSER_H
#define TORRENTFILEPARSER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QCryptographicHash>
#include "core/bencode.hpp"
#include <QTextCodec>
#include <QUrl>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <stdio.h>
#include <QCoreApplication>

class TorrentFileParser : public QObject
{
    Q_OBJECT
public:
    explicit TorrentFileParser(QObject *parent = nullptr);
    Q_INVOKABLE void parse(QString);
    Q_INVOKABLE void readFile(QString);
    Q_INVOKABLE QByteArray getInfoHash(QString);
    Q_INVOKABLE void getInfoHashFromFile(QString);
private:
    int rec(std::vector<char>*,int);
    QByteArray infoBytes;
    std::vector<std::string> keys;
};

#endif // TORRENTFILEPARSER_H

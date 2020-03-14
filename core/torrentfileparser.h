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
#include <stdio.h>

class TorrentFileParser : public QObject
{
    Q_OBJECT
public:
    explicit TorrentFileParser(QObject *parent = nullptr);
    Q_INVOKABLE void parse(QString);
    Q_INVOKABLE QByteArray getInfoHash(QString);
};

#endif // TORRENTFILEPARSER_H
#ifndef TORRENTFILEPARSER_H
#define TORRENTFILEPARSER_H

#include <QObject>
#include <QFile>
#include <QDebug>

#include "core/bencode.hpp"

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
};

#endif // TORRENTFILEPARSER_H

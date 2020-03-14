#include "torrentfileparser.h"

TorrentFileParser::TorrentFileParser(QObject *parent):QObject(parent)
{

}

void TorrentFileParser::parse(QString url)
{
    QString fileUrlSubstring = url.mid(7);


    QFile file(fileUrlSubstring);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to read file at: "<< fileUrlSubstring;
    }

    std::istringstream is(file.readAll().toStdString());
    auto data = bencode::decode(is, bencode::no_check_eof);
    auto list = boost::get<bencode::dict>(data);
    auto pos = list.find("announce");
    if (pos == list.end()) {
        std::cout << "Not found announce\n";
    } else {
        auto value = boost::get<bencode::string>(pos->second);
        std::cout << value << "\n";
    }
    pos = list.find("created by");
    if (pos == list.end()) {
        std::cout << "Not found Created by\n";
    } else {
        auto value = boost::get<bencode::string>(pos->second);
        std::cout << value << "\n";
    }
    pos = list.find("creation date");
    if (pos == list.end()) {
        std::cout << "Not found CreationDate\n";
    } else {
        auto value = boost::get<bencode::integer>(pos->second);
        std::cout << value << "\n";
    }
    pos = list.find("info");
    if (pos == list.end()) {
        std::cout << "Not found info\n";
    } else {
        auto value = boost::get<bencode::dict>(pos->second);
        //std::cout << value << "\n";
    }
}

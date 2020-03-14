#include "torrentfileparser.h"

TorrentFileParser::TorrentFileParser(QObject *parent):QObject(parent)
{

}

struct FileStruct
{
    std::vector<QString> path;
    int length;
};

void TorrentFileParser::parse(QString url)
{
    QString announceUrl;
    int creationDate;
    QString comment;
    QString createdBy;
    int pieceLength;
    QString pieces;
    bool privateTorrent = false;

    std::vector<FileStruct> filesVector;


    QString fileUrlSubstring = url.mid(7);


    QFile file(fileUrlSubstring);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to read file at: "<< fileUrlSubstring;
    }

    /*std::istringstream is(file.readAll().toStdString());
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
    }*/


    std::istringstream is(file.readAll().toStdString());
    auto data = bencode::decode(is, bencode::no_check_eof);
    auto all = boost::get<bencode::dict>(data);

    auto pos = all.find("announce");
    if (pos == all.end()) {
        std::cout << "Not found announce\n";
    } else {
        announceUrl = QString::fromStdString(boost::get<bencode::string>(pos->second));
    }

    pos = all.find("created by");
    if (pos == all.end()) {
        std::cout << "Not found Created by\n";
    } else {
        createdBy = QString::fromStdString(boost::get<bencode::string>(pos->second));
        //std::cout << "Created By: " << value << "\n";
    }

    pos = all.find("comment");
    if (pos == all.end()) {
        std::cout << "Not found Comment\n";
    } else {
        comment = QString::fromStdString(boost::get<bencode::string>(pos->second));
        //std::cout << "Created By: " << value << "\n";
    }


    pos = all.find("creation date");
    if (pos == all.end()) {
        std::cout << "Not found CreationDate\n";
    } else {
        creationDate = boost::get<bencode::integer>(pos->second);
        //std::cout << "CreationDate: " << value << "\n";
    }

    pos = all.find("private");
    if (pos == all.end()) {
        std::cout << "Not found Private\n";
    } else {
        int number = boost::get<bencode::integer>(pos->second);
        privateTorrent = (number == 1);
        //std::cout << "CreationDate: " << value << "\n";
    }

    pos = all.find("info");
    bencode::dict info;
    if (pos == all.end()) {
        std::cout << "Not found info\n";
    } else {
        info = boost::get<bencode::dict>(pos->second);
    }
    //std::cout << "Length of info: " << info.size() << "\n";



    pos = info.find("piece length");
    if (pos == info.end()) {
        std::cout << "Not found piece length\n";
    } else {
        pieceLength = boost::get<bencode::integer>(pos->second);
        //std::cout << "Piece Length: " << value << "\n";

    }

    pos = info.find("pieces");
    if (pos == info.end()) {
        std::cout << "Not found pieces\n";
    } else {
        pieces = QString::fromStdString(boost::get<bencode::string>(pos->second));
        //std::cout << "Pieces " << value << "\n";

    }

    bencode::list files;
    pos = info.find("files");
    if (pos == info.end()) {
        std::cout << "Not found info\n";
    } else {
        files = boost::get<bencode::list>(pos->second);
    }

    //Single-file vs mange filer:
    if(files.size() == 0)
    {
        FileStruct f;
        pos = info.find("name");
        if (pos == info.end()) {
            std::cout << "Not found name\n";
        } else {
            f.path.push_back(QString::fromStdString(boost::get<bencode::string>(pos->second)));
        }

        pos = info.find("length");
        if (pos == info.end()) {
            std::cout << "Not found name\n";
        } else {
            f.length = boost::get<bencode::integer>(pos->second);
        }

        filesVector.push_back(f);
    }
    else
    {
        std::cout << "Length of files: " << files.size() << "\n";


        for(auto file : files)
        {
            FileStruct f;

            auto data = boost::get<bencode::dict>(file);
            //td::cout << "decltype(file) is " << type_name<decltype(file)>() << '\n';
            pos = data.find("length");
            if (pos == info.end()) {
                std::cout << "Not FOund length of file\n";
            } else {

                f.length = boost::get<bencode::integer>(pos->second);
            }

            pos = data.find("path");
            bencode::list path;
            if(pos == info.end())
            {
                std::cout << "Not found path\n";
            }
            else
            {
                path = boost::get<bencode::list>(pos->second);
                for(auto attr : path)
                {
                    //std::cout << "decltype(attr) is " << type_name<decltype(attr)>() << '\n';
                    f.path.push_back(QString::fromStdString(boost::get<bencode::string>(attr)));
                }
            }
            filesVector.push_back(f);
        }

    }


    qDebug() << "AnnounceUrl" << announceUrl << "\n";
    qDebug() << "CreationDate: " << creationDate << "\n";
    qDebug() << "Comment" << comment << "\n";
    qDebug() << "Created By: " << createdBy << "\n";
    qDebug() << "Piece Length" << pieceLength << "\n";
    qDebug() << "Pieces" << pieces << "\n";
    qDebug() << "Private Torrent" << privateTorrent << "\n";
    qDebug() << "File(s):\n";
    for(auto file : filesVector)
    {
        for(auto p : file.path)
        {
            qDebug() << "   PathVal: " << p << "\n";
        }
        qDebug() << "   FileLength: " << file.length << "\n\n";

    }
}

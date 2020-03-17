#include "uploadhandler.h"

uploadHandler::uploadHandler(QObject* parent) : QObject(parent)
{

}

struct FileStruct
{
    std::vector<QString> path;
    int length;
};

bool uploadHandler::readFile(QString qtAbsFileUrl)
{
    //Removes 'file::/' from the start of url
    QString absFileUrl = qtAbsFileUrl.mid(7);
    std::ifstream ifs;
    ifs.open(absFileUrl.toUtf8(), std::ios::binary);

    auto data = bencode::decode(ifs, bencode::no_check_eof);
    std::map<std::string, bencode::data> all = boost::get<bencode::dict>(data);
    return uploadDict(all);


}

QString uploadHandler::getInfoHash(std::map<std::string, bencode::data> infoDict){

    std::stringstream is;
    bencode::encode(is, infoDict);
    std::string s = is.str();
    QByteArray qba;
    for(size_t x=0;x<s.length();x++){
        qba.push_back(s[x]);
    }


    /*std::cout << "\n";
    std::cout << "ROUNDTRIP 2 hex \n";
    for(int x=0;x<qba.size();x++){
        std::cout << std::hex << (int)qba.at(x);
    }*/
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    QByteArray infoHash = hasher.hash(qba, QCryptographicHash::Sha1);
    QString hashAsString = infoHash.toHex();
    qDebug() << "HASH: " << hashAsString;
    return hashAsString;
}

QByteArray uploadHandler::getPieces(bencode::string pieces)
{
    QByteArray qba;
    for(size_t x=0;x<pieces.length();x++){
        qba.push_back(pieces[x]);
    }
    return qba;
}

bool uploadHandler::uploadDict(std::map<std::string, bencode::data> dict){




    QString encoding;
    QString announceUrl;
    int creationDate;
    QString comment;
    QString createdBy;
    int pieceLength;
    QByteArray pieces;
    bool privateTorrent = false;
    QString infoHash;

    std::vector<FileStruct> filesVector;



    std::map<std::string, bencode::data>::iterator iterator = dict.find("announce");
    if (iterator == dict.end()) {
        std::cout << "Not found announce\n";
    } else {
        announceUrl = QString::fromStdString(boost::get<bencode::string>(iterator->second));
    }

    iterator = dict.find("info");
    if (iterator == dict.end()) {
        std::cout << "Not found info dict\n";
    } else {
        std::map<std::string, bencode::data> infoDict = boost::get<bencode::dict>(iterator->second);
        infoHash = getInfoHash(infoDict);
    }

    iterator = dict.find("created by");
    if (iterator == dict.end()) {
        std::cout << "Not found Created by\n";
    } else {
        createdBy = QString::fromStdString(boost::get<bencode::string>(iterator->second));
        //std::cout << "Created By: " << value << "\n";
    }

    iterator = dict.find("comment");
    if (iterator == dict.end()) {
        std::cout << "Not found Comment\n";
    } else {
        comment = QString::fromStdString(boost::get<bencode::string>(iterator->second));
        //std::cout << "Created By: " << value << "\n";
    }


    iterator = dict.find("creation date");
    if (iterator == dict.end()) {
        std::cout << "Not found CreationDate\n";
    } else {
        creationDate = boost::get<bencode::integer>(iterator->second);
        //std::cout << "CreationDate: " << value << "\n";
    }

    iterator = dict.find("private");
    if (iterator == dict.end()) {
        std::cout << "Not found Private\n";
    } else {
        int number = boost::get<bencode::integer>(iterator->second);
        privateTorrent = (number == 1);
        //std::cout << "CreationDate: " << value << "\n";
    }

    iterator = dict.find("info");
    bencode::dict info;
    if (iterator == dict.end()) {
        std::cout << "Not found info\n";
    } else {
        info = boost::get<bencode::dict>(iterator->second);
    }

    iterator = info.find("piece length");
    if (iterator == info.end()) {
        std::cout << "Not found piece length\n";
    } else {
        pieceLength = boost::get<bencode::integer>(iterator->second);
        //std::cout << "Piece Length: " << value << "\n";
    }

    iterator = info.find("pieces");
    if (iterator == info.end()) {
        std::cout << "Not found pieces\n";
    } else {
        pieces = getPieces(boost::get<bencode::string>(iterator->second));
        //std::cout << "Pieces " << value << "\n";

    }

    bencode::list files;
    iterator = info.find("files");
    if (iterator == info.end()) {
        std::cout << "Not found info\n";
    } else {
        files = boost::get<bencode::list>(iterator->second);
    }

    //Single-file vs mange filer:
    if(files.size() == 0)
    {
        FileStruct f;
        iterator = info.find("name");
        if (iterator == info.end()) {
            std::cout << "Not found name\n";
        } else {
            f.path.push_back(QString::fromStdString(boost::get<bencode::string>(iterator->second)));
        }

        iterator = info.find("length");
        if (iterator == info.end()) {
            std::cout << "Not found name\n";
        } else {
            f.length = boost::get<bencode::integer>(iterator->second);
        }

        filesVector.push_back(f);
    }
    else
    {
        //std::cout << "Length of files: " << files.size() << "\n";


        for(auto file : files)
        {
            FileStruct f;

            auto data = boost::get<bencode::dict>(file);
            //td::cout << "decltype(file) is " << type_name<decltype(file)>() << '\n';
            iterator = data.find("length");
            if (iterator == info.end()) {
                std::cout << "Not FOund length of file\n";
            } else {

                f.length = boost::get<bencode::integer>(iterator->second);
            }

            iterator = data.find("path");
            bencode::list path;
            if(iterator == info.end())
            {
                std::cout << "Not found path\n";
            }
            else
            {
                path = boost::get<bencode::list>(iterator->second);
                for(auto attr : path)
                {
                    //std::cout << "decltype(attr) is " << type_name<decltype(attr)>() << '\n';
                    f.path.push_back(QString::fromStdString(boost::get<bencode::string>(attr)));
                }
            }
            filesVector.push_back(f);
        }

    }
     QString query = "INSERT INTO torrent (";
     for(auto file : filesVector)
     {
         for(auto p : file.path)
         {
             qDebug() << "   PathVal: " << p << "\n";
         }
         qDebug() << "   FileLength: " << file.length << "\n\n";
     }
     qDebug() << query;
    /* QSqlQuery q = db->query();
     q.prepare(query);
     //q.bindValue(":username", username);
    // q.bindValue(":password", hashedPassword);
     //q.bindValue(":email", email);
     if (q.exec())
     {
         qDebug() << "Torrent uploaded";
         return true;
     }
     else
     {
         qDebug() << "Torrent upload failed";
         return false;
     }*/


}

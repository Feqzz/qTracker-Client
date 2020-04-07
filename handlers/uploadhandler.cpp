#include "uploadhandler.h"

UploadHandler::UploadHandler(QObject* parent) : QObject(parent)
{

}
void UploadHandler::setTorrentPass(QString _torrentPass)
{
    this->torrentPass = _torrentPass;
}
void UploadHandler::setUrl(QString _url)
{
    this->url = _url;
}

void UploadHandler::setTitle(QString _title)
{
    this->title = _title;
}


bool UploadHandler::uploadFile()
{
    //Removes 'file::/' from the start of url
    QString absFileUrl = url.mid(7);

    QStringList fileTitle = absFileUrl.split('/');
    QString fileNameWithExtension = fileTitle[fileTitle.length()-1];

    QString onlyFileName = fileNameWithExtension.mid(0,fileNameWithExtension.length()-8);
    this->fileName = onlyFileName;
    if(title.length()<=0)
    {
        title = onlyFileName;
    }
    qDebug() << "File title: " << title;

    std::ifstream ifs;
    ifs.open(absFileUrl.toUtf8(), std::ios::binary);

    auto data = bencode::decode(ifs, bencode::no_check_eof);
    std::map<std::string, bencode::data> all = boost::get<bencode::dict>(data);
    return uploadDict(all);


}

QString UploadHandler::getInfoHash(std::map<std::string, bencode::data> infoDict){

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

void UploadHandler::setId(int _id)
{
    this->id = _id;
}

QByteArray UploadHandler::getPieces(bencode::string pieces)
{
    QByteArray qba;
    for(size_t x=0;x<pieces.length();x++){
        qba.push_back(pieces[x]);
    }
    return qba;
}

bool UploadHandler::insertTorrentData(std::vector<QVariant> torrentVars,std::vector<FileStruct> files,QString torrentQuery)
{
    db->startTransaction();
    QSqlQuery q = db->query();
    //qDebug() << torrentQuery;
    q.prepare(torrentQuery);

    for(size_t x=0;x<torrentVars.size();x++)
    {
       // qDebug() << torrentVars.at(x);
        q.bindValue(x, torrentVars.at(x));
    }
    bool torrentQuerySuccess = q.exec();
    if(torrentQuerySuccess)
    {
        QVariant torrentId = q.lastInsertId();

        QString torrentFileQuery = "INSERT INTO torrentFiles (torrentId,length) VALUES (?,?)";

        for(auto file : files)
        {
            q.prepare(torrentFileQuery);
            q.bindValue(0, torrentId);
            q.bindValue(1, file.length);
            bool torrentFileQuerySuccess = q.exec();
            if(torrentFileQuerySuccess)
            {
                QVariant torrentFileId = q.lastInsertId();
                QString torrentFilePathQuery = "INSERT INTO torrentFilePaths (torrentFilesId,path) VALUES (?,?)";
                for(auto p : file.path)
                {
                    q.prepare(torrentFilePathQuery);
                    q.bindValue(0, torrentFileId);
                    q.bindValue(1, p);
                    bool torrentFilePathQuerySuccess = q.exec();
                    if(!torrentFilePathQuerySuccess)
                    {
                        qDebug() << q.lastError();
                        db->rollBack();
                        return false;
                    }
                }
            }
            else
            {
                qDebug() << q.lastError();
                db->rollBack();
                qDebug() << "SQL failed on torrentFile insert";
                return false;
            }
        }

    }
    else
    {
        qDebug() << q.lastError();
        db->rollBack();
        qDebug() << "SQL failed on torrent insert, query:";
        qDebug() << torrentQuery;
        return false;
    }
    db->commit();
    return true;
}

bool UploadHandler::uploadDict(std::map<std::string, bencode::data> dict){

    std::vector<FileStruct> filesVector;
    std::vector<QVariant> sqlVariables;

    sqlVariables.push_back(id);
    sqlVariables.push_back(title);
    sqlVariables.push_back(fileName);
    QString torrentQuery = "INSERT INTO torrent (uploader, title, name";
    QString torrentValues = "VALUES (?,?,?";

    std::map<std::string, bencode::data>::iterator iterator;
    iterator= dict.find("announce");
    if (iterator == dict.end()) {
        std::cout << "Not found announce\n";
    } else {
        QString announceUrl = QString::fromStdString(boost::get<bencode::string>(iterator->second));
        QStringList l = announceUrl.split("/");
        QString fileTorrentPass = l[l.length()-2];
        if(torrentPass.compare(fileTorrentPass)!=0)
        {
            qDebug() << "fileTorrentPass does not match with userTorrentPass \n" << "File torrentPass " << fileTorrentPass << " User torrentPass " << torrentPass;
            return false;
        }
    }

    iterator = dict.find("info");
    if (iterator == dict.end()) {
        std::cout << "Not found info dict, aborting upload\n";
        return false;
    } else {
        std::map<std::string, bencode::data> infoDict = boost::get<bencode::dict>(iterator->second);
        torrentQuery+=",infoHash";
        torrentValues+=",?";
        sqlVariables.push_back(getInfoHash(infoDict));
    }

    //OPTIONAL VARS

    iterator = dict.find("created by");
    if (iterator == dict.end()) {
        std::cout << "Not found Created by\n";
    } else {
        sqlVariables.push_back(QString::fromStdString(boost::get<bencode::string>(iterator->second)));
        torrentValues+=",?";
        torrentQuery+=",createdByClient";
        //std::cout << "Created By: " << value << "\n";
    }

    iterator = dict.find("encoding");
    if (iterator == dict.end()) {
        std::cout << "Not found encoding\n";
    } else {
        sqlVariables.push_back(QString::fromStdString(boost::get<bencode::string>(iterator->second)));
        torrentQuery+=",encoding";
        torrentValues+=",?";
        //std::cout << "Created By: " << value << "\n";
    }

    iterator = dict.find("comment");
    if (iterator == dict.end()) {
        std::cout << "Not found Comment\n";
    } else {
        sqlVariables.push_back(QString::fromStdString(boost::get<bencode::string>(iterator->second)));
        torrentQuery+=",comment";
        torrentValues+=",?";
        //std::cout << "Created By: " << value << "\n";
    }


    iterator = dict.find("creation date");
    if (iterator == dict.end()) {
        std::cout << "Not found CreationDate\n";
    } else {
        sqlVariables.push_back(boost::get<bencode::integer>(iterator->second));
        torrentQuery+=",createdDate";
        torrentValues+=",FROM_UNIXTIME(?)";
        //std::cout << "CreationDate: " << value << "\n";
    }




    iterator = dict.find("info");
    bencode::dict info;
    if (iterator == dict.end()) {
        std::cout << "Not found info, aborting upload\n";
        return false;
    } else {
        info = boost::get<bencode::dict>(iterator->second);
    }

    iterator = info.find("piece length");
    if (iterator == info.end()) {
        std::cout << "Not found piece length, aborting upload\n";
        return false;
    } else {
        sqlVariables.push_back(boost::get<bencode::integer>(iterator->second));
        torrentValues+=",?";
        torrentQuery+=",pieceLength";
        //std::cout << "Piece Length: " << value << "\n";
    }

    iterator = info.find("pieces");
    if (iterator == info.end()) {
        std::cout << "Not found pieces, aborting upload\n";
        return false;
    } else {
        sqlVariables.push_back(getPieces(boost::get<bencode::string>(iterator->second)));
        torrentValues+=",?";
        torrentQuery+=",piece";
        //std::cout << "Pieces " << value << "\n";

    }

    iterator = info.find("private");
    int number = 0;
    if (iterator == info.end()) {
        std::cout << "Not found Private\n";
    } else {
        number = boost::get<bencode::integer>(iterator->second);
        sqlVariables.push_back(number == 1);
        torrentValues+=",?";
        torrentQuery+=",private";
    }

    bencode::list files;
    iterator = info.find("files");
    if (iterator == info.end()) {
        std::cout << "Not found files\n";
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
    torrentQuery+= ") ";
    torrentValues+= ");";
    torrentQuery+= torrentValues;
    return insertTorrentData(sqlVariables,filesVector,torrentQuery);
}

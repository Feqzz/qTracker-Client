#include "torrentfileparser.h"

TorrentFileParser::TorrentFileParser(QObject *parent):QObject(parent)
{

}

struct FileStruct
{
    std::vector<QString> path;
    int length;
};




//QByteArray TorrentFileParser::getInfoHash(QString encodedInfo)
//{
    //qDebug() << "InfoHash: " << encodedInfo << "\n";
    //QCryptographicHash hasher(QCryptographicHash::Sha1);
    /*std::ifstream ifs;
    ifs.open(fileUrlSubstring.toLocal8Bit(), std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    //qDebug() << QString::fromStdString(str);

    unsigned first = str.find("infod") + 7;
    unsigned last = str.find(":piece lengthi");
    std::string strNew = str.substr (first,last-first);
    qDebug() << QString::fromStdString(strNew) << "\n";
    */
    //qDebug() << encodedInfo << "\n";
    //QByteArray infoHash = hasher.hash(QString::fromStdString(strNew).toLocal8Bit(), QCryptographicHash::Sha1);

    //Med Url Enc
    //QByteArray urlEnc = QUrl(encodedInfo).toEncoded();
    //QByteArray infoHash = hasher.hash((urlEnc), QCryptographicHash::Sha1);
    //Uten
    /*QByteArray infoBytes = encodedInfo.toUtf8();

    qDebug() << "DECODED INFO BYTES: " << infoBytes.toHex();

    QByteArray infoHash = hasher.hash((encodedInfo).toUtf8(), QCryptographicHash::Sha1);

    QString hashAsString = infoHash.toHex();
    //qDebug() << "DECODED HASH: " << hashAsString;
    return infoHash;
}*/

void TorrentFileParser::getInfoHashFromFile(QString url)
{
    QString fileUrlSubstring = url.mid(7);
    qDebug() << "FileUrl: " << fileUrlSubstring;
    std::ifstream ifs;
    ifs.open(fileUrlSubstring.toLocal8Bit(), std::ifstream::in);

    std::vector<char> *list = new std::vector<char>();
    char c = ifs.get();
    while (ifs.good()) {
        list->push_back(c);
        c = ifs.get();
    }
    ifs.close();
    recursiveParser(list,0);
    /*for(int x=0;x<infoBytes.size();x++){
       std::cout << infoBytes.at(x);
    }
    std::cout << "\n";*/
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    QByteArray infoHash = hasher.hash(infoBytes, QCryptographicHash::Sha1);

    QString hashAsString = infoHash.toHex();
    // qDebug() << "FILE INFO BYTES: " << barr.toHex();
    qDebug() << "BENCODE PARSE HASH: " << hashAsString;

}



int TorrentFileParser::recursiveParser(std::vector<char>* list,int i){
    /*
     * Should start with d,i,l or a number
     * d,i,l ends with e
     * number ends after the number+1
    */

    static bool infoFound = false;
    static std::vector<bool> hm;
    for(size_t x=i;x<list->size();x++){
        char c = list->at(x);
        //starting conditions
        if(((int)c >= 48 && (int)c <= 57)){
            std::string number = "";
            int count = 0;
            while(((int)list->at(x+count) >= 48 && (int)list->at(x+count) <= 57)){
                if(infoFound){
                    infoBytes.push_back(list->at(x+count));
                }
                number+=list->at(x+count);
                count++;
            }

            int j = std::stoi(number);

            for(int z=0;z<=j;z++){
                if(infoFound){
                    infoBytes.push_back(list->at(x+count+z));
                }
            }

            //info is always 4 length
            if(!infoFound && j==4){
                //qDebug() << "Checking for infod at: "<<x+count;
                std::string infodString = "";
                infodString+=list->at(x+count+1);
                infodString+=list->at(x+count+2);
                infodString+=list->at(x+count+3);
                infodString+=list->at(x+count+4);
                infoFound=(infodString.compare("info")) == 0;
                if(infoFound){
                   // qDebug() << "Found infod at: "<<x+count;
                    //hm.push_back(true);
                }
            }
            //qDebug() << "Found string with length: " << j << "start index: " <<x+1;
            //qDebug() << "Found string with length: " << j << "end index: " <<x+j+count+2;
            int ending = recursiveParser(list,x+j+count+1);

            return ending;

        }
        if(c=='d'){
            if(infoFound){
                hm.push_back(true);
                infoBytes.push_back(c);
            }
            //qDebug() << "Found dict with start index: " <<x+1;
            int ending =recursiveParser(list,x+1);
           // qDebug() << "Found dict with end index: " <<ending+2;
            return ending+1;
        }
        if(c=='l'){
            if(infoFound){
                hm.push_back(true);
                infoBytes.push_back(c);
            }
            //qDebug() << "Found list with start index: " <<x+1;
            int ending =recursiveParser(list,x+1);
            //qDebug() << "Found list with end index: " <<ending+2;
            return ending+1;
        }
        if(c=='i'){
            //qDebug() << "Found integer with start index: " <<x+1;
            int y = 0;
            while(c!='e'){
                c=list->at(x+y);
                if(infoFound){
                    infoBytes.push_back(c);
                }
                y++;
            }
            //qDebug() << "Found integer with end index: " <<x+y+1;
            int ending = recursiveParser(list,x+y);

            return ending;
        }
        if(c=='e'){
            if(infoFound){
                infoBytes.push_back(c);
                //qDebug() << hm.size()<<" Popped e vector: " << x;
                hm.pop_back();
            }
            if(hm.empty()){
                infoFound=false;
                //qDebug() << "Stopped infod at: " << x;
            }
            return recursiveParser(list,x+1);
        }
    }
}

/*void TorrentFileParser::readFile(QString url)
{

    QString fileUrlSubstring = url.mid(7);
    std::ifstream ifs;
    ifs.open(fileUrlSubstring.toLocal8Bit(), std::ifstream::in);

    std::vector<char> list;
    char c = ifs.get();
    while (ifs.good()) {
        list.push_back(c);
        c = ifs.get();
    }
    ifs.close();

    bool f = false;
    QByteArray barr;
    for(size_t x=0;x<list.size()-1;x++){
        if(!f)
        {
            char first = list.at(x);
            char second = list.at(x+1);
            char third = list.at(x+2);
            char fourth = list.at(x+3);
            if(first == 'i'&& second == 'n'&& third == 'f'&& fourth == 'o'){
                f=true;
                x+=4;
            }
        }
        if(f)
        {
            barr.push_back(list.at(x));
        }
    }

    QCryptographicHash hasher(QCryptographicHash::Sha1);
    QByteArray infoHash = hasher.hash(barr, QCryptographicHash::Sha1);

    QString hashAsString = infoHash.toHex();
    // qDebug() << "FILE INFO BYTES: " << barr.toHex();
    qDebug() << "FILE HASH: " << hashAsString;
}*/

void TorrentFileParser::parse(QString url)
{
   // QCryptographicHash hasher(QCryptographicHash::Sha1);

    QString announceUrl;
    int creationDate;
    QString comment;
    QString createdBy;
    int pieceLength;
    QString pieces;
    bool privateTorrent = false;
    //QByteArray infoHash;

    std::vector<FileStruct> filesVector;


    QString fileUrlSubstring = url.mid(7);


    /*QFile file(fileUrlSubstring);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to read file at: "<< fileUrlSubstring;
    }
    std::istringstream is(file.readAll().toStdString());*/

    std::ifstream ifs;
    ifs.open(fileUrlSubstring.toLocal8Bit(), std::ifstream::in);

    //infoHash = getInfoHash(fileUrlSubstring);

    auto data = bencode::decode(ifs, bencode::no_check_eof);
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

    //auto encodedInfo = bencode::encode(info);
    //qDebug() << info;
    //infoHash = getInfoHash(QString::fromStdString(encodedInfo));



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
        //std::cout << "Length of files: " << files.size() << "\n";


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


    //qDebug() << "AnnounceUrl" << announceUrl << "\n";
    //qDebug() << "CreationDate: " << creationDate << "\n";
    //qDebug() << "Comment" << comment << "\n";
    //qDebug() << "Created By: " << createdBy << "\n";
    //qDebug() << "Piece Length" << pieceLength << "\n";
    //qDebug() << "Pieces" << pieces << "\n";
    //qDebug() << "Private Torrent" << privateTorrent << "\n";
    //qDebug() << "File(s):\n";
    /*for(auto file : filesVector)
    {
        for(auto p : file.path)
        {
            qDebug() << "   PathVal: " << p << "\n";
        }
        qDebug() << "   FileLength: " << file.length << "\n\n";
    }*/

}

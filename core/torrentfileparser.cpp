#include "torrentfileparser.h"

TorrentFileParser::TorrentFileParser(QObject *parent):QObject(parent)
{

}


void TorrentFileParser::setTorrentPass(QString _torrentPass)
{
    this->torrentPass = _torrentPass;
}

void TorrentFileParser::getTorrentFile(int torrentId)
{
    QString title;
    QString encoding;
    QString comment;
    QString createdByClient;
    qlonglong createdtime;
    qlonglong pieceLength;
    QByteArray pieces;
    bool privateTorrent;

    QString torrentQuery = "SELECT title,encoding,comment,createdByClient,UNIX_TIMESTAMP(createdDate) as createdtime,pieceLength,piece,private FROM torrent WHERE id = ?;";
    QSqlQuery q = db->query();
    q.prepare(torrentQuery);
    q.bindValue(0, torrentId);
    if (q.exec())
    {
        if(q.next())
        {
            title = q.value(0).toString();
            encoding = q.value(1).toString();
            comment = q.value(2).toString();
            createdByClient = q.value(3).toString();
            createdtime = q.value(4).toLongLong();
            pieceLength = q.value(5).toLongLong();
            pieces = q.value(6).toByteArray();
            privateTorrent = q.value(7).toBool();
            std::vector<TorrentFileParser::FileStruct> t = getTorrentFiles(torrentId);
            std::map<std::string, bencode::data> all;
            QString announceURL = "http://www."+trackerHostName+"/"+torrentPass+"/announce";
            all.insert(std::pair<std::string,bencode::data>("announce",announceURL.toStdString()));
            if(encoding.length()>0)
            {
                all.insert(std::pair<std::string,bencode::data>("encoding",encoding.toStdString()));
            }
            if(comment.length()>0)
            {
                all.insert(std::pair<std::string,bencode::data>("comment",comment.toStdString()));
            }
            if(createdByClient.length()>0)
            {
                all.insert(std::pair<std::string,bencode::data>("created by",createdByClient.toStdString()));
            }
            if(createdtime>0)
            {
                all.insert(std::pair<std::string,bencode::data>("creation date",createdtime));
            }

            std::map<std::string, bencode::data> info;
            info.insert(std::pair<std::string,bencode::data>("piece length",pieceLength));
            info.insert(std::pair<std::string,bencode::data>("pieces",pieces.toStdString()));
            info.insert(std::pair<std::string,bencode::data>("name",title.toStdString()));
            if(privateTorrent)
            {
                info.insert(std::pair<std::string,bencode::data>("private",privateTorrent));
            }

            //bencode::dict all;
            //Multiple files mode
            if(t.size()>1)
            {
                std::vector<bencode::data> fileList;
                for(size_t y=0;y<t.size();y++)
                {
                    std::map<std::string, bencode::data> fileDict;
                    fileDict.insert(std::pair<std::string,bencode::data>("length",t.at(y).length));
                    std::vector<QString> paths = t.at(y).path;
                    std::vector<bencode::data> pathList;
                    for(size_t x=0;x<paths.size();x++)
                    {
                        pathList.push_back(paths.at(x).toStdString());
                    }
                    fileDict.insert(std::pair<std::string,bencode::data>("path",pathList));
                    fileList.push_back(fileDict);
                }
                info.insert(std::pair<std::string,bencode::data>("files",fileList));
            }
            else
            {
                info.insert(std::pair<std::string,bencode::data>("length",t.at(0).length));
                info.insert(std::pair<std::string,bencode::data>("name",t.at(0).path.at(0).toStdString()));
            }
            all.insert(std::pair<std::string,bencode::data>("info",info));
            std::ofstream ofs;
            ofs.open((title+".torrent").toUtf8());
            bencode::encode(ofs, all);
            ofs.close();
        }
    }
    else
    {
        qDebug() << q.lastError();
    }


}

std::vector<TorrentFileParser::FileStruct> TorrentFileParser::getTorrentFiles(int torrentId)
{
    std::vector<TorrentFileParser::FileStruct> fileVector;
    QString torrentFileQuery = "SELECT id,length FROM torrentFiles WHERE torrentId = ?;";
    QSqlQuery q = db->query();
    q.prepare(torrentFileQuery);
    q.bindValue(0, torrentId);
    if (q.exec())
    {
        while(q.next())
        {
            FileStruct f;
            f.length = q.value(1).toLongLong();
            int fileId = q.value(0).toInt();
            QString torrentFilePathQuery = "SELECT path FROM torrentFilePaths WHERE torrentFilesId = ?;";
            QSqlQuery q2 = db->query();
            q2.prepare(torrentFilePathQuery);
            q2.bindValue(0, fileId);
            if (q2.exec())
            {
                while(q2.next())
                {
                    f.path.push_back(q2.value(0).toString());
                }
            }
            else
            {
                qDebug() << q2.lastError();
            }
            fileVector.push_back(f);

        }
    }
    else
    {
        qDebug() << q.lastError();
    }
    return fileVector;

}

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

void TorrentFileParser::createFile(QString url)
{
    QString fileUrlSubstring = url.mid(7);
    QString readUrl = "/home/tarves/ttest.torrent";
    QString writeUrl = "/home/tarves/ttest.altered.torrent";
    QString announceUrl;


    std::ifstream ifs;
    ifs.open(fileUrlSubstring.toUtf8(), std::ios::binary);

    auto data = bencode::decode(ifs, bencode::no_check_eof);
    auto all = boost::get<bencode::dict>(data);

    std::map<std::string, bencode::data>::iterator it;
    it=all.find("announce");
    it->second = "http://www.feqzz.no/announce";

    /*std::ofstream ofs;
    ofs.open(writeUrl.toUtf8(), std::ios::binary);
    bencode::encode(ofs, all);
    ofs.close();*/

    //Litt teit å lagre fil, så lese fil

    /*it = all.find("info");
    auto info = boost::get<bencode::dict>(it->second);
    std::ofstream infoofs;
    infoofs.open((readUrl+".info").toUtf8(), std::ios::binary);
    bencode::encode(infoofs, info);
    infoofs.close();

    QByteArray qba;
    std::ifstream is ((readUrl+".info").toUtf8(), std::ifstream::binary);
    while(!is.eof()){
        qba.append(is.get());
    }

    std::cout << "\n";
    std::cout << "ROUNDTRIP hex \n";
    for(int x=0;x<qba.size();x++){
       std::cout << std::hex << (int)qba.at(x);
    }
    std::cout << "\n";
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    QByteArray infoHash = hasher.hash(qba, QCryptographicHash::Sha1);
    QString hashAsString = infoHash.toHex();
    qDebug() << "ROUNDTRIP HASH: " << hashAsString;*/


    it = all.find("info");
    auto info = boost::get<bencode::dict>(it->second);

    std::stringstream is;
    bencode::encode(is, info);
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
    qDebug() << "ROUNDTRIP HASH: " << hashAsString;

}

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
    /* std::cout << "\n";
    std::cout << "Korrekt hex \n";
    for(int x=0;x<infoBytes.size();x++){
        std::cout << std::hex << (int)infoBytes.at(x);
    }
    std::cout << "\n";*/
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    QByteArray infoHash = hasher.hash(infoBytes, QCryptographicHash::Sha1);

    QString hashAsString = infoHash.toHex();
    // qDebug() << "FILE INFO BYTES: " << barr.toHex();
    qDebug() << "BENCODE   HASH: " << hashAsString;

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
        std::cout << "Not found private\n";
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

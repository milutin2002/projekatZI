#include "networksender.h"

NetworkSender::NetworkSender(QTcpSocket *socket,QString filePath):socket(socket),filePath(filePath) {

}

void NetworkSender::run(){
    QDataStream stream{socket};
    QFile file{filePath};
    TigerHash *t=new TigerHash();
    if(file.open(QFile::ReadOnly)){
        QByteArray fileContent=file.readAll();
        std::string hash=t->hashTiger(fileContent.toStdString());
        QByteArray hashBytes=QByteArray::fromStdString(hash);
        stream<<filePath;
        stream<<fileContent.size();
        stream<<hashBytes.size();
        stream<<hashBytes;
        stream<<fileContent;
    }
}

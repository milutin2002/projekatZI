#include "networkrecv.h"

NetworkRecv::NetworkRecv(QTcpSocket *socket,QString decodeDir):socket(socket) {}

void NetworkRecv::run(){
    QDataStream stream(socket);
    QString filename;
    quint64 fileSize;
    quint32 hashSize;
    QByteArray hash;
    QByteArray fileContent;
    stream>>filename;
    stream>>fileSize;
    stream>>hashSize;
    hash=socket->read(hashSize);
    fileContent=socket->readAll();
    QFile file(decodeDir+filename);
    if(!file.open(QIODevice::WriteOnly) ){
        file.write(fileContent);
    }
}

#include "fileservice.h"
#include <qdebug.h>

FileService::FileService() {}

QByteArray FileService::readAllTheText(const QString& filename){
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)){
        return file.readAll();
    }
    return QByteArray();
}

void FileService::writeAllTheText(const QString &filename,const QByteArray&data){
    QFile outputFile(filename);
    if(!outputFile.open(QIODevice::WriteOnly)){
        qWarning()<<"Failed to open file with";
        return;
    }
    outputFile.write(data);
}

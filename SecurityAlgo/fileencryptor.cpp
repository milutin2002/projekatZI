#include "fileencryptor.h"
#include<QMessageBox>
FileEncryptor::FileEncryptor() {}

FileEncryptor::FileEncryptor(QString ext){
    this->ext=ext;
}

QByteArray FileEncryptor::readAllTheText(const QString& filename){
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)){
        return file.readAll();
    }
    return QByteArray();
}
void FileEncryptor::encryptFile(const QString& file,const QString &decodeDir){
    QByteArray data=readAllTheText(file);
    QByteArray encodedData=encrypt(data);
    QString filename=file.split("/").back();
    QString enFilename=decodeDir+"/"+filename+ext;
    QFile outputFile(enFilename);
    if(!outputFile.open(QIODevice::WriteOnly)){
        qWarning()<<"Failed to open file with";
        return;
    }
    outputFile.write(encodedData);
    //QMessageBox::information(nullptr,"Success","Finished encryption of file "+file);
}
void FileEncryptor::decryptFile(const QString& file){
    if(!file.endsWith(ext)){
        qWarning()<<"Not encrypted with given encryption ";
        return;
    }
    int lastDot = file.lastIndexOf('.');
    QString deFile = file.left(lastDot);
    QByteArray data=readAllTheText(file);
    QByteArray encodedData=decrypt(data);
    QFile outputFile(deFile);
    if(!outputFile.open(QIODevice::WriteOnly)){
        qWarning()<<"Failed to open file with filename ";
        return;
    }
    outputFile.write(encodedData);
   // QMessageBox::information(nullptr,"Success","Finished decryption of file "+file);
}

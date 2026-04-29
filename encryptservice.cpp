#include "encryptservice.h"

EncryptService::EncryptService() {}

void EncryptService::encryptFile(const QString& file,const QString &decodeDir){
    if (!encryptor) return;
    QByteArray data=readAllTheText(file);
    QByteArray encodedData=this->encryptor->encrypt(data);
    QString filename=file.split("/").back();
    QString enFilename=decodeDir+"/"+filename+this->encryptor->getExt();
    QFile outputFile(enFilename);
    if(!outputFile.open(QIODevice::WriteOnly)){
        qWarning()<<"Failed to open file with";
        return;
    }
    outputFile.write(encodedData);
}

void EncryptService::decryptFile(const QString &file){
    if(!file.endsWith(encryptor->getExt())){
        qWarning()<<"Not encrypted with given encryption ";
        return;
    }
    int lastDot = file.lastIndexOf('.');
    QString deFile = file.left(lastDot);
    QByteArray data=readAllTheText(file);
    QByteArray encodedData=this->encryptor->decrypt( data);
    QFile outputFile(deFile);
    if(!outputFile.open(QIODevice::WriteOnly)){
        qWarning()<<"Failed to open file with filename ";
        return;
    }
    outputFile.write(encodedData);
}

QByteArray EncryptService::readAllTheText(const QString& filename){
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)){
        return file.readAll();
    }
    return QByteArray();
}

void EncryptService::setEncryptor(std::unique_ptr<FileEncryptor>encryptor){
    this->encryptor=std::move(encryptor);
}

void EncryptService::updateEnigmaSettings(const QString &key,const QString &ring,const QString&rotor,const QString&plugboard){

}
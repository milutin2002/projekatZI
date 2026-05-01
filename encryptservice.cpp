#include "encryptservice.h"

EncryptService::EncryptService() {}

void EncryptService::encryptFile(const QString& file,const QString &decodeDir){
    if (!encryptor) return;
    QByteArray data=FileService::readAllTheText(file);
    QByteArray encodedData=this->encryptor->encrypt(data);
    QString filename=file.split("/").back();
    QString enFilename=decodeDir+"/"+filename+this->encryptor->getExt();
    FileService::writeAllTheText(enFilename,data);
}

void EncryptService::decryptFile(const QString &file){
    if(!encryptor)return;
    if(!file.endsWith(encryptor->getExt())){
        qWarning()<<"Not encrypted with given encryption ";
        return;
    }
    int lastDot = file.lastIndexOf('.');
    QString deFile = file.left(lastDot);
    QByteArray data=FileService::readAllTheText(file);
    QByteArray encodedData=this->encryptor->decrypt( data);
    FileService::writeAllTheText(deFile,encodedData);
}



void EncryptService::setEncryptor(std::unique_ptr<FileEncryptor>encryptor){
    this->encryptor=std::move(encryptor);
}

void EncryptService::updateEnigmaSettings(const QString &key,const QString &ring,const QString&rotor,const QString&plugboard){
    if(auto e=dynamic_cast<EnigmaFileEncryptor*>(encryptor.get())){
        e->setSettings(key,ring,rotor,plugboard);
    }
}
void EncryptService::updateXXteaSettings(uint32_t *key,uint32_t *iv){
    if(auto x=dynamic_cast<XXteaCFB*>(encryptor.get())){
        x->setK(key);
        x->setIV(iv);
    }
}
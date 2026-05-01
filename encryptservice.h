#ifndef ENCRYPTSERVICE_H
#define ENCRYPTSERVICE_H

#include<memory.h>
#include<iostream>
#include<QFile>
#include"SecurityAlgo/enigmafileencryptor.h"
#include"SecurityAlgo/xxteacfb.h"
#include"fileservice.h"

using namespace std;
class EncryptService
{
private:
    std::unique_ptr<FileEncryptor>encryptor;
public:
    void setEncryptor(std::unique_ptr<FileEncryptor>e);
    FileEncryptor* getEncryptor() const { return encryptor.get(); }
    EncryptService();
    void encryptFile(const QString& file,const QString &decodeDir);
    void decryptFile(const QString &file);
    void updateEnigmaSettings(const QString &key,const QString &ring,const QString&rotor,const QString&plugboard);
    void updateXXteaSettings(uint32_t *key,uint32_t *iv);
};

#endif // ENCRYPTSERVICE_H

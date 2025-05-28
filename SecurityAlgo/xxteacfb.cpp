#include "xxteacfb.h"

XXteaCFB::XXteaCFB(uint32_t *k,uint32_t *iv):XXteaEncryptor(k),iv(iv) {}

QByteArray XXteaCFB::encrypt( const QByteArray&data){
    std::string text=data.toStdString();
    uint32_t block[4];
    uint8_t c[16];
    memcpy(c,iv,4*sizeof(int));
    std::string res="";
    for(int i=0;i<text.size();i+=16) {
        memcpy(block,c,4*sizeof(int));
        QByteArray encryptedBlock=XXteaEncryptor::encrypt(QByteArray::fromRawData((char *)block,16));
        memcpy(block,encryptedBlock.data(),4*sizeof(int));
        for (size_t j = 0; j < 16 && i + j < text.size(); ++j) {
            uint8_t encrypted_byte = reinterpret_cast<uint8_t *>(block)[j];
            uint8_t plain_byte = text[i + j];
            uint8_t cipher_byte = encrypted_byte ^ plain_byte;
            res.push_back(cipher_byte);
            reinterpret_cast<uint8_t *>(c)[j] = cipher_byte;
        }
    }
    return QByteArray::fromStdString(res);
}
QByteArray XXteaCFB::decrypt(const QByteArray&data){
    std::string cipher=data.toStdString();
    uint8_t c[16];
    uint32_t block[4];
    memcpy(c,iv,4*sizeof(uint32_t));
    std::string res="";
    for(int i=0;i<cipher.size();i+=16) {
        memcpy(block,c,4*sizeof(uint32_t));
        QByteArray encryptedBlock=XXteaEncryptor::encrypt(QByteArray::fromRawData((char*)block,16));
        memcpy(block,encryptedBlock.data(),4*sizeof(int));
        //encrypt(block,4,key);
        for (size_t j = 0; j < 16 && i + j < cipher.size(); ++j) {
            uint8_t encrypted_byte = reinterpret_cast<uint8_t *>(block)[j];
            uint8_t cipher_byte = cipher[i + j];
            uint8_t plain_byte = encrypted_byte ^ cipher_byte;
            res.push_back(plain_byte);
            reinterpret_cast<uint8_t *>(c)[j] = cipher[i+j];
        }
    }
    return QByteArray::fromStdString(res);
}
QString XXteaCFB::decryptPart(const QByteArray&data,int start,int count){
    std::string text=data.toStdString();
    uint8_t c[16];
    uint32_t block[4];
    memcpy(c,iv,4*sizeof(uint32_t));
    std::string res="";
    int i=start*count*16;
    for(i=start*count*16;i<text.size() && count;i+=16) {
        memcpy(block,c,4*sizeof(uint32_t));
        QByteArray encryptedBlock=XXteaEncryptor::encrypt(QByteArray::fromRawData((char*)block,16));
        memcpy(block,encryptedBlock.data(),4*sizeof(int));
        //encrypt(block,4,key);
        for (size_t j = 0; j < 16 && i + j < text.size(); ++j) {
            uint8_t encrypted_byte = reinterpret_cast<uint8_t *>(block)[j];
            uint8_t plain_byte = text[i + j];
            uint8_t cipher_byte = encrypted_byte ^ plain_byte;
            res.push_back(cipher_byte);
            reinterpret_cast<uint8_t *>(c)[j] = text[i+j];
        }
        count--;
    }
    return QString::fromStdString(res);
}

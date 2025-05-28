#ifndef XXTEAENCRYPTOR_H
#define XXTEAENCRYPTOR_H
#define MX ((z>>5)^(y<<2))+((y>>3)^(z<<4))^(sum^y)+ (k[i&3^e]^z)
#define DELTA 0x9e3779b9

#include "fileencryptor.h"

class XXteaEncryptor : public FileEncryptor
{
public:
    XXteaEncryptor(uint32_t *k);
    virtual QByteArray encrypt(const QByteArray& data)override;
    virtual QByteArray decrypt(const QByteArray& data)override;
    void setK(uint32_t *k){
        this->k=k;
    }
protected:
    uint32_t *k;
private:
    void encryptXXtea(uint32_t *a,uint32_t n);
    void decryptXXtea(uint32_t *a,uint32_t n);
};

#endif // XXTEAENCRYPTOR_H

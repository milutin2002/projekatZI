#ifndef SYMETRICFILEENCRYPTOR_H
#define SYMETRICFILEENCRYPTOR_H

#include "fileencryptor.h"

class SymetricFileEncryptor : public FileEncryptor
{
public:
    SymetricFileEncryptor();
     SymetricFileEncryptor(QString ext);
    virtual QByteArray encrypt(const QByteArray&data)=0;
    virtual QByteArray decrypt(const QByteArray&data)override;
};

#endif // SYMETRICFILEENCRYPTOR_H

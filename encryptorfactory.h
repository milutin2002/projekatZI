#ifndef ENCRYPTORFACTORY_H
#define ENCRYPTORFACTORY_H

#include <memory>
#include "encryptservice.h"


class EncryptorFactory
{
public:
    // Enigma creation
    static std::unique_ptr<FileEncryptor> createEnigma(
        const QString& key,
        const QString& ring,
        const QString& rotor,
        const QString& plugboard
        );

    // XXTEA creation
    static std::unique_ptr<FileEncryptor> createXXTEA(
        uint32_t* key,
        uint32_t* iv
        );
};

#endif // ENCRYPTORFACTORY_H

#ifndef ENCRYPTORFACTORY_H
#define ENCRYPTORFACTORY_H

#include <memory>
#include <array>
#include "encryptservice.h"


class EncryptorFactory
{
public:
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
        const std::array<uint32_t,4>& key,
        const std::array<uint32_t,4>& iv
        );
};

#endif // ENCRYPTORFACTORY_H

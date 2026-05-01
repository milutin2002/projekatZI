#include "encryptorfactory.h"


unique_ptr<FileEncryptor> EncryptorFactory::createEnigma(const QString& key,
                                                         const QString& ring,
                                                         const QString& rotor,
                                                         const QString& plugboard){
    auto encryptor=make_unique<EnigmaFileEncryptor>();
    encryptor->setSettings(key,ring,rotor,plugboard);
    return encryptor;
}

unique_ptr<FileEncryptor> EncryptorFactory::createXXTEA(uint32_t* key, uint32_t* iv){
    return make_unique<XXteaCFB>(key,iv);
}

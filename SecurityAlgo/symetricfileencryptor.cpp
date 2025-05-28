#include "symetricfileencryptor.h"

SymetricFileEncryptor::SymetricFileEncryptor() {}

SymetricFileEncryptor::SymetricFileEncryptor(QString ext):FileEncryptor(ext){

}

QByteArray SymetricFileEncryptor::decrypt(const QByteArray& data){
    return encrypt(data);
}

#include "threadencryptor.h"

ThreadEncryptor::ThreadEncryptor(FileEncryptor *f,const QString &filename,QString &decodeDir,bool encrypt) :f(f),filename(filename),decodeDir(decodeDir),encrypt(encrypt){}

void ThreadEncryptor::run(){
    if(encrypt){
        f->encryptFile(filename,decodeDir);
    }
    else{
        f->decryptFile(filename);
    }
}

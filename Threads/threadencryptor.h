#ifndef THREADENCRYPTOR_H
#define THREADENCRYPTOR_H

#include <QRunnable>
#include"../SecurityAlgo/fileencryptor.h"
class ThreadEncryptor : public QRunnable
{
private:
    FileEncryptor *f;
    QString filename;
    QString decodeDir;
    bool encrypt;
public:
    ThreadEncryptor(FileEncryptor *f,const QString &filename,QString &decodeDir,bool encrypt);
    void run()override;
};

#endif // THREADENCRYPTOR_H

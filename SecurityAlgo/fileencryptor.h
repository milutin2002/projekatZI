#ifndef FILEENCRYPTOR_H
#define FILEENCRYPTOR_H

#include<QString>
#include<QFile>
#include<QDebug>

class FileEncryptor
{
private:
    QByteArray readAllTheText(const QString& filename);
    QString ext;
public:
    FileEncryptor();
    FileEncryptor(QString ext);
    virtual QByteArray encrypt(const QByteArray& data)=0;
    virtual QByteArray decrypt(const QByteArray& data)=0;
    void encryptFile(const QString& file,const QString &decodeDir);
    void decryptFile(const QString &file);

};

#endif // FILEENCRYPTOR_H

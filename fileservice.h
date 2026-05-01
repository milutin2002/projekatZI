#ifndef FILESERVICE_H
#define FILESERVICE_H
#include<QByteArray>
#include<QFile>

class FileService
{
public:
    FileService();
    static QByteArray readAllTheText(const QString& filename);
    static void writeAllTheText(const QString & filename,const QByteArray&data);
};

#endif // FILESERVICE_H

#ifndef NETWORKSENDER_H
#define NETWORKSENDER_H

#include <QRunnable>
#include<QTcpSocket>
#include<QFile>
#include"SecurityAlgo/tigerhash.h"
class NetworkSender : public QRunnable
{
private:
    QTcpSocket *socket;
    QString filePath;
public:
    NetworkSender(QTcpSocket *socket,QString filePath);
    void run()override;
};

#endif // NETWORKSENDER_H

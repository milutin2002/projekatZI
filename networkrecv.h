#ifndef NETWORKRECV_H
#define NETWORKRECV_H

#include <QRunnable>
#include<QTcpSocket>
#include<QFile>
class NetworkRecv : public QRunnable
{
private:
    QTcpSocket *socket;
    QString decodeDir;
public:
    NetworkRecv(QTcpSocket *socket,QString decodeDir);
    void run()override;
};

#endif // NETWORKRECV_H

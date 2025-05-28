#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include<QGroupBox>
#include<QRadioButton>
#include<QVBoxLayout>
#include<QPushButton>
#include<QLabel>
#include<QFileDialog>
#include<QLineEdit>
#include<QFileSystemWatcher>
#include"SecurityAlgo/enigmafileencryptor.h"
#include"Threads/threadencryptor.h"
#include"SecurityAlgo/xxteacfb.h"
#include<QThreadPool>
#include<QIntValidator>
#include<QTcpServer>
#include<QInputDialog>
#include<QTcpSocket>
#include<QMessageBox>
#include "networkrecv.h"
#include"networksender.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QGroupBox * createFSWBox();
    QGroupBox * createDecodeMenu();
    QGroupBox * createAlgoMenu();
    QGroupBox  * creatEnigmaMenu();
    QGroupBox * createXXteaMenu();
    QGroupBox * createSendToServer();
    QGroupBox * createEncodeFile();
    bool isKeyRingSettingsOk(QString key);
    bool isRotorOk(QString rotor);
    bool isPlugboard(QString plugboard);
    void handleConnection();

private:
    QString watcherDir,decodeDir,selectedFileToEncrypt,selectedFileToDecode;
    QFileSystemWatcher *watcher;
    Ui::Widget *ui;
    QStringList files;
    QGroupBox *fileMenu;
    QGroupBox *enigmaMenu;
    QGroupBox *xxteaMenu;
    EnigmaFileEncryptor *encryptor;
    XXteaCFB *xxteaEncryptor;
    FileEncryptor **encryptors;
    QTcpServer *server;
    int selected=0;

};
#endif // WIDGET_H

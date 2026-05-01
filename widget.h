#ifndef WIDGET_H
#define WIDGET_H

#include <array>
#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QFileSystemWatcher>
#include <QThreadPool>
#include <QIntValidator>
#include <QTcpServer>
#include <QInputDialog>
#include <QTcpSocket>
#include <QMessageBox>

#include "Threads/threadencryptor.h"
#include "encryptorfactory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QGroupBox* createFSWBox();
    QGroupBox* createDecodeMenu();
    QGroupBox* createAlgoMenu();
    QGroupBox* creatEnigmaMenu();
    QGroupBox* createXXteaMenu();
    QGroupBox* createSendToServer();
    QGroupBox* createEncodeFile();
    void handleConnection();

private:
    bool isKeyRingSettingsOk(const QString& key) const;
    bool isRotorOk(const QString& rotor) const;
    bool isPlugboard(const QString& plugboard) const;

    QString watcherDir, decodeDir;
    QFileSystemWatcher *watcher;
    QStringList files;
    Ui::Widget *ui;

    QGroupBox *fileMenu;
    QGroupBox *enigmaMenu;
    QGroupBox *xxteaMenu;

    std::array<uint32_t,4> xxteaKey{1,2,3,4};
    std::array<uint32_t,4> xxteaIV{1,2,3,4};
    QString enigmaKey{"AAA"}, enigmaRing{"AAA"}, enigmaRotor{"123"};
    QString enigmaPlugboard{"PO ML IU KJ NH YT GB VF RE DC"};

    EncryptService service;
    QTcpServer *server;
};

#endif // WIDGET_H

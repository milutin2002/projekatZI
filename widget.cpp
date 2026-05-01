#include "widget.h"
#include "./ui_widget.h"
#include "SecurityAlgo/tigerhash.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    int port = QInputDialog::getInt(this,"Select port for tcp server","Port",4444,1,65536,1);
    server = new QTcpServer(this);
    if(server->listen(QHostAddress{"127.0.0.1"}, port))
        qDebug() << "Server started listening\n";
    else
        qDebug() << "Server failed\n";
    connect(server, &QTcpServer::newConnection, this, &Widget::handleConnection);

    service.setEncryptor(EncryptorFactory::createEnigma(enigmaKey,enigmaRing,enigmaRotor,enigmaPlugboard));

    watcherDir = QFileDialog::getExistingDirectory(this,"Select watch dir","/home/milutin");
    decodeDir  = QFileDialog::getExistingDirectory(this,"Select decode dir","/home/milutin");

    watcher = new QFileSystemWatcher(this);
    QDir dir(watcherDir);
    files = dir.entryList();
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, [&](const QString& path){
        QDir dir(path);
        QStringList l;
        for(const QString& file : dir.entryList()){
            QFile fileN(path+"/"+file);
            if(!files.contains(fileN.fileName())){
                ThreadEncryptor *t = new ThreadEncryptor(service.getEncryptor(),path+"/"+file,decodeDir,true);
                QThreadPool::globalInstance()->start(t);
                l.append(file);
            }
        }
        files = l;
    });

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(createFSWBox(),      0, 0);
    grid->addWidget(createDecodeMenu(),  0, 1);
    grid->addWidget(createEncodeFile(),  0, 2);
    grid->addWidget(createAlgoMenu(),    1, 0);
    grid->addWidget(creatEnigmaMenu(),   1, 1);
    grid->addWidget(createXXteaMenu(),   1, 2);
    grid->addWidget(createSendToServer(),2, 0);
    setLayout(grid);
    setWindowTitle(tr("App"));
    resize(480,320);
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::isKeyRingSettingsOk(const QString& key) const {
    return key.size()==3 && key[0].isUpper() && key[1].isUpper() && key[2].isUpper();
}

bool Widget::isRotorOk(const QString& rotor) const {
    return rotor.size()==3
        && rotor[0]>='1' && rotor[0]<='8'
        && rotor[1]>='1' && rotor[1]<='8'
        && rotor[2]>='1' && rotor[2]<='8';
}

bool Widget::isPlugboard(const QString& plugboard) const {
    if(plugboard.size()!=29) return false;
    for(int i=0;i<10;i++){
        if(!plugboard[3*i].isUpper() || !plugboard[3*i+1].isUpper())
            return false;
    }
    return true;
}

QGroupBox* Widget::createFSWBox(){
    QGroupBox *groupBox = new QGroupBox("Activate file system watcher",this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    QRadioButton *radioButton1 = new QRadioButton("FSW");
    QRadioButton *radioButton2 = new QRadioButton("No FSW");
    connect(radioButton1, &QRadioButton::clicked, this, [&](bool){
        watcher->addPath(watcherDir);
        fileMenu->setEnabled(false);
        enigmaMenu->setEnabled(false);
        xxteaMenu->setEnabled(false);
    });
    connect(radioButton2, &QRadioButton::clicked, this, [&](bool){
        watcher->removePath(watcherDir);
        fileMenu->setEnabled(true);
        enigmaMenu->setEnabled(true);
        xxteaMenu->setEnabled(true);
    });
    radioButton2->setChecked(true);
    layout->addWidget(radioButton1);
    layout->addWidget(radioButton2);
    return groupBox;
}

QGroupBox* Widget::createDecodeMenu(){
    QGroupBox *groupBox = new QGroupBox("Decode file",this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    QPushButton *button = new QPushButton(tr("Select file"));
    connect(button, &QPushButton::clicked, this, [&](){
        QString filename = QFileDialog::getOpenFileName(this,"Select file to decode",decodeDir);
        ThreadEncryptor *t = new ThreadEncryptor(service.getEncryptor(),filename,decodeDir,false);
        QThreadPool::globalInstance()->start(t);
    });
    layout->addWidget(button);
    layout->addWidget(new QLabel(decodeDir));
    layout->addWidget(new QPushButton(tr("Decode selected file")));
    groupBox->setLayout(layout);
    return groupBox;
}

QGroupBox* Widget::createEncodeFile(){
    fileMenu = new QGroupBox("Encode file",this);
    QVBoxLayout *layout = new QVBoxLayout(fileMenu);
    QPushButton *button = new QPushButton(tr("Select file"));
    connect(button, &QPushButton::clicked, this, [&](){
        QString file = QFileDialog::getOpenFileName(this,"Select file to encrypt",watcherDir);
        ThreadEncryptor *t = new ThreadEncryptor(service.getEncryptor(),file,decodeDir,true);
        QThreadPool::globalInstance()->start(t);
    });
    layout->addWidget(button);
    layout->addWidget(new QLabel(watcherDir));
    layout->addWidget(new QPushButton(tr("Encode selected file")));
    fileMenu->setLayout(layout);
    return fileMenu;
}

QGroupBox* Widget::createAlgoMenu(){
    QGroupBox *groupBox = new QGroupBox("Algorithm",this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    QRadioButton *radio1 = new QRadioButton(tr("Enigma"));
    QRadioButton *radio2 = new QRadioButton(tr("XXtea"));
    radio1->setChecked(true);
    connect(radio1, &QRadioButton::clicked, this, [&](){
        service.setEncryptor(EncryptorFactory::createEnigma(enigmaKey,enigmaRing,enigmaRotor,enigmaPlugboard));
    });
    connect(radio2, &QRadioButton::clicked, this, [&](){
        service.setEncryptor(EncryptorFactory::createXXTEA(xxteaKey.data(),xxteaIV.data()));
    });
    layout->addWidget(radio1);
    layout->addWidget(radio2);
    groupBox->setLayout(layout);
    return groupBox;
}

QGroupBox* Widget::creatEnigmaMenu(){
    enigmaMenu = new QGroupBox("Enigma encode",this);
    QVBoxLayout *layout = new QVBoxLayout(enigmaMenu);
    QLineEdit *keySetting  = new QLineEdit("AAA");
    QLineEdit *ringSetting = new QLineEdit("AAA");
    QLineEdit *rotor       = new QLineEdit("123");
    QLineEdit *plugBoard   = new QLineEdit("PO ML IU KJ NH YT GB VF RE DC");
    QPushButton *makeChanges = new QPushButton(tr("Make changes"));
    layout->addWidget(new QLabel("Key settings"));
    layout->addWidget(keySetting);
    layout->addWidget(new QLabel("Ring settings"));
    layout->addWidget(ringSetting);
    layout->addWidget(new QLabel("Rotor"));
    layout->addWidget(rotor);
    layout->addWidget(new QLabel("Plugboard settings"));
    layout->addWidget(plugBoard);
    layout->addWidget(makeChanges);
    enigmaMenu->setLayout(layout);
    connect(makeChanges, &QPushButton::clicked, this, [=](){
        if(!isKeyRingSettingsOk(keySetting->text())
            || !isKeyRingSettingsOk(ringSetting->text())
            || !isRotorOk(rotor->text())
            || !isPlugboard(plugBoard->text())){
            QMessageBox::information(this,"Warning","You didn't set valid options");
            return;
        }
        enigmaKey       = keySetting->text();
        enigmaRing      = ringSetting->text();
        enigmaRotor     = rotor->text();
        enigmaPlugboard = plugBoard->text();
        service.setEncryptor(EncryptorFactory::createEnigma(enigmaKey,enigmaRing,enigmaRotor,enigmaPlugboard));
        QMessageBox::information(this,"Success","You changed options for enigma");
    });
    return enigmaMenu;
}

QGroupBox* Widget::createXXteaMenu(){
    xxteaMenu = new QGroupBox("XXtea encode with cfb",this);
    QVBoxLayout *layout = new QVBoxLayout(xxteaMenu);
    QIntValidator *v = new QIntValidator(xxteaMenu);

    QHBoxLayout *kBox = new QHBoxLayout();
    QLineEdit *k1 = new QLineEdit("1"); QLineEdit *k2 = new QLineEdit("2");
    QLineEdit *k3 = new QLineEdit("3"); QLineEdit *k4 = new QLineEdit("4");
    for(auto* e : {k1,k2,k3,k4}) e->setValidator(v);
    kBox->addWidget(new QLabel("K[0]")); kBox->addWidget(k1);
    kBox->addWidget(new QLabel("K[1]")); kBox->addWidget(k2);
    kBox->addWidget(new QLabel("K[2]")); kBox->addWidget(k3);
    kBox->addWidget(new QLabel("K[3]")); kBox->addWidget(k4);

    QHBoxLayout *iBox = new QHBoxLayout();
    QLineEdit *i1 = new QLineEdit("1"); QLineEdit *i2 = new QLineEdit("2");
    QLineEdit *i3 = new QLineEdit("3"); QLineEdit *i4 = new QLineEdit("4");
    for(auto* e : {i1,i2,i3,i4}) e->setValidator(v);
    iBox->addWidget(new QLabel("I[0]")); iBox->addWidget(i1);
    iBox->addWidget(new QLabel("I[1]")); iBox->addWidget(i2);
    iBox->addWidget(new QLabel("I[2]")); iBox->addWidget(i3);
    iBox->addWidget(new QLabel("I[3]")); iBox->addWidget(i4);

    QPushButton *makeChanges = new QPushButton(tr("Make changes"));
    layout->addWidget(new QLabel("Key"));
    layout->addLayout(kBox);
    layout->addWidget(new QLabel("Init"));
    layout->addLayout(iBox);
    layout->addWidget(makeChanges);
    xxteaMenu->setLayout(layout);

    connect(makeChanges, &QPushButton::clicked, this, [=](){
        bool keyOk = k1->text()!="" && k2->text()!="" && k3->text()!="" && k4->text()!="";
        bool ivOk  = i1->text()!="" && i2->text()!="" && i3->text()!="" && i4->text()!="";
        if(keyOk && ivOk){
            xxteaKey = {k1->text().toUInt(),k2->text().toUInt(),k3->text().toUInt(),k4->text().toUInt()};
            xxteaIV  = {i1->text().toUInt(),i2->text().toUInt(),i3->text().toUInt(),i4->text().toUInt()};
            service.setEncryptor(EncryptorFactory::createXXTEA(xxteaKey.data(),xxteaIV.data()));
            QMessageBox::information(this,"Success","You changed XXTEA settings");
        } else {
            QMessageBox::information(this,"Warning","You didn't set valid options");
        }
    });
    return xxteaMenu;
}

QGroupBox* Widget::createSendToServer(){
    QGroupBox *groupBox = new QGroupBox("Send",this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    QLineEdit *ip = new QLineEdit("127.0.0.1");
    QLineEdit *p  = new QLineEdit("4444");
    QPushButton *button = new QPushButton("Send to chosen server");
    layout->addWidget(new QLabel("Send to server"));
    layout->addWidget(ip);
    layout->addWidget(new QLabel("Send to port"));
    layout->addWidget(p);
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, [=](){
        QTcpSocket *client = new QTcpSocket();
        QHostAddress address{ip->text()};
        QString filePath = QFileDialog::getOpenFileName(this,"Select file to send",decodeDir);
        QDataStream stream{client};
        QFile file{filePath};
        TigerHash t;
        client->connectToHost(address, p->text().toInt());
        if(file.open(QFile::ReadOnly)){
            QByteArray fileContent = file.readAll();
            QByteArray hashBytes   = QByteArray::fromStdString(t.hashTiger(fileContent.toStdString()));
            stream << QFileInfo(filePath).fileName();
            stream << (quint64)fileContent.size();
            stream << (quint32)hashBytes.size();
            stream << hashBytes;
            stream << fileContent;
            client->disconnect();
            QMessageBox::information(this,"Sender","You have sent the file "+filePath);
        }
    });
    groupBox->setLayout(layout);
    return groupBox;
}

void Widget::handleConnection(){
    qDebug() << "Added new connection\n";
    QTcpSocket *socket = server->nextPendingConnection();
    QByteArray *buffer = new QByteArray();
    connect(socket, &QTcpSocket::readyRead, this, [=](){
        buffer->append(socket->readAll());
        QDataStream stream(*buffer);
        stream.device()->seek(0);
        QString filename;
        stream >> filename;
        quint64 fileSize;
        quint32 hashSize;
        QByteArray hash, fileContent;
        if(buffer->size() < (int)(sizeof(quint32)+sizeof(quint64)+filename.size()))
            return;
        stream >> fileSize >> hashSize;
        qDebug() << filename << " " << fileSize << " " << hashSize;
        quint64 totalSize = hashSize+fileSize+sizeof(quint64)+sizeof(quint32)+filename.size();
        if(buffer->size() < (int)totalSize)
            return;
        stream >> hash >> fileContent;
        TigerHash t;
        QFile file(decodeDir+"/"+filename);
        if(file.open(QIODevice::WriteOnly) && t.hashTiger(fileContent.toStdString())==hash.toStdString()){
            file.write(fileContent);
            QMessageBox::information(nullptr,"Receiver","You received file");
        }
    });
    connect(socket, &QTcpSocket::disconnected, this, [=](){
        delete buffer;
        socket->disconnect();
    });
}

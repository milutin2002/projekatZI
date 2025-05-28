#include "widget.h"
#include "./ui_widget.h"
#include"SecurityAlgo/tigerhash.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    int port=QInputDialog::getInt(this,"Select port for tcp server","Port",4444,1,65536,1);
    server=new QTcpServer(this);
    QHostAddress address{"127.0.0.1"};
    if(server->listen(address,port)){
        qDebug()<<"Server started listening\n";
    }
    else{
        qDebug()<<"Server failed\n";
    }
    connect(server,&QTcpServer::newConnection,this,&Widget::handleConnection);
    encryptor=new EnigmaFileEncryptor();
    xxteaEncryptor=new XXteaCFB(new uint32_t[]{1,2,3,4},new uint32_t[]{1,2,3,4});
    encryptors=new FileEncryptor*[2];
    encryptors[0]=encryptor;
    encryptors[1]=xxteaEncryptor;
    watcherDir=QFileDialog::getExistingDirectory(this,"Select watch dir x","/home/milutin");
    decodeDir=QFileDialog::getExistingDirectory(this,"Select decode dir x","/home/milutin");
    watcher=new QFileSystemWatcher(this);
    QDir dir(watcherDir);
    files=dir.entryList();
    connect(watcher,&QFileSystemWatcher::directoryChanged,this,[&](const QString& path){
        QDir dir(path);
        QStringList l;
        qDebug()<<"Directory "<<path;
        for(const QString & file : dir.entryList()){
            QFile fileN(path+"/"+file);
            if(!files.contains(fileN.fileName())){
                ThreadEncryptor *t=new ThreadEncryptor(encryptors[selected],path+"/"+file,decodeDir,true);
                QThreadPool::globalInstance()->start(t);
                l.append(file);
                qDebug() << "New file added:" << file;
            }
        }
        files=l;
    });
    QGridLayout *grid=new QGridLayout(this);
    grid->addWidget(createFSWBox(),0,0);
    grid->addWidget(createDecodeMenu(),0,1);
    grid->addWidget(createEncodeFile(),0,2);
    grid->addWidget(createAlgoMenu(),1,0);
    grid->addWidget(creatEnigmaMenu(),1,1);
    grid->addWidget(createXXteaMenu(),1,2);
    grid->addWidget(createSendToServer(),2,0);
    setLayout(grid);
    setWindowTitle(tr("App"));
    resize(480,320);
    ui->setupUi(this);
}

QGroupBox* Widget::createFSWBox(){
    QGroupBox *groupBox = new QGroupBox("Activate file system watcher",this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    QRadioButton *radioButton1 = new QRadioButton("FSW");
    QRadioButton *radioButton2 = new QRadioButton("No FSW");
    connect(radioButton1,&QRadioButton::clicked,this,[&](bool b){
        watcher->addPath(watcherDir);
        fileMenu->setEnabled(false);
        enigmaMenu->setEnabled(false);
        xxteaMenu->setEnabled(false);
    });
    connect(radioButton2,&QRadioButton::clicked,this,[&](bool b){
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

QGroupBox * Widget::createDecodeMenu(){
    QGroupBox *groupBox=new QGroupBox("Decode file",this);
    QVBoxLayout *layout=new QVBoxLayout(groupBox);
    QPushButton * button=new QPushButton(tr("Select file"));
    connect(button,&QPushButton::clicked,this,[&](){
        QString filename=QFileDialog::getOpenFileName(this,"Select file to decode",decodeDir);
        //encryptor->decryptFile(filename);
        //if(selected==0){
            ThreadEncryptor *t=new ThreadEncryptor(encryptors[selected],filename,decodeDir,false);
            QThreadPool::globalInstance()->start(t);
        //}
        /*else{
            QFile file(filename);
            QByteArray data=QByteArray();
            if(file.open(QIODevice::ReadOnly)){
                data=file.readAll();
            }
            CountDown *c=new CountDown(8);
            std::vector<QString>results(8);
            int iter=data.size()/16;
            for(int i=0;i<results.size();i++){
                ThreadXXteaDecryptor *d=new ThreadXXteaDecryptor(results,i,data,c,xxteaEncryptor,iter);
                QThreadPool::globalInstance()->start(d);
            }
             c->wait();
            QString decodeFilename=filename.split("/").back();
            int lastDot=decodeFilename.lastIndexOf(".");
            decodeFilename=decodeFilename.left(lastDot);
            decodeFilename=decodeDir+"/"+decodeFilename;
            QFile fileD{decodeFilename};
            if(fileD.open(QIODevice::WriteOnly)){
                QString res="";
                for (int i = 0; i < results.size(); ++i) {
                    res+=results[i];
                }
                fileD.write(QByteArray::fromStdString(res.toStdString()));
            }
            //QWriteInFile *w=new QWriteInFile(c,results,filename,decodeDir);
            //QThreadPool::globalInstance()->start(w);

        }*/
    });
    QLabel *label=new QLabel(decodeDir);
    QPushButton * decodeButton=new QPushButton(tr("Decode selected file"));
    layout->addWidget(button);
    layout->addWidget(label);
    layout->addWidget(decodeButton);
    groupBox->setLayout(layout);
    return groupBox;
}
QGroupBox *Widget::createEncodeFile(){
    fileMenu=new QGroupBox("Encode file",this);
    QVBoxLayout *layout=new QVBoxLayout(fileMenu);
    QPushButton * button=new QPushButton(tr("Select file"));
    QLabel *label=new QLabel(watcherDir);
    QPushButton * decodeButton=new QPushButton(tr("Encode selected file"));
    layout->addWidget(button);
    layout->addWidget(label);
    layout->addWidget(decodeButton);
    fileMenu->setLayout(layout);
    connect(button,&QPushButton::clicked,this,[&](){
        selectedFileToEncrypt=QFileDialog::getOpenFileName(this,"Select file to encrypt",watcherDir);
        //encryptor->encryptFile(selectedFileToEncrypt,decodeDir);
        ThreadEncryptor *t=new ThreadEncryptor(encryptors[selected],selectedFileToEncrypt,decodeDir,true);
        QThreadPool::globalInstance()->start(t);
    });
    return fileMenu;
}
QGroupBox * Widget::createAlgoMenu(){
    QGroupBox *groupBox=new QGroupBox("Decode file",this);
    QVBoxLayout *layout=new QVBoxLayout(groupBox);
    QPushButton * button=new QPushButton(tr("Select file"));
    QRadioButton *radio1=new QRadioButton(tr("Enigma"));
    QRadioButton *radio2=new QRadioButton(tr("XXtea"));
    radio1->setChecked(true);
    connect(radio1,&QRadioButton::clicked,this,[&](){

        selected=0;
    });
    connect(radio2,&QRadioButton::clicked,this,[&](){
        selected=1;
    });
    layout->addWidget(radio1);
    layout->addWidget(radio2);
    groupBox->setLayout(layout);
    return groupBox;
}
bool Widget::isKeyRingSettingsOk(QString key){
    return key.size()==3 && key[0].isUpper() && key[1].isUpper() && key[2].isUpper();
}
bool Widget::isRotorOk(QString  rotor){
    return rotor.size()==3 && rotor[0]>='1' && rotor[0]<='8'  && rotor[1]>='1' && rotor[1]<='8'  && rotor[2]>='1' && rotor[2]<='8';
}
bool Widget::isPlugboard(QString  plugboard){
    if(plugboard.size()!=29){
        return false;
    }
    for(int i=0;i<10;i++){
        if(!plugboard[3*i].isUpper() || !plugboard[3*i+1].isUpper()){
            return false;
        }
    }
    return true;
}
QGroupBox * Widget::creatEnigmaMenu(){
    enigmaMenu=new QGroupBox("Enigma encode",this);
    QVBoxLayout *layout=new QVBoxLayout(enigmaMenu);
    QLabel *l1=new QLabel("Key settings");
    QLabel *l2=new QLabel("Ring settings");
    QLabel *l3=new QLabel("Rottor");
    QLabel *l4=new QLabel("Plugboard settings");
    QLineEdit *keySetting=new QLineEdit("AAA");
    QLineEdit *ringSetting=new QLineEdit("AAA");
    QLineEdit *rotor=new QLineEdit("123");
    QLineEdit *plugBoard=new QLineEdit("PO ML IU KJ NH YT GB VF RE DC");
    QPushButton *makeChanges=new QPushButton(tr("Make changes"));
    layout->addWidget(l1);
    layout->addWidget(keySetting);
    layout->addWidget(l2);
    layout->addWidget(ringSetting);
    layout->addWidget(l3);
    layout->addWidget(rotor);
    layout->addWidget(l4);
    layout->addWidget(plugBoard);
    layout->addWidget(makeChanges);
    enigmaMenu->setLayout(layout);
    connect(makeChanges,&QPushButton::clicked,this,[=](){
        qDebug()<<keySetting->text()<<" "<<ringSetting->text()<<" "<<rotor->text()<<" "<<plugBoard->text();
        if(!isKeyRingSettingsOk(keySetting->text()) || !isKeyRingSettingsOk(ringSetting->text()) || !isRotorOk(rotor->text()) || !isPlugboard(plugBoard->text())){
            QMessageBox::information(this,"Warning","You didn't set valid options");
            return;
        }
        encryptor->setSettings(keySetting->text(),ringSetting->text(),rotor->text(),plugBoard->text());
         QMessageBox::information(this,"Success","You changed options for enigma");
    });
    return enigmaMenu;
}

QGroupBox *Widget::createXXteaMenu(){
    xxteaMenu=new QGroupBox("XXtea encode with cfb",this);
    QVBoxLayout *layout=new QVBoxLayout(xxteaMenu);
    QLabel *key=new QLabel("Key");
    QHBoxLayout *kBox=new QHBoxLayout();
    QLabel *l1=new QLabel("K[0]");
    QLabel *l2=new QLabel("K[1]");
    QLabel *l3=new QLabel("K[2]");
    QLabel *l4=new QLabel("K[3]");
    QLineEdit *k1=new QLineEdit("1");
    QLineEdit *k2=new QLineEdit("2");
    QLineEdit *k3=new QLineEdit("3");
    QLineEdit *k4=new QLineEdit("4");
    QIntValidator *v=new QIntValidator();
    k1->setValidator(v);
    k2->setValidator(v);
    k3->setValidator(v);
    k4->setValidator(v);
    kBox->addWidget(l1);
    kBox->addWidget(k1);
    kBox->addWidget(l2);
    kBox->addWidget(k2);
    kBox->addWidget(l3);
    kBox->addWidget(k3);
    kBox->addWidget(l4);
    kBox->addWidget(k4);
    QLabel *init=new QLabel("Init");
    QHBoxLayout *iBox=new QHBoxLayout();
    QLabel *li1=new QLabel("I[0]");
    QLabel *li2=new QLabel("I[1]");
    QLabel *li3=new QLabel("I[2]");
    QLabel *li4=new QLabel("I[3]");
    QLineEdit *i1=new QLineEdit("1");
    QLineEdit *i2=new QLineEdit("2");
    QLineEdit *i3=new QLineEdit("3");
    QLineEdit *i4=new QLineEdit("4");
    i1->setValidator(v);
    i2->setValidator(v);
    i3->setValidator(v);
    i4->setValidator(v);
    QPushButton *makeChanges=new QPushButton(tr("Make changes"));
    iBox->addWidget(li1);
    iBox->addWidget(i1);
    iBox->addWidget(li2);
    iBox->addWidget(i2);
    iBox->addWidget(li3);
    iBox->addWidget(i3);
    iBox->addWidget(li4);
    iBox->addWidget(i4);
    layout->addWidget(key);
    layout->addLayout(kBox);
    layout->addWidget(init);
    layout->addLayout(iBox);
    layout->addWidget(makeChanges);
    xxteaMenu->setLayout(layout);
    connect(makeChanges,&QPushButton::clicked,this,[=](){
        if(k1->text()!="" && k2->text()!="" && k3->text()!="" && k4->text()!=""){
            xxteaEncryptor->setK(new uint32_t[]{k1->text().toUInt(),k2->text().toUInt(),k3->text().toUInt(),k4->text().toUInt()});
            QMessageBox::information(this,"Success","You changed key value");
        }
        else{
            QMessageBox::information(this,"Warning","You didn't set valid options for key");
        }
        if(i1->text()!="" && i2->text()!="" && i3->text()!="" && i4->text()!=""){
            xxteaEncryptor->setIV(new uint32_t[]{i1->text().toUInt(),i2->text().toUInt(),i3->text().toUInt(),i4->text().toUInt()});
            QMessageBox::information(this,"Success","You changed iv value");
        }
        else{
            QMessageBox::information(this,"Warning","You didn't set valid options for iv ");
        }
    });
    return xxteaMenu;
}

QGroupBox * Widget::createSendToServer(){
    QGroupBox *groupBox=new QGroupBox("Send",this);
    QVBoxLayout *layout=new QVBoxLayout(groupBox);
    QLabel *ipLabel=new QLabel("Send to server");
    QLineEdit *ip=new QLineEdit("127.0.0.1");
    QLabel *pLabel=new QLabel("Send to port");
    QLineEdit *p=new QLineEdit("4444");
    QPushButton *button=new QPushButton("Send to chosen server");
    layout->addWidget(ipLabel);
    layout->addWidget(ip);
    layout->addWidget(pLabel);
    layout->addWidget(p);
    layout->addWidget(button);
    connect(button,&QPushButton::clicked,this,[=](){
        QTcpSocket *client=new QTcpSocket();
        QHostAddress address{ip->text()};
        QString filePath=QFileDialog::getOpenFileName(this,"Select file to send",decodeDir);
        //NetworkSender *send=new NetworkSender(client,filePath);
        //QThreadPool::globalInstance()->start(send);
        QDataStream stream{client};
        QFile file{filePath};
        TigerHash *t=new TigerHash();
        client->connectToHost(address,p->text().toInt());
        if(file.open(QFile::ReadOnly)){
            QByteArray fileContent=file.readAll();
            std::string hash=t->hashTiger(fileContent.toStdString());
            qDebug()<<hash;
            QByteArray hashBytes=QByteArray::fromStdString(hash);
            qDebug()<<QFileInfo(filePath).fileName();
            stream<<QFileInfo(filePath).fileName();
            quint64 fileSize=fileContent.size();
            quint32 hashSize=hashBytes.size();
            stream<<fileSize;
            stream<<hashSize;
            stream<<hashBytes;
            stream<<fileContent;
            client->disconnect();
        }
    });
    groupBox->setLayout(layout);
    return groupBox;
}

Widget::~Widget()
{
    if(this->encryptor){
        delete encryptor;
        encryptor=nullptr;
    }
    if(this->xxteaEncryptor){
        delete this->xxteaEncryptor;
        xxteaEncryptor=nullptr;
    }
    if(this->encryptors){
        delete []this->encryptors;
        encryptors=nullptr;
    }
    if(this->fileMenu){
        delete this->enigmaMenu;
        enigmaMenu=nullptr;
    }
    if(this->enigmaMenu){
        delete this->xxteaMenu;
        xxteaMenu=nullptr;
    }

    delete ui;
}
void Widget::handleConnection(){
    qDebug()<<"Added new connection\n";
    QTcpSocket *socket=server->nextPendingConnection();
    QByteArray *buffer = new QByteArray();
    connect(socket,&QTcpSocket::readyRead,this,[=](){
        buffer->append(socket->readAll());
        QDataStream stream(*buffer);
            stream.device()->seek(0);
            QString filename;
            stream>>filename;
            quint64 fileSize;
            quint32 hashSize;
            QByteArray hash;
            QByteArray fileContent;
            if(buffer->size()<sizeof(quint32)+sizeof(quint64)+filename.size()){
                return;
            }
            stream>>fileSize;
            stream>>hashSize;
            qDebug()<<filename<<" "<<fileSize<<" "<<hashSize;
            quint64 totalSize=hashSize+fileSize+sizeof(quint64) + sizeof(quint32)+filename.size();
            if(buffer->size()<totalSize){
                return;
            }
            stream>>hash;
            stream>>fileContent;
            TigerHash *t=new TigerHash();
            QFile file(decodeDir+"/"+filename);
            qDebug()<<hashSize<<"\n";
            qDebug()<<"Writing file "<< decodeDir+"/"+filename<<"\n";
            if(file.open(QIODevice::WriteOnly) && t->hashTiger(fileContent.toStdString())==hash.toStdString() ){
                qDebug()<<"Writing file "<< decodeDir+"/"+filename<<"\n";
                file.write(fileContent);
            }
    });
    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        delete buffer;
        socket->disconnect();
    });
}

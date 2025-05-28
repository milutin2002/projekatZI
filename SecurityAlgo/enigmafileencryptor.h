#ifndef ENIGMAFILEENCRYPTOR_H
#define ENIGMAFILEENCRYPTOR_H

#include "symetricfileencryptor.h"

class EnigmaFileEncryptor : public SymetricFileEncryptor
{
private:
    std::string keys[16]={"EKMFLGDQVZNTOWYHXUSPAIBRCJ","AJDKSIRUXBLHWTMCQGZNPYFVOE","BDFHJLCPRTXVZNYEIWGAKMUSQO",
                        "ESOVPZJAYQUIRHXLNFTGKDCMWB","VZBRGITYUPSDNHLXAWMJQOFECK","JPGVOUMFYQBENHZRDKASXLICTW","NZJHGRCXMYSWBOUFAIVLPEKQDT","FKQHTLXOCBJSPDZRAMEWNIUYGV","UWYGADFPVZBECKMTHXSLRINQOJ","AJPCZWRLFBDKOTYUQGENHXMIVS","TAGBPCSDQEUFVNZHYIXJWLRKOM",
                        "HZWVARTNLGUPXQCEJMBSKDYOIF","QCYLXWENFTZOSMVJUDKGIARPHB","SKXQLHCNWARVGMEBJPTYFDZUIO",
                        "QMGYVPEDRCWTIANUXFKZOSLHJB","QJINSAYDVKBFRUHMCPLEWZTGXO"};
    int notch[8][2]={{16,16},{4,4},{21,21},{9,9},{25,25},{25,12},{25,12},{25,12}};
    std::string plugboardSettings="PO ML IU KJ NH YT GB VF RE DA";
    std::string keySettings="AAA";
    std::string ringSettings="AAA";
    std::string rotorSetting="123";
    void swap(char &a,char& b);
    int code(char ch);
    char rotor(char ch,int r,int offset);
    char simpleSub(char ch,std::string key);
    void increaseSettings(int *key,int *r);
    char enigma(char ch,int *key,int *rotors,int *ring,std::string plugboard);

public:
    void setSettings(QString keySettings,QString  ringSettings,QString rotorSettings,QString plugboardSettings){
        this->keySettings=keySettings.toStdString();
        this->ringSettings=ringSettings.toStdString();
        this->rotorSetting=rotorSettings.toStdString();
        this->plugboardSettings=plugboardSettings.toStdString();
    }
    EnigmaFileEncryptor();
    virtual QByteArray encrypt(const QByteArray &data)override;

};

#endif // ENIGMAFILEENCRYPTOR_H

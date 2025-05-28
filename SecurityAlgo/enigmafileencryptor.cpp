#include "enigmafileencryptor.h"

EnigmaFileEncryptor::EnigmaFileEncryptor():SymetricFileEncryptor(".enigma") {}

int EnigmaFileEncryptor::code(char ch){
    return toupper(ch)-'A';
}
char EnigmaFileEncryptor::rotor(char ch,int r,int offset){
    int cod=(code(ch)+26+offset)%26;
    return (code(keys[r][cod])+26-offset)%26+'A';
}
char EnigmaFileEncryptor::simpleSub(char ch,std::string key){
    return key[code(ch)];
}
void EnigmaFileEncryptor::increaseSettings(int *key,int *r){
    if(key[1]==notch[r[1]][0] || key[1]==notch[r[1]][1]) {
        key[0]=(key[0]+1)%26;
        key[1]=(key[1]+1)%26;
    }
    if(key[2]==notch[r[2]][0] || key[2]==notch[r[2]][1]) {
        key[1]=(key[1]+1)%26;
    }
    key[2]=(key[2]+1)%26;
}
char EnigmaFileEncryptor::enigma(char ch,int *key,int *rotors,int *ring,std::string plugboard){
    ch=simpleSub(ch,plugboard);
    ch=rotor(ch,rotors[2],key[2]-ring[2]);
    ch=rotor(ch,rotors[1],key[1]-ring[1]);
    ch=rotor(ch,rotors[0],key[0]-ring[0]);
    ch=simpleSub(ch,"YRUHQSLDPXNGOKMIEBFZCWVJAT");
    ch=rotor(ch,rotors[0]+8,key[0]-ring[0]);
    ch=rotor(ch,rotors[1]+8,key[1]-ring[1]);
    ch=rotor(ch,rotors[2]+8,key[2]-ring[2]);
    ch=simpleSub(ch,plugboard);
    return ch;
}

void EnigmaFileEncryptor::swap(char &a,char &b){
    char t=a;
    a=b;
    b=t;
}
QByteArray EnigmaFileEncryptor::encrypt(const QByteArray&data){
    std::string str=std::string(data.data(),data.size());
    int key[3]{code(keySettings[0]),code(keySettings[1]),code(keySettings[2]) };
    int rotor[3]{rotorSetting[0]-'1',rotorSetting[1]-'1',rotorSetting[2]-'1'};
    int ring[3]={code(ringSettings[0]),code(ringSettings[1]),code(ringSettings[2]) };
    std::string plugboard="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i=0,j=1;i<plugboardSettings.size();i+=3,j+=3) {
        int ichar=plugboard.find(plugboardSettings[i]);
        int jchar=plugboard.find(plugboardSettings[j]);
        swap(plugboard[ichar],plugboard[jchar]);
    }
    std::string cipherText="";
    for(int i=0;i<str.size();i++) {
        if(str[i]>='A' && str[i]<='Z' || str[i]>='a' && str[i]<='z') {
            increaseSettings(key,rotor);
            char ch=enigma(str[i],key,rotor,ring,plugboard);
            if(str[i]>='a' && str[i]<='z') {
                ch=tolower(ch);
            }
            cipherText+=ch;
        }
        else {
            cipherText+=str[i];
        }
    }
    return QByteArray(cipherText.c_str(),static_cast<int>(str.size()));

}

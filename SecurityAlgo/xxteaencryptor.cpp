#include "xxteaencryptor.h"

XXteaEncryptor::XXteaEncryptor(uint32_t *k):FileEncryptor(".xxtea"),k(k) {}

void XXteaEncryptor::encryptXXtea(uint32_t *a,uint32_t n){
    if(n<1) {
        return;
    }
    uint32_t z=a[n-1],y,sum=0,e,i;
    uint32_t q=6+52/n;
    while(q--) {
        sum+=DELTA;
        e=(sum>>2)&3;
        for(i=0;i<n-1;i++) {
            y=a[i+1];
            z=a[i]+=MX;
        }
        y=a[0];
        z=a[n-1]+=MX;
    }
}

void XXteaEncryptor::decryptXXtea(uint32_t *a,uint32_t n){
    if(n<1) {
        return;
    }
    uint32_t i,z=a[n-1],y=a[0],sum=0,e;
    uint32_t q=6+52/n;
    sum=q*DELTA;
    while(q--) {
        e=(sum>>2)&3;
        for(i=n-1;i>0;i--) {
            z=a[i-1];
            y=a[i]-=MX;
        }
        z=a[n-1];
        y=a[0]-=MX;
        sum-=DELTA;
    }
}
QByteArray XXteaEncryptor::encrypt(const QByteArray &data){
    std::string s=data.toStdString();
    encryptXXtea((uint32_t*)&s[0],s.size()/4);
    return QByteArray::fromStdString(s);
}

QByteArray XXteaEncryptor::decrypt(const QByteArray &data){
    std::string s=data.toStdString();
    decryptXXtea((uint32_t*)&s[0],s.size()/4);
    return QByteArray::fromStdString(s);
}

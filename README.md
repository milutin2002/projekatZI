# 🔐 ZastitaInformacijaProjekat

A Qt-based C++ desktop application for encrypting and decrypting files using symmetric algorithms and custom-built encryption logic. Developed as part of an academic project focused on information protection.

---

## 📌 Features

- 🧱 Block-based file encryption and decryption
- 🔐 Multiple algorithms:
  - Custom symmetric encryption
  - Enigma-like cipher
  - XXTEA and CFB mode variants
- ⚡ Multi-threaded encryption support
- 📁 Simple and responsive Qt GUI
- 📤 Network send/receive logic
- 🐯 Optional Tiger Hash integration for hashing files

---

## 🛠️ Technologies Used

- **C++17**
- **Qt  Qt 6** (Widgets + Network)
- **CMake ≥ 3.16**

---

## 📁 Project Structure
```
.
├── CMakeCache.txt
├── cmake_install.cmake
├── CMakeLists.txt
├── .gitignore
├── main.cpp
├── Makefile
├── projekatZI_autogen
│   ├── deps
│   ├── EWIEGA46WW
│   │   ├── moc_widget.cpp
│   │   └── moc_widget.cpp.d
│   ├── include
│   │   └── ui_widget.h
│   ├── moc_predefs.h
│   ├── mocs_compilation.cpp
│   └── timestamp
├── .qt
│   ├── QtDeploySupport.cmake
│   └── QtDeployTargets.cmake
├── README.md
├── SecurityAlgo
│   ├── enigmafileencryptor.cpp
│   ├── enigmafileencryptor.h
│   ├── fileencryptor.cpp
│   ├── fileencryptor.h
│   ├── symetricfileencryptor.cpp
│   ├── symetricfileencryptor.h
│   ├── tigerhash.cpp
│   ├── tigerhash.h
│   ├── xxteacfb.cpp
│   ├── xxteacfb.h
│   ├── xxteaencryptor.cpp
│   └── xxteaencryptor.h
├── Threads
│   ├── threadencryptor.cpp
│   └── threadencryptor.h
├── widget.cpp
├── widget.h
└── widget.ui

7 directories, 33 files
```

---

## 📐 UML Class Diagram

```mermaid
classDiagram
    class FileEncryptor {
        <<abstract>>
        -QString ext
        +FileEncryptor()
        +FileEncryptor(ext: QString)
        +encrypt(data: QByteArray) QByteArray*
        +decrypt(data: QByteArray) QByteArray*
        +encryptFile(file: QString, decodeDir: QString) void
        +decryptFile(file: QString) void
        +getExt() QString
    }

    class SymetricFileEncryptor {
        <<abstract>>
        +SymetricFileEncryptor()
        +SymetricFileEncryptor(ext: QString)
        +encrypt(data: QByteArray) QByteArray*
        +decrypt(data: QByteArray) QByteArray
    }

    class EnigmaFileEncryptor {
        -keys: string[16]
        -notch: int[8][2]
        -plugboardSettings: string
        -keySettings: string
        -ringSettings: string
        -rotorSetting: string
        +EnigmaFileEncryptor()
        +setSettings(key: QString, ring: QString, rotor: QString, plugboard: QString) void
        +encrypt(data: QByteArray) QByteArray
        +decrypt(data: QByteArray) QByteArray
    }

    class XXteaEncryptor {
        #k: uint32_t*
        +XXteaEncryptor(k: uint32_t*)
        +encrypt(data: QByteArray) QByteArray
        +decrypt(data: QByteArray) QByteArray
        +setK(k: uint32_t*) void
        -encryptXXtea(a: uint32_t*, n: uint32_t) void
        -decryptXXtea(a: uint32_t*, n: uint32_t) void
    }

    class XXteaCFB {
        -iv: uint32_t*
        +XXteaCFB(k: uint32_t*, iv: uint32_t*)
        +encrypt(data: QByteArray) QByteArray
        +decrypt(data: QByteArray) QByteArray
        +setIV(iv: uint32_t*) void
        +decryptPart(data: QByteArray, i: int, count: int) QString
    }

    class TigerHash {
        -h0: uint64_t
        -h1: uint64_t
        -h2: uint64_t
    }

    class EncryptorFactory {
        <<factory>>
        +createEnigma(key: QString, ring: QString, rotor: QString, plugboard: QString)$ unique_ptr~FileEncryptor~
        +createXXTEA(key: uint32_t*, iv: uint32_t*)$ unique_ptr~FileEncryptor~
    }

    class EncryptService {
        -encryptor: unique_ptr~FileEncryptor~
        +EncryptService()
        +setEncryptor(e: unique_ptr~FileEncryptor~) void
        +getEncryptor() FileEncryptor*
        +encryptFile(file: QString, decodeDir: QString) void
        +decryptFile(file: QString) void
        +updateEnigmaSettings(key: QString, ring: QString, rotor: QString, plugboard: QString) void
        +updateXXteaSettings(key: uint32_t*, iv: uint32_t*) void
    }

    class FileService {
        +FileService()
        +readAllTheText(filename: QString)$ QByteArray
        +writeAllTheText(filename: QString, data: QByteArray)$ void
    }

    class ThreadEncryptor {
        -f: FileEncryptor*
        -filename: QString
        -decodeDir: QString
        -encrypt: bool
        +ThreadEncryptor(f: FileEncryptor*, filename: QString, decodeDir: QString, encrypt: bool)
        +run() void
    }

    class QRunnable {
        <<Qt>>
    }

    class QWidget {
        <<Qt>>
    }

    class QTcpServer {
        <<Qt>>
    }

    class QFileSystemWatcher {
        <<Qt>>
    }

    class Widget {
        -watcherDir: QString
        -decodeDir: QString
        -watcher: QFileSystemWatcher*
        -files: QStringList
        -service: EncryptService
        -server: QTcpServer*
        -xxteaKey: array~uint32_t,4~
        -xxteaIV: array~uint32_t,4~
        -enigmaKey: QString
        -enigmaRing: QString
        -enigmaRotor: QString
        -enigmaPlugboard: QString
        +Widget(parent: QWidget*)
        +handleConnection() void
        -isKeyRingSettingsOk(key: QString) bool
        -isRotorOk(rotor: QString) bool
        -isPlugboard(plugboard: QString) bool
    }

    FileEncryptor <|-- SymetricFileEncryptor
    FileEncryptor <|-- EnigmaFileEncryptor
    FileEncryptor <|-- XXteaEncryptor
    XXteaEncryptor <|-- XXteaCFB
    QRunnable <|-- ThreadEncryptor
    QWidget <|-- Widget

    EncryptService o-- FileEncryptor : owns (unique_ptr)
    EncryptService ..> FileService : uses
    EncryptorFactory ..> EnigmaFileEncryptor : creates
    EncryptorFactory ..> XXteaCFB : creates
    ThreadEncryptor --> FileEncryptor : uses
    Widget *-- EncryptService
    Widget --> QTcpServer : owns
    Widget --> QFileSystemWatcher : owns
    Widget ..> EncryptorFactory : uses
    Widget ..> ThreadEncryptor : creates
    Widget ..> TigerHash : uses
```

> Note: `EnigmaFileEncryptor` inherits directly from `FileEncryptor` (not from `SymetricFileEncryptor`, which is currently unused by the rest of the codebase but kept as a reusable base for future symmetric ciphers).

---

🧰 Troubleshooting
If you encounter errors such as:

Qt6 not found

QWidget: No such file or directory

Could NOT find Qt6 (missing: Qt6Widgets)

You likely need to install the required Qt and build tools.

🐧 On Arch/Manjaro:
bash
Copy
Edit
```
sudo pacman -S qt6-base cmake g++
```
🐧 On Ubuntu/Debian:
bash
Copy
Edit
```
sudo apt update
sudo apt install qt6-base-dev cmake g++
```
💡 Tip: If you have both Qt5 and Qt6 installed, CMake will usually find Qt6 by default. You can force a specific version using -DQt6_DIR=....
```
mkdir build && cd build
cmake ..
cmake --build .
```
<h1>Screenshoots</h1>
Choose on which port will server listen on
<img src="https://github.com/milutin2002/projekatZI/blob/main/Images/startServer.png?raw=true">

Choose location where your file watcher will watch for new files
<img src="https://github.com/milutin2002/projekatZI/blob/main/Images/watchDirSel.png?raw=true">

Choose where decrypted files will be stored after decryption
<img src="https://github.com/milutin2002/projekatZI/blob/main/Images/decodeDirSel.png?raw=true">

Main window
<img src="https://github.com/milutin2002/projekatZI/blob/main/Images/mainWindow.png?raw=true">

Encryption
<img src="https://github.com/milutin2002/projekatZI/blob/main/Images/encoding.gif?raw=true">

Decryption
<img src="https://github.com/milutin2002/projekatZI/blob/main/Images/decrypt.gif?raw=true">

File watcher
<img src="https://github.com/milutin2002/projekatZI/blob/main/Images/watcher.gif?raw=true">

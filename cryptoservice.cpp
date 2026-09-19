#include "cryptoservice.h"
#include <QFile>
#include <QProcess>

namespace crypto{

Result genKeys(const QString &keyDir){
    // --- Шаг 1: генерируем закрытый ключ (RSA 2048) ---
    QProcess genPrivate;
    genPrivate.start("openssl", QStringList()
                                    << "genpkey"
                                    << "-algorithm" << "RSA"
                                    << "-out" << keyDir + "/private_key.pem"
                                    << "-pkeyopt" << "rsa_keygen_bits:2048"
                     );
    genPrivate.waitForFinished(-1);  // ждём завершения

    if (genPrivate.exitCode() != 0) {
        return {false,"Faild to generate private key"};
    }

    // --- Шаг 2: извлекаем открытый ключ из закрытого ---
    QProcess genPublic;
    genPublic.start("openssl", QStringList()
                                   << "rsa"
                                             << "-in"  << keyDir + "/private_key.pem"
                                   << "-pubout"
                                             << "-out" << keyDir + "/public_key.pem"
                    );
    genPublic.waitForFinished(-1);

    if (genPublic.exitCode() != 0)
        return {false,"Faild to generate public key"};
    return {true,"Ok"};
}

Result signFile(const QString &filePath, const QString &keyPath){
    // Пути (при необходимости поменяй)
    QString signaturePath = filePath + ".sig";      // подпись рядом с файлом

    QProcess sign;
    sign.start("openssl", QStringList()
        << "dgst"
        << "-sha256"
        << "-sign" << keyPath
        << "-out"  << signaturePath
        << filePath);
    sign.waitForFinished(-1);

    if (sign.exitCode() != 0)
        return {false,"Error: "+ sign.readAllStandardError()};

    return {true,"Ok"};
}

Result verifyFile(const QString &filePath, const QString &keyPath){

    QString signaturePath  = filePath + ".sig";
    QProcess verify;
    verify.start("openssl", QStringList()
                                << "dgst"
                                << "-sha256"
                                << "-verify" << keyPath
                                << "-signature" << signaturePath
                                << filePath
                 );
    verify.waitForFinished(-1);

    QString out = verify.readAllStandardOutput();
    QString err = verify.readAllStandardError();

    if (verify.exitCode() == 0 && out.contains("Verified OK"))
        return {true,"Ok"};
    return {false,"Error: "+err};
}




}
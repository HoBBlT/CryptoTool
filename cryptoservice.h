#ifndef CRYPTOSERVICE_H
#define CRYPTOSERVICE_H

#include <QString>

struct Result {
    bool ok = false;
    QString message;
};

namespace crypto{

Result genKeys(const QString &filePath);

Result signFile(const QString &filePath, const QString &keyPath);

Result verifyFile(const QString &filePath, const QString &keyPath);


}
#endif // CRYPTOSERVICE_H

#ifndef ENCRYPTIONHELPER_H
#define ENCRYPTIONHELPER_H

#include <QString>

class EncryptionHelper
{
public:
    static QString encrypt(const QString &text);
    static QString decrypt(const QString &text);

private:
    EncryptionHelper() = default;
};

#endif // ENCRYPTIONHELPER_H

#include "encryptionhelper.h"

QString EncryptionHelper::encrypt(const QString &text)
{
    QString encryptedText;
    for (const QChar &ch : text) {
        encryptedText.append(QChar(ch.toLatin1() + 1)); // Shift characters by 1
    }
    return encryptedText;
}

QString EncryptionHelper::decrypt(const QString &text)
{
    QString decryptedText;
    for (const QChar &ch : text) {
        decryptedText.append(QChar(ch.toLatin1() - 1)); // Shift characters back by 1
    }
    return decryptedText;
}

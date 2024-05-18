#ifndef USERVALIDATION_H
#define USERVALIDATION_H

#include <QString>
#include <QMessageBox>

class UserValidation
{
public:
    UserValidation(); // Constructor declaration

    static bool validateUserData(const QString &username, const QString &password, const QString &email, const QString &number);
};

#endif // USERVALIDATION_H


#include "uservalidation.h"

UserValidation::UserValidation() {}

bool UserValidation::validateUserData(const QString &username, const QString &password, const QString &email, const QString &number)
{
    // Validate username
    if (username.isEmpty()) {
        QMessageBox::warning(nullptr, "Registration", "Username cannot be empty.");
        return false;
    }

    // Validate password
    if (password.isEmpty()) {
        QMessageBox::warning(nullptr, "Registration", "Password cannot be empty.");
        return false;
    }

    if (password.contains(" ")) {
        QMessageBox::warning(nullptr, "Registration", "Password cannot contain spaces.");
        return false;
    }

    // Validate email
    if (!email.contains("@")) {
        QMessageBox::warning(nullptr, "Registration", "Invalid email format.");
        return false;
    }

    // Validate number
    if (number.length() != 10 || !number.toInt()) {
        QMessageBox::warning(nullptr, "Registration", "Invalid number format. Must be 10 digits long.");
        return false;
    }
    return true;
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "EncryptionHelper.h"
#include "uservalidation.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCheckBox>
#include <QIcon>
#include <QSize>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
     ui->stackedWidget->setCurrentIndex(0);
    ui->checkBox->setIcon(QIcon(":/img/on.png.png"));
    ui->checkBox_2->setIcon(QIcon(":/img/off.png.png"));
    ui->checkBox_2->setIconSize(QSize(100,50));
    ui->checkBox->setIconSize(QSize(100,50));
    loadUsers();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{

}
void MainWindow::loadUsers()
{
    QFile file("userdata.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // Read the file line by line until the end
        while (!in.atEnd()) {
            QString line = in.readLine();
            QString username;
            QString password;
            // Find the index of the first colon (':') in the line
            int index1 = line.indexOf(':');
            if (index1 != -1) {
                username = line.left(index1);
                int index2 = line.indexOf(':', index1 + 1);
                if (index2 != -1) {
                    password = line.mid(index1 + 1, index2 - index1 - 1);



                }
            }
            // Insert the username-password pair into the users data structure
            users.insert(username, password);
            file.close();
        }
    }
    else {
        qDebug() << "Failed to open userdata.txt for reading";
    }
}

void MainWindow::on_loginButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    UserPassword = ui->passwordLineEdit->text().trimmed();
    loadUsers();
    if (users.contains(username)) {
        // Decrypt password stored in QMAP
        QString storedPassword= decrypt(users.value(username,UserPassword));
        // Compare
        if (storedPassword == UserPassword) {
            currentUser = username;
            // Check if the login successful message has been shown before
            if (!loginMessageShown) {
                QMessageBox::information(this, "Login", "Login successful!");
                loginMessageShown = true;
            }
            openPasswordForm();
            return;
        }
    }
    QMessageBox::warning(this, "Login", "Invalid username or password");
}


void MainWindow::on_registerButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::saveUsers()
{
    //Load file
    QFile file("userdata.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        // Iterate over the key-value pairs in the users map
        for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
            //Write, separated by a colon (':')
            out << it.key() << ":" << it.value() << "\n";
        }
        file.close();
    } else {
        qDebug() << "Failed to open userdata.txt for writing";
    }
}
//Encryption
QString MainWindow::encrypt(const QString &text)
{
    return EncryptionHelper::encrypt(text);
}
//Decryption
QString MainWindow::decrypt(const QString &text)
{
    return EncryptionHelper::decrypt(text);
}

//Set boolean to false
bool registrationMessageShown = false;


void MainWindow::on_newRegister_clicked()
{
    QString username = ui->usernameLineEdit_3->text();
    QString password = ui->passwordLineEdit_3->text();
    QString email = ui->EmailLineEdit->text();
    QString number = ui->numberLineEdit->text();

    // Validate user data
    if (UserValidation::validateUserData(username, password, email, number)==true){
        if (!users.contains(username)) {
            QString encryptedPassword = encrypt(password);
            QString encryptedEmail = encrypt(email);
            QString encryptedNumber = encrypt(number);
            users.insert(username, encryptedPassword);
            users.insert(username, encrypt(password) + ":" + encrypt(email) + ":" + encrypt(number));
            saveUsers();
            ui->usernameLineEdit->clear();
            ui->passwordLineEdit->clear();
            if (!registrationMessageShown) {
                QMessageBox::information(this, "Registration", "User registered successfully!");
                ui->usernameLineEdit_3->clear();
                ui->passwordLineEdit_3->clear();
                ui->EmailLineEdit->clear();
                ui->usernameLineEdit->clear();
                registrationMessageShown = true;
            }
            ui->stackedWidget->setCurrentIndex(0);
        } else {
            QMessageBox::warning(this, "Registration", "Username already exists");
        }
        ;
    }
}

void MainWindow::on_returnButton_clicked(){
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_ChangePasswordBtn_clicked(){
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_changePasswordButton_clicked(){
    QString newPassword= ui->newPasswordLineEdit->text();
    QString ConfirmPassword=ui->confirmNewPasswordLineEdit->text();
    QString username=ui->usernameLineEdit->text();

    QFile file("userdata.txt");
    // Validate the new password and confirm password using a validation function
    if ((UserValidation::validateUserData("Checked",newPassword,"Checked@","1234567890")==false)||(UserValidation::validateUserData("Checked",ConfirmPassword,"Checked@","1234567890")==false)){
        return;}
    if (newPassword!=ConfirmPassword){
        QMessageBox::warning(this, "Password Change", "Please enter the new password in both fields");
        return; }
    if (newPassword==UserPassword){
        QMessageBox::warning(this, "Password Change", "New password can not be the same as old password");
        return; } else{
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Failed to open userdata.txt for reading and writing";
            return;
        }

        // Read all lines and update the password if the username matches
        QTextStream in(&file);
        QStringList lines;
        QString EncryptPassword=encrypt(newPassword);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(':');
            if (parts.size() >= 2 && parts[0] == username) {
                parts[1] = EncryptPassword;
                line = parts.join(":");

                // Show success message and clear input fields
                QMessageBox::information(this, "Succes", "Passowrd changed successfully!");
                 ui->stackedWidget->setCurrentIndex(2);
                ui->newPasswordLineEdit->clear();
                 ui->confirmNewPasswordLineEdit->clear();

            }
            lines.append(line);
        }

        file.resize(0);
        QTextStream out(&file);
        for (const QString &line : lines) {
            out << line << "\n";
        }}


    file.close();
}

void MainWindow::openPasswordForm()
{
    ui->stackedWidget->setCurrentIndex(2);
    setUsername(currentUser);
    ui->websiteLineEdit->setEnabled(true);
    ui->usernameLineEdit_2->setEnabled(true);
    ui->passwordLineEdit_2->setEnabled(true);
    ui->websiteLineEdit->clear();
    ui->usernameLineEdit_2->clear();
    ui->passwordLineEdit_2->clear();
}



bool addPasswordMessageShown = false;
bool deletePasswordMessageShown = false;



void MainWindow::on_addPasswordButton_clicked()
{
    QString website = encrypt(ui->websiteLineEdit->text());
    QString username = encrypt(ui->usernameLineEdit_2->text());
    QString password = encrypt(ui->passwordLineEdit_2->text());
    //Verify input
    if (website.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields.");


        return;
    }

    // Check if password already exists for the entered website, username, and password
    if (passwordExists(website, username, password)) {
        QMessageBox::warning(this, "Error", "Password already exists.");

        return;
    }
    savePassword(website, username, password);


}


void MainWindow::on_deletePasswordButton_clicked()
{
    //Verify row selected
    int row = ui->passwordTableWidget->currentRow();
    if (row == -1) {
        if (!deletePasswordMessageShown) {
            QMessageBox::warning(this, "Error", "Please select a password to delete.");
            deletePasswordMessageShown = true;
        }
        return;
    }
    QString website =encrypt(ui->passwordTableWidget->item(row, 0)->text());
    QString username = encrypt(ui->passwordTableWidget->item(row, 1)->text());
    QString password = encrypt(ui->passwordTableWidget->item(row, 2)->text());

    deletePassword(website, username, password);
}



void MainWindow::setUsername(const QString &username)
{
    m_username = username;
    ui->usernameLabel->setText("Welcome, " + username + "!");
    loadPasswords();
}

void MainWindow::loadPasswords()
{
    ui->passwordTableWidget->clearContents();
    ui->passwordTableWidget->setRowCount(0);
    // Open the passwords file
    QFile file("passwords.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open passwords file.");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        // Split the line into parts based on colon (':') separator
        QStringList parts = line.split(':');
        if (parts.size() == 4 && parts[0] == m_username) {
            int row = ui->passwordTableWidget->rowCount();
            ui->passwordTableWidget->insertRow(row);
            // Decrypt and set the website, username, and password into the table widget
            QString decryptedwebsite = decrypt(parts[1]);
            ui->passwordTableWidget->setItem(row, 0, new QTableWidgetItem(decryptedwebsite));
            QString decryptedUsername = decrypt(parts[2]);
            ui->passwordTableWidget->setItem(row, 1, new QTableWidgetItem(decryptedUsername));
            QString decryptedPassword = decrypt(parts[3]);
            ui->passwordTableWidget->setItem(row, 2, new QTableWidgetItem(decryptedPassword));
        }
    }

    file.close();
}

void MainWindow::savePassword(const QString &website, const QString &username, const QString &password)
{
    QFile file("passwords.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open passwords file for writing.");
        return;
    }

    QString encryptedPassword = password;
    QString encryptedWebsite = website;
    QString encryptedUsername = username;
    QTextStream out(&file);
    //Output to file
    out << m_username << ":" << website << ":" << username << ":" << password << "\n";
    file.close();

    loadPasswords();
}

void MainWindow::deletePassword(const QString &website, const QString &username, const QString &password)
{
    QFile file("passwords.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open passwords file.");
        return;
    }

    QTextStream in(&file);
    QString content;
    //Set boolean to false
    bool passwordDeleted = false;
    // Read each line from the file
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(':');
        // Check if the line matches the criteria for deletion
        if (parts.size() == 4 && parts[0] == m_username &&
            parts[1] == website && parts[2] == username) {

            passwordDeleted = true;
            continue;

        }
        content += line + "\n";
    }
    file.resize(0);
    QTextStream out(&file);
    out << content;
    file.close();

    if (passwordDeleted) {
        QMessageBox::information(this, "Success", "Password deleted successfully.");
        loadPasswords();
    } else {
        QMessageBox::warning(this, "Error", "Password not found.");
    }
}



bool MainWindow::passwordExists(const QString &website, const QString &username, const QString &password)
{
    QFile file("passwords.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    // Read the content of the file
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(':');
        //Break up input into a particular format
        if (parts.size() == 4 && parts[0] == m_username &&
            parts[1] == website && parts[2] == username) {
            file.close();
            return true;

        }
    }

    file.close();
    return false;
}



void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(false);
    ui->checkBox->setIcon(QIcon(":/img/on.png.png"));
    ui->checkBox_2->setIcon(QIcon(":/img/off.png.png"));
    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->checkBox->setIcon(QIcon(":/img/on.png.png"));
    ui->checkBox_2->setIcon(QIcon(":/img/off.png.png"));
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(false);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::filterPasswords(const QString &searchText)
{
    //Set text to lower case
    QString search = searchText.toLower();
    //Store count of rows
    int rowCount = ui->passwordTableWidget->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        QString website = ui->passwordTableWidget->item(i, 0)->text().toLower();
        QString username = ui->passwordTableWidget->item(i, 1)->text().toLower();
        QString password = ui->passwordTableWidget->item(i, 2)->text().toLower();
        //Check if contains the search term
        bool match = website.contains(search) || username.contains(search) || password.contains(search);
        //Set the visibility of the row
        ui->passwordTableWidget->setRowHidden(i, !match);
    }
}

void MainWindow::on_searchBar_textChanged(const QString &arg1)
{
    filterPasswords(ui->searchBar->text());

}

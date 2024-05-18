#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_ChangePasswordBtn_clicked();
    void on_addPasswordButton_clicked();
    void on_deletePasswordButton_clicked();
    void on_newRegister_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_checkBox_2_stateChanged(int arg1);
    void filterPasswords(const QString &searchText);
    void on_changePasswordButton_clicked();
    void on_searchBar_textChanged(const QString &arg1);
    void on_returnButton_clicked();

private:
    void setupUI();
    void loadUsers();
    void saveUsers();
    QString encrypt(const QString &text);
    QString decrypt(const QString &text);
    void openPasswordForm();
    void setUsername(const QString &username);
    void loadPasswords();
    void savePassword(const QString &website, const QString &username, const QString &password);
    void deletePassword(const QString &website, const QString &username, const QString &password);
    bool passwordExists(const QString &website, const QString &username, const QString &password);
    bool validateUserData(const QString &username, const QString &password, const QString &email, const QString &number);

    Ui::MainWindow *ui;
    QString m_username;
    QString currentUser;
    QMap<QString, QString> users;
    bool loginMessageShown = false;
    bool deletePasswordMessageShown = false;
    QString UserPassword;

};

#endif // MAINWINDOW_H

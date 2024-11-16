#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class AuthWindow : public QWidget {
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);

signals:
    void authenticated();

private slots:
    void handleLogin();
    void handleRegister();

private:
    bool checkCredentials(const QString &username, const QString &password = QString());
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *registerButton;
};

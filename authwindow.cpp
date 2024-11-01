#include "authwindow.h"
#include "mainwindow.h"
#include <QMessageBox>

AuthWindow::AuthWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Админ-авторизация");
    setMinimumSize(300, 200);

    auto *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Вход администратора");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Логин");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Пароль");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    loginButton = new QPushButton("Войти", this);
    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::handleLogin);
    layout->addWidget(loginButton);

    setLayout(layout);
}

void AuthWindow::handleLogin() {
    // Заглушка проверки логина и пароля
    if (usernameInput->text() == "admin" && passwordInput->text() == "password") {
        auto *mainWindow = new MainWindow();
        mainWindow->show();
        hide();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}

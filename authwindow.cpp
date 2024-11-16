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

    QString style = R"(
        QWidget {
            background-color: #f8f8f8;
            color: #333;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            font-size: 14px;
        }
        QLabel {
            font-size: 16px;
            font-weight: bold;
            color: #007AFF;
        }
        QLineEdit {
            border: 1px solid #d0d0d0;
            border-radius: 8px;
            padding: 6px 10px;
            background-color: #ffffff;
            color: #333;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 1px solid #007AFF;
            outline: none;
        }
        QPushButton {
            background-color: #007AFF;
            border: none;
            border-radius: 8px;
            color: #ffffff;
            font-size: 14px;
            padding: 8px 12px;
        }
        QPushButton:hover {
            background-color: #005ecb;
        }
        QPushButton:pressed {
            background-color: #004ba0;
        }
    )";
    setStyleSheet(style);
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

// authwindow.h

#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

class QLineEdit;
class QPushButton;

class AuthWindow : public QWidget {
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);
    QString getAuthToken();

private slots:
    void handleLogin();                      // Слот для обработки нажатия кнопки "Войти"
    void checkCredentials(const QString &username, const QString &password); // Метод для отправки запроса на сервер
    void loginFailed(const QString &errorMessage);
    void loginSuccess();

signals:
    void onloginSuccess();                           // Сигнал при успешной аутентификации
    void onloginFailed(const QString &errorMessage); // Сигнал при неудачной аутентификации

private:
    QLineEdit *usernameInput;      // Поле ввода логина
    QLineEdit *passwordInput;      // Поле ввода пароля
    QPushButton *loginButton;      // Кнопка "Войти"

    QNetworkAccessManager *manager; // Менеджер для сетевых запросов
    QString authToken;             // Переменная для хранения токена
};

#endif // AUTHWINDOW_H

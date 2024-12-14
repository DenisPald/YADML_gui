// authwindow.cpp

#include "authwindow.h"
#include "mainwindow.h" // Убедитесь, что этот заголовочный файл существует и корректен
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

AuthWindow::AuthWindow(QWidget *parent)
    : QWidget(parent), manager(new QNetworkAccessManager(this)), authToken("") {

    setWindowTitle("Админ-авторизация");
    setMinimumSize(300, 200);

    // Создаем основной вертикальный макет
    auto *layout = new QVBoxLayout(this);

    // Заголовок окна
    QLabel *titleLabel = new QLabel("Вход администратора");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    // Поле ввода логина
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Логин");
    layout->addWidget(usernameInput);

    // Поле ввода пароля
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Пароль");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    // Кнопка "Войти"
    loginButton = new QPushButton("Войти", this);
    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::handleLogin);
    layout->addWidget(loginButton);

    // Устанавливаем макет для окна
    setLayout(layout);

    connect(this, &AuthWindow::onloginSuccess, this, &AuthWindow::loginSuccess);
    connect(this, &AuthWindow::onloginFailed, this, &AuthWindow::loginFailed);
}

QString AuthWindow::getAuthToken() {
    return authToken;
}

void AuthWindow::handleLogin() {
    QString username = usernameInput->text().trimmed();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите логин и пароль.");
        return;
    }

    // Отправляем запрос на проверку учетных данных
    checkCredentials(username, password);
}

void AuthWindow::checkCredentials(const QString &username, const QString &password) {
    QUrl url("http://192.168.31.88:8000/auth"); // Замените на ваш реальный URL
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Формируем JSON с учетными данными
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    qDebug() << "Отправка запроса на аутентификацию:" << QString(data);

    // Отправляем POST-запрос
    QNetworkReply *reply = manager->post(request, data);

    // Обрабатываем ответ с помощью лямбда-функции
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            qDebug() << "Ответ от сервера:" << QString(response);

            QJsonDocument responseDoc = QJsonDocument::fromJson(response);

            if (responseDoc.isObject()) {
                QJsonObject responseObj = responseDoc.object();

                if (responseObj.contains("token")) {
                    authToken = responseObj["token"].toString();
                    qDebug() << "Получен токен:" << authToken;
                    emit onloginSuccess();
                } else if (responseObj.contains("detail")) {
                    QString errorMessage = responseObj["detail"].toString();
                    emit onloginFailed(errorMessage);
                } else {
                    emit onloginFailed("Неизвестный формат ответа от сервера.");
                }
            } else {
                emit onloginFailed("Неверный формат ответа от сервера.");
            }
        } else {
            emit onloginFailed("Сетевая ошибка: " + reply->errorString());
        }

        reply->deleteLater();
    });
}

void AuthWindow::loginSuccess() {
    QMessageBox::information(this, "Успех", "Вы успешно вошли в систему.");

    this->close();

    MainWindow *mainWindow = new MainWindow();
    mainWindow->setAuthToken(this->getAuthToken()); //передача в мейн токена
    mainWindow->show();
}

void AuthWindow::loginFailed(const QString &errorMessage) {
    QMessageBox::warning(this, "Ошибка входа", "Ошибка: " + errorMessage);

    passwordInput->clear();
    passwordInput->setFocus();
}

#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

extern const QString ip_adress;

class QLineEdit;
class QPushButton;

class AuthWindow : public QWidget {
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);
    QString getAuthToken();


private slots:
    void handleLogin();
    void checkCredentials(const QString &username, const QString &password);
    void loginFailed(const QString &errorMessage);
    void loginSuccess();

signals:
    void onloginSuccess();
    void onloginFailed(const QString &errorMessage);

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;

    QNetworkAccessManager *manager;
    QString authToken;
};

#endif // AUTHWINDOW_H

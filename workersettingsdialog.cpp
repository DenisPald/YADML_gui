#include "workersettingsdialog.h"

WorkerSettingsDialog::WorkerSettingsDialog(const QString &workerName, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Настройки воркера: " + workerName);

    QFormLayout *formLayout = new QFormLayout(this);

    ipEdit = new QLineEdit(this);
    portEdit = new QLineEdit(this);
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow("IP адрес:", ipEdit);
    formLayout->addRow("Порт:", portEdit);
    formLayout->addRow("Имя пользователя:", usernameEdit);
    formLayout->addRow("Пароль:", passwordEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    formLayout->addWidget(buttonBox);

    setLayout(formLayout);
}

QString WorkerSettingsDialog::getIP() const {
    return ipEdit->text();
}

int WorkerSettingsDialog::getPort() const {
    return portEdit->text().toInt();
}

QString WorkerSettingsDialog::getUsername() const {
    return usernameEdit->text();
}

QString WorkerSettingsDialog::getPassword() const {
    return passwordEdit->text();
}

void WorkerSettingsDialog::setIP(const QString &ip) {
    ipEdit->setText(ip);
}

void WorkerSettingsDialog::setPort(int port) {
    portEdit->setText(QString::number(port));
}

void WorkerSettingsDialog::setUsername(const QString &username) {
    usernameEdit->setText(username);
}

void WorkerSettingsDialog::setPassword(const QString &password) {
    passwordEdit->setText(password);
}

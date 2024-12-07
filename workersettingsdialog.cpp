#include "workersettingsdialog.h"

WorkerSettingsDialog::WorkerSettingsDialog(const QStringList &workers, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Настройки воркеров");
    resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    tabWidget = new QTabWidget(this);

    for (const QString &workerName : workers) {
        createWorkerTab(workerName);
    }

    layout->addWidget(tabWidget);

    QPushButton *saveButton = new QPushButton("Сохранить", this);
    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(saveButton);

    setLayout(layout);
}

void WorkerSettingsDialog::createWorkerTab(const QString &workerName) {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *tabLayout = new QVBoxLayout(tab);

    QLabel *infoLabel = new QLabel("Настройки для " + workerName, this);
    tabLayout->addWidget(infoLabel);

    QLineEdit *ipInput = new QLineEdit(this);
    ipInput->setPlaceholderText("IP адрес");
    tabLayout->addWidget(ipInput);

    QLineEdit *portInput = new QLineEdit(this);
    portInput->setPlaceholderText("Порт");
    tabLayout->addWidget(portInput);

    QLineEdit *loginInput = new QLineEdit(this);
    loginInput->setPlaceholderText("Логин");
    tabLayout->addWidget(loginInput);

    QLineEdit *passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Пароль");
    passwordInput->setEchoMode(QLineEdit::Password); // Скрытие введенного текста
    tabLayout->addWidget(passwordInput);

    tab->setLayout(tabLayout);
    tabWidget->addTab(tab, workerName);
}

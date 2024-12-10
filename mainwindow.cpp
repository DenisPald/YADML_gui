#include "mainwindow.h"
#include "workersettingsdialog.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Распределенное обучение");
    setMinimumSize(400, 300);

    auto *layout = new QVBoxLayout(this);

    QLabel *fileLabel = new QLabel("Выберите файл матрицы:");
    layout->addWidget(fileLabel);

    filePathInput = new QLineEdit(this);
    filePathInput->setPlaceholderText("Путь к файлу");
    filePathInput->setReadOnly(true);
    layout->addWidget(filePathInput);

    QPushButton *loadButton = new QPushButton("Загрузить файл", this);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadMatrix);
    layout->addWidget(loadButton);

    QLabel *machineLabel = new QLabel("Выберите машины для обучения:");
    layout->addWidget(machineLabel);

    machineSelector = createMachineSelector();
    layout->addWidget(machineSelector);

    QHBoxLayout *workerButtonsLayout = new QHBoxLayout();

    QPushButton *addWorkerButton = new QPushButton("Добавить воркер", this);
    connect(addWorkerButton, &QPushButton::clicked, this, &MainWindow::addWorker);
    workerButtonsLayout->addWidget(addWorkerButton);

    QPushButton *removeWorkerButton = new QPushButton("Удалить воркер", this);
    connect(removeWorkerButton, &QPushButton::clicked, this, &MainWindow::removeWorker);
    workerButtonsLayout->addWidget(removeWorkerButton);

    layout->addLayout(workerButtonsLayout);

    QPushButton *settingsButton = new QPushButton("Открыть настройки воркеров", this);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openWorkerSettings);
    layout->addWidget(settingsButton);

    QPushButton *getDataButton = new QPushButton("Получить данные", this);
    connect(getDataButton, &QPushButton::clicked, this, &MainWindow::getData);
    layout->addWidget(getDataButton);

    QPushButton *sendDataButton = new QPushButton("Отправить данные", this);
    connect(sendDataButton, &QPushButton::clicked, this, &MainWindow::sendData);
    layout->addWidget(sendDataButton);

    startTrainingButton = new QPushButton("Начать обучение", this);
    connect(startTrainingButton, &QPushButton::clicked, this, &MainWindow::startTraining);
    layout->addWidget(startTrainingButton);

    setLayout(layout);

    setupDatabase(); // Настраиваем базу данных
    loadWorkers();   // Загружаем воркеров из базы данных
}

void MainWindow::setupDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("workers.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных!");
        return;
    }

    QSqlQuery query;
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS workers (
            id INTEGER PRIMARY KEY,
            name TEXT UNIQUE,
            ip TEXT,
            port INTEGER,
            username TEXT,
            password TEXT
        )
    )");
}


void MainWindow::loadWorkers() {
    machineSelector->clear();

    QSqlQuery query("SELECT name FROM workers");
    while (query.next()) {
        QString workerName = query.value(0).toString();
        machineSelector->addItem(workerName);
    }
}

void MainWindow::deleteWorker(const QString &workerName) {
    QSqlQuery query;
    query.prepare("DELETE FROM workers WHERE name = :name");
    query.bindValue(":name", workerName);
    query.exec();
}

void MainWindow::loadMatrix() {
    filePath = QFileDialog::getOpenFileName(this, "Выберите файл матрицы", "", "Matrix Files (*.txt *.csv)");
    if (!filePath.isEmpty()) {
        filePathInput->setText(filePath);
    }
}

QListWidget* MainWindow::createMachineSelector() {
    auto *selector = new QListWidget(this);
    selector->setSelectionMode(QAbstractItemView::MultiSelection);
    return selector;
}

void MainWindow::addWorker() {
    bool ok;
    QString workerName = QInputDialog::getText(this, "Добавить нового воркера", "Введите имя воркера:", QLineEdit::Normal, "", &ok);

    if (!ok || workerName.isEmpty()) {
        return; // Пользователь отменил ввод
    }

    // Проверяем, есть ли воркер с таким именем в базе
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM workers WHERE name = :name");
    checkQuery.bindValue(":name", workerName);
    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось проверить существование воркера.");
        return;
    }

    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Ошибка", "Воркер с таким именем уже существует.");
        return;
    }

    // Добавляем нового воркера в базу данных
    QSqlQuery insertQuery;
    insertQuery.prepare(R"(
        INSERT INTO workers (name, ip, port, username, password)
        VALUES (:name, :ip, :port, :username, :password)
    )");
    insertQuery.bindValue(":name", workerName);
    insertQuery.bindValue(":ip", "127.0.0.1");  // Значение по умолчанию
    insertQuery.bindValue(":port", 8080);      // Значение по умолчанию
    insertQuery.bindValue(":username", "");   // Пустой логин
    insertQuery.bindValue(":password", "");   // Пустой пароль

    if (!insertQuery.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить воркера в базу данных.");
        return;
    }

    // Добавляем нового воркера в QListWidget
    machineSelector->addItem(workerName);

    QMessageBox::information(this, "Успех", "Новый воркер успешно добавлен.");
}


void MainWindow::removeWorker() {
    QList<QListWidgetItem *> selectedItems = machineSelector->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выбраны воркеры для удаления");
        return;
    }

    for (QListWidgetItem *item : selectedItems) {
        deleteWorker(item->text());  // Удаляем из базы данных
        delete machineSelector->takeItem(machineSelector->row(item)); // Удаляем из списка
    }
}

void MainWindow::openWorkerSettings() {
    QList<QListWidgetItem *> selectedItems = machineSelector->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выбраны машины для настройки");
        return;
    }

    for (QListWidgetItem *item : selectedItems) {
        QString workerName = item->text();

        // Получаем текущие параметры из базы данных
        QSqlQuery query;
        query.prepare("SELECT ip, port, username, password FROM workers WHERE name = :name");
        query.bindValue(":name", workerName);
        if (!query.exec() || !query.next()) {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить данные для воркера: " + workerName);
            continue;
        }

        QString ip = query.value(0).toString();
        int port = query.value(1).toInt();
        QString username = query.value(2).toString();
        QString password = query.value(3).toString();

        WorkerSettingsDialog dialog(workerName, this);
        dialog.setIP(ip);
        dialog.setPort(port);
        dialog.setUsername(username);
        dialog.setPassword(password);

        if (dialog.exec() == QDialog::Accepted) {
            // Сохраняем обновленные параметры
            QSqlQuery updateQuery;
            updateQuery.prepare(R"(
                UPDATE workers
                SET ip = :ip, port = :port, username = :username, password = :password
                WHERE name = :name
            )");
            updateQuery.bindValue(":ip", dialog.getIP());
            updateQuery.bindValue(":port", dialog.getPort());
            updateQuery.bindValue(":username", dialog.getUsername());
            updateQuery.bindValue(":password", dialog.getPassword());
            updateQuery.bindValue(":name", workerName);
            if (!updateQuery.exec()) {
                QMessageBox::warning(this, "Ошибка", "Не удалось сохранить изменения для воркера: " + workerName);
            }
        }
    }
}


void MainWindow::startTraining() {
    QStringList selectedMachines;
    for (QListWidgetItem *item : machineSelector->selectedItems()) {
        selectedMachines << item->text();
    }

    if (selectedMachines.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выбраны машины для обучения");
        return;
    }

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не выбран файл для обучения");
        return;
    }

    QMessageBox::information(this, "Обучение", "Обучение началось на следующих машинах: " + selectedMachines.join(", "));
}

void MainWindow::getData() {
    QMessageBox::information(this, "Получить данные", "Функционал получения данных будет реализован позже.");
}

void MainWindow::sendData() {
    QMessageBox::information(this, "Отправить данные", "Функционал отправки данных будет реализован позже.");
}

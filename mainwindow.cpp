#include "mainwindow.h"
#include "authwindow.h"
#include "workersettingsdialog.h"



MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Распределенное обучение");
    setMinimumSize(1000, 700);

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

    // Настройка выбора метода деления датасета
    QLabel *datasetLabel = new QLabel("Выберите метод деления датасета:");
    layout->addWidget(datasetLabel);

    datasetGroup = new QButtonGroup(this);
    QVBoxLayout *datasetLayout = new QVBoxLayout();

    randomSplitButton = new QRadioButton("Случайное деление", this);
    stratifiedSplitButton = new QRadioButton("Стратифицированное деление", this);

    datasetGroup->addButton(randomSplitButton);
    datasetGroup->addButton(stratifiedSplitButton);
    datasetLayout->addWidget(randomSplitButton);
    datasetLayout->addWidget(stratifiedSplitButton);

    layout->addLayout(datasetLayout);

    // Настройка выбора метода агрегации весов
    QLabel *aggregationLabel = new QLabel("Выберите метод объединения весов:");
    layout->addWidget(aggregationLabel);

    aggregationGroup = new QButtonGroup(this);
    QVBoxLayout *aggregationLayout = new QVBoxLayout();

    simple_averageButton = new QRadioButton("simple_average", this);
    medianButton = new QRadioButton("median", this);
    regularizedButton = new QRadioButton("regularized", this);

    aggregationGroup->addButton(simple_averageButton);
    aggregationGroup->addButton(medianButton);
    aggregationGroup->addButton(regularizedButton);

    aggregationLayout->addWidget(simple_averageButton);
    aggregationLayout->addWidget(medianButton);
    aggregationLayout->addWidget(regularizedButton);



    layout->addLayout(aggregationLayout);

    // Кнопки для открытия настроек и получения/отправки данных
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

    QLabel *globalEpochsLabel = new QLabel("Количество глобальных эпох:");
    globalEpochsInput = new QLineEdit(this);
    globalEpochsInput->setPlaceholderText("Введите количество глобальных эпох");

    QLabel *localEpochsLabel = new QLabel("Количество локальных эпох:");
    localEpochsInput = new QLineEdit(this);
    localEpochsInput->setPlaceholderText("Введите количество локальных эпох");

    // Добавление виджетов в основной layout
    layout->addWidget(globalEpochsLabel);
    layout->addWidget(globalEpochsInput);

    layout->addWidget(localEpochsLabel);
    layout->addWidget(localEpochsInput);

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






void MainWindow::sendData() {
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Файл для отправки не выбран.");
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://192.168.31.88:8000/upload");
    QNetworkRequest request(url);

    // Указываем тип содержимого как multipart/form-data
    QString boundary = "---QtBoundary123456789"; // Уникальная граница для multipart данных
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());


    // Создаем данные для отправки
    QByteArray postData;
    postData.append(("--" + boundary + "\r\n").toUtf8());
    postData.append(("Content-Disposition: form-data; name=\"file\"; filename=\"" + QFileInfo(filePath).fileName() + "\"\r\n").toUtf8());
    postData.append("Content-Type: application/octet-stream\r\n\r\n");


    // Читаем содержимое файла и добавляем его к postData
    QFile file(filePath);

    postData.append(file.readAll());
    file.close();

    postData.append(("\r\n--" + boundary + "--\r\n").toUtf8());

    // Отправляем запрос
    QNetworkReply *reply = manager->post(request, postData);

    // Обрабатываем ответ
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Успех", "Файл успешно отправлен.");
        } else {
            QMessageBox::warning(this, "Ошибка", "Ошибка при отправке файла: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void MainWindow::getData() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://192.168.31.88:8000/get");
    QNetworkRequest request(url);

    // Указываем заголовок для авторизации
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    // Отправляем GET-запрос
    QNetworkReply *reply = manager->get(request);

    // Обрабатываем ответ
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Получаем бинарный файл из ответа
            QByteArray fileData = reply->readAll();

            // Сохраняем файл
            QString savePath = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "TensorFlow Models (*.pt)");
            if (!savePath.isEmpty()) {
                QFile file(savePath);
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(fileData);
                    file.close();
                    QMessageBox::information(this, "Успех", "Файл успешно загружен и сохранен.");
                } else {
                    QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
                }
            }
        } else {
            if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == "409"){
                QMessageBox::warning(this, "Ошибка", "Производится обучение");
            } else if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) =="400"){
                QMessageBox::warning(this, "Ошибка", "Обучение еще не проводилось");
            } else {
                QMessageBox::warning(this, "Ошибка", "Ошибка при загрузке файла: " + reply->errorString());
            }
        }
        reply->deleteLater();
    });
}

void MainWindow::startTraining() {
    qDebug() << "Запуск обучения...";

    // Проверяем подключение к базе данных
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных.");
        return;
    }

    if (!randomSplitButton || !stratifiedSplitButton ||
        !simple_averageButton || !medianButton) {
        QMessageBox::critical(this, "Ошибка", "Радиокнопки не инициализированы.");
        return;
    }

    // Определяем методы агрегации и деления датасета
    QString aggregationMethod = simple_averageButton->isChecked() ? "simple_average" :
                                    medianButton->isChecked() ? "median" : regularizedButton->isChecked() ? "regularized" : "";
    QString splitMethod = randomSplitButton->isChecked() ? "random" :
                              stratifiedSplitButton->isChecked() ? "stratified" : "";

    if (aggregationMethod.isEmpty() || splitMethod.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Не выбраны методы агрегации или деления датасета.");
        return;
    }

    // Получаем значения из полей ввода
    int globalEpochs = globalEpochsInput->text().toInt();
    int localEpochs = localEpochsInput->text().toInt();



    /*if (globalEpochs <= 0 || localEpochs <= 0) {
        QMessageBox::critical(this, "Ошибка", "Количество эпох должно быть больше 0.");
        return;
    }*/

    // Загружаем данные о воркерах из базы
    QSqlQuery query("SELECT ip, port, username, password FROM workers");
    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось выполнить запрос к базе данных: " + query.lastError().text());
        return;
    }

    if (!query.first()) {
        QMessageBox::critical(this, "Ошибка", "Нет данных о воркерах в базе.");
        return;
    }

    // Формируем JSON для запроса
    QJsonArray nodesArray;
    do {
        QJsonObject nodeObject{
            {"hostname", query.value(0).toString()},
            {"port", query.value(1).toString()},
            {"username", query.value(2).toString()},
            {"password", query.value(3).toString()}
        };
        nodesArray.append(nodeObject);
    } while (query.next());

    QJsonObject json{
        {"aggregation_method", aggregationMethod},
        {"split_method", splitMethod},
        {"global", globalEpochs}, // Добавляем глобальные эпохи
        {"local", localEpochs},   // Добавляем локальные эпохи
        {"nodes", nodesArray}
    };

    QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);

    // Инициализируем менеджер при необходимости
    if (!manager) {
        manager = new QNetworkAccessManager(this);
    }

    // Настраиваем запрос
    QUrl url("http://192.168.31.88:8000/run");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

    // Отправляем запрос в формате JSON
    QNetworkReply *reply = manager->post(request, jsonData);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Успех", "Обучение успешно начато.");
        } else {
            if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == "409"){
                QMessageBox::warning(this, "Ошибка", "Производится обучение");
            } else {
                QMessageBox::warning(this, "Ошибка", "Ошибка при загрузке файла: " + reply->errorString());
            }
        }
        reply->deleteLater();
    });
}

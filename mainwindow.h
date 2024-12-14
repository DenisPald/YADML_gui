#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QButtonGroup>
#include <QRadioButton>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSqlError>


class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setAuthToken(const QString &token) {
        authToken = token;
    }


private slots:
    void loadMatrix();          // Загрузка матрицы из файла
    void openWorkerSettings();  // Открытие настроек воркеров
    void startTraining();       // Начало обучения
    void getData();             // Получение данных (заглушка)
    void sendData();            // Отправка данных
    void addWorker();           // Добавление воркера
    void removeWorker();        // Удаление выбранных воркеров

private:
    void setupDatabase();       // Настройка базы данных
    void loadWorkers();         // Загрузка списка воркеров из базы данных
    void saveWorker(const QString &workerName);  // Сохранение воркера в базу данных
    void deleteWorker(const QString &workerName); // Удаление воркера из базы данных

    QButtonGroup *datasetGroup;           // Группа для выбора метода деления датасета
    QButtonGroup *aggregationGroup;       // Группа для выбора метода объединения весов

    QLineEdit *filePathInput;      // Поле для отображения пути к файлу
    QListWidget *machineSelector; // Список выбора машин
    QPushButton *startTrainingButton; // Кнопка для начала обучения
    QString filePath;              // Путь к выбранному файлу

    QString authToken;

    QLineEdit *globalEpochsInput; // Поле ввода для глобальных эпох
    QLineEdit *localEpochsInput;  // Поле ввода для локальных эпох



    QRadioButton *randomSplitButton;
    QRadioButton *stratifiedSplitButton;
    QRadioButton *medianButton;
    QRadioButton *simple_averageButtton;
    QRadioButton *regularizedButtton;

    QSqlDatabase db;               // Объект базы данных
    QListWidget* createMachineSelector(); // Метод для создания виджета выбора машин

    QNetworkAccessManager *manager;

};

#endif // MAINWINDOW_H

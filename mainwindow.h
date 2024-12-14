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
    void loadMatrix();           // Загрузка матрицы из файла
    void openWorkerSettings();   // Открытие настроек воркеров
    void startTraining();        // Начало обучения
    void getData();              // Получение данных (заглушка, должна быть определена в cpp)
    void sendData();             // Отправка данных
    void addWorker();            // Добавление воркера
    void removeWorker();         // Удаление выбранных воркеров

private:
    void setupDatabase();       // Настройка базы данных
    void loadWorkers();         // Загрузка списка воркеров из базы данных
    void deleteWorker(const QString &workerName); // Удаление воркера из базы данных

    QButtonGroup *datasetGroup;           // Группа для выбора метода деления датасета
    QButtonGroup *aggregationGroup;       // Группа для выбора метода объединения весов

    QLineEdit *filePathInput;      // Поле для отображения пути к файлу
    QListWidget *machineSelector;   // Список выбора машин
    QPushButton *startTrainingButton; // Кнопка для начала обучения
    QString filePath;               // Путь к выбранному файлу

    QString authToken;              // Токен авторизации

    QLineEdit *globalEpochsInput;   // Поле ввода глобальных эпох
    QLineEdit *localEpochsInput;    // Поле ввода локальных эпох

    QRadioButton *randomSplitButton;       // Радиокнопка для случайного деления
    QRadioButton *stratifiedSplitButton;   // Радиокнопка для стратифицированного деления
    QRadioButton *medianButton;            // Радиокнопка для median
    QRadioButton *simple_averageButton;    // Радиокнопка для simple_average
    QRadioButton *regularizedButton;       // Радиокнопка для regularized

    QSqlDatabase db;               // Объект базы данных
    QNetworkAccessManager *manager; // Менеджер сетевых запросов

    QListWidget* createMachineSelector(); // Метод для создания виджета выбора машин
};

#endif // MAINWINDOW_H

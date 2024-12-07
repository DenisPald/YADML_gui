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

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void loadMatrix();          // Загрузка матрицы из файла
    void openWorkerSettings();  // Открытие настроек воркеров
    void startTraining();       // Начало обучения
    void getData();             // Получение данных (заглушка)
    void sendData();            // Отправка данных (заглушка)

private:
    QLineEdit *filePathInput;   // Поле для отображения пути к файлу
    QListWidget *machineSelector; // Список выбора машин
    QPushButton *startTrainingButton; // Кнопка для начала обучения
    QString filePath;           // Путь к выбранному файлу

    QListWidget* createMachineSelector(); // Метод для создания виджета выбора машин
};

#endif // MAINWINDOW_H


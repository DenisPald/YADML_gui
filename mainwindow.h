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
#include <QEventLoop>
#include <QUrlQuery>




class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setAuthToken(const QString &token) {
        authToken = token;
    }

private slots:
    void loadMatrix();
    void openWorkerSettings();
    void startTraining();
    void getData();
    void sendData();
    void addWorker();
    void removeWorker();

private:
    void setupDatabase();
    void loadWorkers();
    void deleteWorker(const QString &workerName);

    QNetworkAccessManager *manager = nullptr;

    QButtonGroup *datasetGroup;
    QButtonGroup *aggregationGroup;

    QLineEdit *filePathInput;
    QListWidget *machineSelector;
    QPushButton *startTrainingButton;
    QString filePath;

    QString authToken;

    QLineEdit *globalEpochsInput;
    QLineEdit *localEpochsInput;

    QRadioButton *randomSplitButton;
    QRadioButton *stratifiedSplitButton;
    QRadioButton *medianButton;
    QRadioButton *simple_averageButton;
    QRadioButton *regularizedButton;

    QSqlDatabase db;

    QListWidget* createMachineSelector();
};

#endif // MAINWINDOW_H

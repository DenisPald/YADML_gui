#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void loadMatrix();
    void startTraining();

private:
    QLineEdit *filePathInput;
    QListWidget *machineSelector;
    QPushButton *startTrainingButton;
    QString filePath;

    QListWidget* createMachineSelector();
};

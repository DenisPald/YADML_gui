#include "mainwindow.h"
#include <QMessageBox>

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

    startTrainingButton = new QPushButton("Начать обучение", this);
    connect(startTrainingButton, &QPushButton::clicked, this, &MainWindow::startTraining);
    layout->addWidget(startTrainingButton);

    setLayout(layout);
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

    // Заглушка, потом будет получение машин с сервера
    selector->addItem("Machine 1");
    selector->addItem("Machine 2");
    selector->addItem("Machine 3");
    selector->addItem("Machine abc");

    return selector;
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

    if (filePath.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Не выбран файл для обучения");
        return;
    }

    // Заглушка
    QMessageBox::information(this, "Обучение", "Обучение началось на следующих машинах: " + selectedMachines.join(", "));
}

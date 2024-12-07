#ifndef WORKERSETTINGSDIALOG_H
#define WORKERSETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class WorkerSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit WorkerSettingsDialog(const QStringList &workers, QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;

    void createWorkerTab(const QString &workerName);
};

#endif // WORKERSETTINGSDIALOG_H

#ifndef WORKERSETTINGSDIALOG_H
#define WORKERSETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>

class WorkerSettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit WorkerSettingsDialog(const QString &workerName, QWidget *parent = nullptr);

    QString getIP() const;
    int getPort() const;
    QString getUsername() const;
    QString getPassword() const;

    void setIP(const QString &ip);
    void setPort(int port);
    void setUsername(const QString &username);
    void setPassword(const QString &password);

private:
    QLineEdit *ipEdit;
    QLineEdit *portEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
};

#endif // WORKERSETTINGSDIALOG_H

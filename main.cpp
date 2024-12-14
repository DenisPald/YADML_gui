#include <QApplication>
#include "authwindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    AuthWindow authWindow;
    authWindow.show();

    app.setStyleSheet(R"(
        QWidget {
            background-color: #f8f8f8;
            color: #333;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            font-size: 14px;
        }
        QLabel {
            font-size: 16px;
            font-weight: bold;
            color: #007AFF;
        }
        QLineEdit {
            border: 1px solid #d0d0d0;
            border-radius: 8px;
            padding: 6px 10px;
            background-color: #ffffff;
            color: #333;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 1px solid #007AFF;
            outline: none;
        }
        QPushButton {
            background-color: #007AFF;
            border: none;
            border-radius: 8px;
            color: #ffffff;
            font-size: 14px;
            padding: 8px 12px;
        }
        QPushButton:hover {
            background-color: #005ecb;
        }
        QPushButton:pressed {
            background-color: #004ba0;
        }
    )");
    return app.exec();
}

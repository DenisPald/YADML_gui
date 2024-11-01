#include <QApplication>
#include "authwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AuthWindow authWindow;
    authWindow.show();
    app.setStyleSheet(R"(
    QWidget {
        font-family: NerdFont;
        font-size: 14px;
    }
    )");
    return app.exec();
}

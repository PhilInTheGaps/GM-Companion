#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    w.showMaximized();

    w.setVersion("0.3.0.0");

    return app.exec();
}

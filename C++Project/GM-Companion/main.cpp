#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    w.resize(QApplication::primaryScreen()->availableGeometry().width(), QApplication::primaryScreen()->availableGeometry().height());


    w.showMaximized();
    w.createThumbnailToolbar();

    return app.exec();
}

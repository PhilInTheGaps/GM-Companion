#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    QRect rec = QApplication::desktop()->screenGeometry();
    w.resize(rec.width(), rec.height());

    w.showMaximized();
    w.createThumbnailToolbar();

    return app.exec();
}

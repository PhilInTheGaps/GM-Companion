#include "mainwindow.h"
#include "settingsmanager.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    // Set StyleSheet
    SettingsManager* settings = new SettingsManager;
    QString style = settings->getSetting(Setting::uiMode);

    QFile file(QDir::homePath()+"/.gm-companion/styles/"+style+".qss");
    if (file.exists()){
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
    }else{
        QFile defaultStyle(QDir::homePath()+"/.gm-companion/styles/dark.qss");
        if (defaultStyle.exists()){
            defaultStyle.open(QFile::ReadOnly);
            QString styleSheet = QLatin1String(defaultStyle.readAll());
            app.setStyleSheet(styleSheet);
        }
    }

    // Sets the window size to maximized, w.showMaximized() is glitchy under windows
    #ifdef _WIN32
    w.resize(QApplication::primaryScreen()->availableGeometry().width(), QApplication::primaryScreen()->availableGeometry().height());
    #endif

    w.showMaximized();

    #ifdef _WIN32
    w.createThumbnailToolbar();
    #endif

    return app.exec();
}

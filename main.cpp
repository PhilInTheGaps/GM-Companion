#include "mainwindow.h"
#include "settingsmanager.h"
#include "whatisnewwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Translator
    SettingsManager* settings = new SettingsManager;
    QTranslator* translator = new QTranslator();

    #ifdef _WIN32
    if (translator->load("gm-companion_"+settings->getSetting(Setting::language), QApplication::applicationDirPath()+"/translations")) {
        app.installTranslator(translator);
    }
    #else
    if (translator->load("gm-companion_"+settings->getSetting(language), "/usr/share/gm-companion/translations")) {
        app.installTranslator(translator);
    }
    #endif

    // Start mainwindow
    MainWindow w;

    // Set StyleSheet
    QString style = settings->getSetting(Setting::uiMode);

    QFile file(QDir::homePath()+"/.gm-companion/styles/"+style+".qss");
    if (file.exists()){
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
    }else{
        QFile defaultStyle(QDir::homePath()+"/.gm-companion/styles/DarkStyle.qss");
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

    // Open Window Maximized
    w.showMaximized();
    w.focusWidget();

    // Create Thumbnail Toolbar if system is windows
    #ifdef _WIN32
    w.createThumbnailToolbar();
    #endif

    // Open WhatIsNewWindow
    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    int openNewFeatures = checkSettings.value("openWhatIsNewWindow", 1).toInt();
    int settingsVersion = checkSettings.value("version", 0).toInt();
    if (openNewFeatures == 1 || w.getVersionNumber() > settingsVersion){
        if (w.getVersionNumber() > settingsVersion){
            qDebug() << QCoreApplication::translate("Program Start", "Opening New Features Window because of an Update...");
        }else if (openNewFeatures == 1){
            qDebug() << QCoreApplication::translate("Program Start", "Opening New Features Window because of the settings preferences...");
        }

        WhatIsNewWindow* whatIsNewWindow = new WhatIsNewWindow;
        whatIsNewWindow->show();

        settings->updateSettings();

        w.updateSettingsVersion();
    }

    return app.exec();
}

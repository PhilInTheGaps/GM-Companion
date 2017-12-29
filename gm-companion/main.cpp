#include "mainwindow.h"
#include "managers/settingsmanager.h"
#include "dialogs/whatisnewwindow.h"
#include "managers/updatemanager.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QTranslator>
#include <QDebug>
#include <QSplashScreen>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QSettings>

// Responsible for writing the console output into a log if debug option in settings is disabled (default)
void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QDate date;
    QTime time;

    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = time.currentTime().toString() + ": " + QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = time.currentTime().toString() + ": " + QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = time.currentTime().toString() + ": " + QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = time.currentTime().toString() + ": " + QString("Fatal: %1").arg(msg);
        break;
    }

    QFile outFile(QDir::homePath()+"/.gm-companion/logs/"+date.currentDate().toString());
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Check if debug mode is enabled (disabled by default)
    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    if (checkSettings.value("debug", 0).toInt() == 1)
    {
        qDebug().noquote() << "Debug mode activated ...";
    }
    else
    {
        qDebug().noquote() << "Debug mode is not active ...";
        qInstallMessageHandler(myMessageHandler);
    }

    qDebug().noquote() << "Starting GM-Companion ...";

    // Show splash screen
    qDebug().noquote() << "Showing splash screen ...";
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/resources/splash.jpg"));
    splash->show();

    // Translator
    qDebug().noquote() << "Initializing translations ...";
    SettingsManager* settingsManager = new SettingsManager;
    QTranslator* translator = new QTranslator();

    #ifdef _WIN32
    if (translator->load("gm-companion_"+settingsManager->getSetting(language), QApplication::applicationDirPath()+"/translations")) {
        app.installTranslator(translator);
    }
    #else
    if (translator->load("gm-companion_"+settingsManager->getSetting(language), "/usr/share/gm-companion/translations")) {
        app.installTranslator(translator);
    }
    #endif

    // Start mainwindow
    MainWindow w;
    w.setVersion("1.0.0.0");

    // Update Manager
    UpdateManager *updateManager = new UpdateManager(1000);
    updateManager->checkForUpdates();

    // Set StyleSheet
    QString style = settingsManager->getSetting(uiMode);
    qDebug().noquote() << "Loading stylesheet:" << style << "...";

    QFile file(QDir::homePath()+"/.gm-companion/styles/"+style+".qss");
    if (file.exists()){
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
    }else{
        QFile defaultStyle(QDir::homePath()+"/.gm-companion/styles/White.qss");
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
    qDebug().noquote() << "Opening UI ...";
    w.showMaximized();
    w.focusWidget();

    // Add Tools to mainwindow
    w.addTools();

    // I currently disabled the new features window because I don't know what to put in there this patch...

//    // Open WhatIsNewWindow
//    int openNewFeatures = checkSettings.value("openWhatIsNewWindow", 1).toInt();
//    int settingsVersion = checkSettings.value("version", 0).toInt();
//    if (openNewFeatures == 1 || w.getVersionNumber() > settingsVersion)
//    {
//        if (w.getVersionNumber() > settingsVersion)
//        {
//            qDebug().noquote() << QCoreApplication::translate("Program Start", "Opening New Features Window because of an Update...");
//        }
//        else if (openNewFeatures == 1)
//        {
//            qDebug().noquote() << QCoreApplication::translate("Program Start", "Opening New Features Window because of the settings preferences...");
//        }

//        WhatIsNewWindow* whatIsNewWindow = new WhatIsNewWindow;
//        whatIsNewWindow->show();

//        settingsManager->updateSettings();

//        w.updateSettingsVersion();
//    }

    qDebug().noquote() << "Closing splash screen ...";
    splash->close();

    return app.exec();
}

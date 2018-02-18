#include "ui/mainwindow.h"
#include "settings/settingsmanager.h"
#include "ui/whatisnewwindow.h"
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

// Responsible for writing the console output into a log if debug option in
// settings is disabled (default)
void myMessageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg)
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

    default:
        txt = time.currentTime().toString() + ": " + msg;
        break;
    }

    QFile outFile(QDir::homePath() + "/.gm-companion/logs/" + date.currentDate().toString());
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

// Show "What is new?" window
void showWhatIsNew(SettingsManager *settingsManager, MainWindow *w)
{
    QSettings checkSettings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);

    // Should window be opened by default?
    int openNewFeatures = checkSettings.value("openWhatIsNewWindow", 1).toInt();

    // Program version the last time it was used
    int settingsVersion = checkSettings.value("version", 0).toInt();

    if ((openNewFeatures == 1) || (w->getVersionNumber() > settingsVersion))
    {
        // Print the reason why the window is being opened
        if (w->getVersionNumber() > settingsVersion) qDebug().noquote() << "Opening New Features Window because of an Update...";
        else if (openNewFeatures == 1) qDebug().noquote() << "Opening New Features Window because of the settings preferences...";

        WhatIsNewWindow *whatIsNewWindow = new WhatIsNewWindow;
        whatIsNewWindow->show();

        // Update the settings file in case it needs to be modified because of
        // an update
        settingsManager->updateSettings();

        // Update the version of the settings file
        w->updateSettingsVersion();
    }
}

// Check if debug mode is enabled (disabled by default)
void enableDebug()
{
    QSettings checkSettings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);

    if (checkSettings.value("debug", 0).toInt() == 1)
    {
        qDebug().noquote() << "Debug mode activated ...";
    }
    else
    {
        qDebug().noquote() << "Debug mode is not active ...";

        // Debug messages are written in a log file instead of the console
        qInstallMessageHandler(myMessageHandler);
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Enable or disable debug mode
    enableDebug();

    qDebug().noquote() << "Starting GM-Companion ...";

    // Show splash screen
    qDebug().noquote() << "Showing splash screen ...";
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/splash.jpg"));
    splash->show();

    // Set the language and install a translator
    qDebug().noquote() << "Initializing translations ...";
    SettingsManager *settingsManager = new SettingsManager;
    QTranslator     *translator      = new QTranslator();

    if (translator->load("gm-companion_" + settingsManager->getSetting(language), ":/translations")) app.installTranslator(translator);
    else qDebug() << "Could not load translation ...";

    // Start mainwindow
    MainWindow *w = new MainWindow();
    w->setVersion("1.0.0.0");

    // Add Tools to mainwindow
    w->addTools();

    // Update Manager checks if a new version of the gm-companion is available
    UpdateManager *updateManager = new UpdateManager(1000);
    updateManager->checkForUpdates();

    // Set StyleSheet
    settingsManager->setStyleSheet(settingsManager->getSetting(uiMode));

    // Sets the window size to maximized, w.showMaximized() is glitchy under
    // windows
    #ifdef _WIN32
    w->resize(QApplication::primaryScreen()->availableGeometry().width(), QApplication::primaryScreen()->availableGeometry().height());
    #endif // ifdef _WIN32

    // Open Window Maximized
    qDebug().noquote() << "Opening UI ...";
    w->showMaximized();
    w->focusWidget();

    // Open WhatIsNewWindow
    //    showWhatIsNew(settingsManager, w); // TODO


    qDebug().noquote() << "Closing splash screen ...";
    splash->close();
    delete splash;

    return app.exec();
}

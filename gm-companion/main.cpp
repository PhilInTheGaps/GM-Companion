#include "settings/settingsmanager.h"

// #include "ui/whatisnewwindow.h"
// #include "managers/updatemanager.h"

#include <QTranslator>

#include <QDebug>

// #include <QFile>
// #include <QTextStream>
// #include <QDateTime>
#include <QSettings>
#include <QDir>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

#include "tools/audio/audiotool.h"
#include "tools/maptool.h"
#include "tools/dicetool.h"
#include "tools/combattracker.h"
#include "tools/notestool.h"
#include "tools/convertertool.h"
#include "settings/settingstool.h"
#include "managers/updatemanager.h"
#include "platformdetails.h"

/*
   // Responsible for writing the console output into a log if debug option in
   // settings is disabled (default)
   void myMessageHandler(QtMsgType type, const QMessageLogContext&, const
   QString& msg)
   {


    QDate date;
    QTime time;

    QString txt;

    switch (type) {
    case QtDebugMsg:
        txt = time.currentTime().toString() + ": " + QString("Debug:
   %1").arg(msg);
        break;

    case QtWarningMsg:
        txt = time.currentTime().toString() + ": " + QString("Warning:
   %1").arg(msg);
        break;

    case QtCriticalMsg:
        txt = time.currentTime().toString() + ": " + QString("Critical:
   %1").arg(msg);
        break;

    case QtFatalMsg:
        txt = time.currentTime().toString() + ": " + QString("Fatal:
   %1").arg(msg);
        break;

    default:
        txt = time.currentTime().toString() + ": " + msg;
        break;
    }

    QFile outFile(QDir::homePath() + "/.gm-companion/logs/" +
   date.currentDate().toString());
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
   }
 */

/*
   // Show "What is new?" window
   void showWhatIsNew(SettingsManager *settingsManager, MainWindow *w)
   {
    QSettings checkSettings(QDir::homePath() + "/.gm-companion/settings.ini",
   QSettings::IniFormat);

    // Should window be opened by default?
    int openNewFeatures = checkSettings.value("openWhatIsNewWindow",
   1).toInt();

    // Program version the last time it was used
    int settingsVersion = checkSettings.value("version", 0).toInt();

    if ((openNewFeatures == 1) || (w->getVersionNumber() > settingsVersion))
    {
        // Print the reason why the window is being opened
        if (w->getVersionNumber() > settingsVersion) qDebug().noquote() <<
   "Opening New Features Window because of an Update...";
        else if (openNewFeatures == 1) qDebug().noquote() << "Opening New
   Features Window because of the settings preferences...";

        WhatIsNewWindow *whatIsNewWindow = new WhatIsNewWindow;
        whatIsNewWindow->show();

        // Update the settings file in case it needs to be modified because of
        // an update
        settingsManager->updateSettings();

        // Update the version of the settings file
        w->updateSettingsVersion();
    }
   }
 */


// Check if debug mode is enabled (disabled by default)
void enableDebug()
{
    QSettings checkSettings(QDir::homePath() + "/.gm-companion/settings.ini",  QSettings::IniFormat);

    if (checkSettings.value("debug", 0).toInt() == 1)
    {
        qDebug().noquote() << "Debug mode activated ...";
    }
    else
    {
        qDebug().noquote() << "Debug mode is not active ...";

        // Debug messages are written in a log file instead of the console
        //        qInstallMessageHandler(myMessageHandler);
    }
}

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif // if defined(Q_OS_WIN)

    QGuiApplication app(argc, argv);

    // Enable or disable debug mode
    enableDebug();

    qDebug().noquote() << "Starting GM-Companion ...";

    // Set the language and install a translator
    qDebug().noquote() << "Initializing translations ...";
    SettingsManager *settingsManager = new SettingsManager;
    QTranslator     *translator      = new QTranslator();

    if (translator->load("gm-companion_" + settingsManager->getSetting(language), ":/translations")) app.installTranslator(translator);
    else qDebug() << "Could not load translation ...";

    // Make classes available for QML
    QUrl source(QStringLiteral("qrc:/main.qml"));
    qmlRegisterType<AudioTool>(      "gm.companion.audiotool",     1, 0, "AudioTool");
    qmlRegisterType<MapTool>(        "gm.companion.maptool",       1, 0, "MapTool");
    qmlRegisterType<DiceTool>(       "gm.companion.dicetool",      1, 0, "DiceTool");
    qmlRegisterType<CombatTracker>(  "gm.companion.combattracker", 1, 0, "CombatTrackerTool");
    qmlRegisterType<NotesTool>(      "gm.companion.notestool",     1, 0, "NotesTool");
    qmlRegisterType<ConverterTool>(  "gm.companion.convertertool", 1, 0, "ConverterTool");
    qmlRegisterType<SettingsTool>(   "gm.companion.settingstool",  1, 0, "SettingsTool");

    qmlRegisterType<UpdateManager>(  "gm.companion.updatemanager", 1, 0, "UpdateManager");
    qmlRegisterType<PlatformDetails>("gm.companion.platforms",     1, 0, "PlatformDetails");

    // Set Icon
    app.setWindowIcon(QIcon(":/icons/gm-companion/icon256_new.png"));

    QQmlApplicationEngine engine;
    engine.load(source);

    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();

    /*
        // Update Manager checks if a new version of the gm-companion is
       available
        UpdateManager *updateManager = new UpdateManager(1000);
        updateManager->checkForUpdates();
     */
}

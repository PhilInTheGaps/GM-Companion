#include "mainwindow.h"
#include "managers/settingsmanager.h"
#include "dialogs/whatisnewwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QTranslator>
#include <QDebug>
#include <QSplashScreen>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

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

//    qInstallMessageHandler(myMessageHandler);

    // Show splash screen
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/resources/splash.jpg"));
    splash->show();

    // Translator
    qDebug() << "Initializing translations ...";
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
    qDebug() << "Loading stylesheet ...";
    QString style = settings->getSetting(uiMode);

    QFile file(QDir::homePath()+"/.gm-companion/styles/"+style+".qss");
    if (file.exists()){
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
    }else{
        QFile defaultStyle(QDir::homePath()+"/.gm-companion/styles/Dark.qss");
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
    qDebug() << "Opening UI ...";
    w.showMaximized();
    w.focusWidget();

    // Add Tools to mainwindow
    qDebug() << "Loading tools ...";
    w.addTools();

    // Open WhatIsNewWindow
    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    int openNewFeatures = checkSettings.value("openWhatIsNewWindow", 1).toInt();
    int settingsVersion = checkSettings.value("version", 0).toInt();
    if (openNewFeatures == 1 || w.getVersionNumber() > settingsVersion)
    {
        if (w.getVersionNumber() > settingsVersion)
        {
            qDebug() << QCoreApplication::translate("Program Start", "Opening New Features Window because of an Update...");
        }
        else if (openNewFeatures == 1)
        {
            qDebug() << QCoreApplication::translate("Program Start", "Opening New Features Window because of the settings preferences...");
        }

        WhatIsNewWindow* whatIsNewWindow = new WhatIsNewWindow;
        whatIsNewWindow->show();

        settings->updateSettings();

        w.updateSettingsVersion();
    }

    splash->close();

    return app.exec();
}

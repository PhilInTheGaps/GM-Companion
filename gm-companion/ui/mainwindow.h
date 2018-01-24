#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMediaPlayer>
#include <QMediaPlayerControl>
#include <QMediaPlaylist>
#include <QUrl>
#include <QSignalMapper>
#include <QTreeWidget>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QNetworkReply>
#include <QTimer>
#include <QTabWidget>
#include <QString>
#include <QSplashScreen>

#include "gm-companion/settings/settingsmanager.h"
#include "gm-companion/functions.h"
#include "gm-companion/ui/flowlayout.h"

// Includes QtWinExtras if system is Windows
#ifdef Q_OS_WIN
#include <QtWinExtras>
#endif

struct Unit{
    QString name;
    double refUnits;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QSplashScreen *splash, QWidget *parent = 0);
    ~MainWindow();
    void addToPlaylist(QUrl url, bool music); // Add audio file to playlist. Bool true if music, false if sound

    // Version
    void setVersion(QString); // Set Version Number (String)
    QString getVersion(); // Get Version Number (String), e.g. 1.3.1.0
    int getVersionNumber(); // Get Version Number (int), e.g. 1310
    void updateSettingsVersion(); // Sets version in settings to current version

    // Tools
    void addTools();

private slots:
    // Tools
    void on_pushButton_audioTool_clicked();
    void on_pushButton_mapTool_clicked();
    void on_pushButton_characters_clicked();
    void on_pushButton_generators_clicked();
    void on_pushButton_notes_clicked();
    void on_pushButton_unitConverter_clicked();
    void on_pushButton_settings_clicked();
//    void on_pushButton_addons_clicked();  // TODO
    void on_pushButton_combatTracker_clicked();
    void on_pushButton_shopTool_clicked();

private:
    Ui::MainWindow *ui;

    bool programStart = true;
    bool onStartUpdateCheck = false;

    int versionNumber;
    QString versionString;

    // Settings
    SettingsManager* settingsManager;
    void writeSettings(QString content, QString indicator, QString newSetting);
    void setFolderLocations(QString indicator);
    QString readSettings();
};

#endif // MAINWINDOW_H
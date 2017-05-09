#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
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
#include <QtWinExtras>
#include <QDebug>

#include "settingsmanager.h"
#include "dicemanager.h"
#include "functions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addToPlaylist(QUrl url, bool music);

    void setVersion(QString);
    QString getVersion();

    void createThumbnailToolbar();

private slots:
    void playMusic(QString);
    void playSound(QString);
    void setMap(QString mapPath);

    void updateMetaData();
    void updateProgressBar();
    void on_metaPlayerGotMetadata();

    void on_musicPauseButton_clicked();
    void on_musicPlayButton_clicked();
    void on_musicReplayButton_clicked();
    void on_musicNextButton_clicked();

    void on_setMusicFolder_clicked();
    void on_setSoundFolder_clicked();
    void on_setMapsFolder_clicked();
    void on_setResourcesFolder_clicked();

    void on_menuGM_Help_triggered();
    void on_menuMusic_triggered();
    void on_menuSound_triggered();
    void on_menuMaps_triggered();

    void on_actionMusicPlayer_clicked();
    void on_actionSoundPlayer_clicked();
    void on_actionMapsViewer_clicked();

    void on_actionDice_clicked();
    void on_actionDatabase_clicked();

    void on_openWiki_clicked();
    void on_checkForUpdates_clicked();
    void on_reportABug_clicked();
    void on_iWantToUseAnOlderVersionClicked();

    void on_tableDoubleClicked(int,int);

    void on_soundPlayButton_clicked();
    void on_soundPauseButton_clicked();
    void on_soundReplayButton_clicked();
    void on_soundNextButton_clicked();

    void on_musicVolumeSlider_valueChanged(int value);
    void on_soundVolumeSlider_valueChanged(int value);

    void on_mapsZoomInButton_clicked();
    void on_mapsZoomOutButton_clicked();
    void on_mapsFitToViewButton_clicked();
    void on_mapsResetSizeButton_clicked();

    void on_networkAccessManagerFinished(QNetworkReply*);

    void on_radioMetaDataChanged();

    void on_rivendellPlayButton_clicked();
    void on_rivendellReloadButton_clicked();
    void on_actionRadio_clicked();

    void on_radioTimer_timeout();
    void on_radioNetworkAccessManager_finished(QNetworkReply*);

    void on_mmorpgPlayButton_clicked();
    void on_mmorpgReloadButton_clicked();

    void on_generateNames(QString);
    void on_actionNameGenerator_clicked();
    void on_checkForUpdatesOnStart(bool);

    void on_characterListClicked(int);

    void on_actionCharacters_clicked();

    void on_createCharacterButton_clicked();

    void on_updateCharactersButton_clicked();
    void charactersFolderChanged();
    void charactersTimerFinished();

    void on_pushButton_clicked();
    void on_musicRandomButton_clicked();

    void on_soundRandomButton_clicked();

private:
    Ui::MainWindow *ui;

    bool programStart = true;

    void generateMusicButtons();
    void generateSoundButtons();
    void generateMaps();
    QTabWidget *tabWidgetMusic;
    QTabWidget *tabWidgetSound;

    SettingsManager* settingsManager;
    DiceManager* diceManager;

    void generateNamesTab();

    int versionNumber;
    QString versionString;

    QSignalMapper *signalMapperMusic;
    QSignalMapper *signalMapperSound;
    QSignalMapper *signalMapperMaps;
    QSignalMapper *signalMapperNames;

    QNetworkAccessManager *networkManager;

    //Radio
    QMediaPlayer *radioPlayer;
    QNetworkAccessManager *radioNetworkManager;
    QTimer *radioTimer;
    int radioTimerDuration;
    bool radioActive;
    int radioID;

    // Music
    QHBoxLayout *tabMusicLayout;

    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicPlaylist;
    QMediaPlayer *metaPlayer;
    QMediaPlaylist *metaPlaylist;

    QTabWidget* musicTabWidget;
    QTableWidget *musicTable;
    QTreeWidgetItem *folderItem;

    QMediaPlayer *soundPlayer;
    QMediaPlaylist *soundPlaylist;

    QScrollArea *mapsScrollArea;
    QLabel *mapsImageLabel;
    double mapsZoomFactor = 1.0;

    void writeSettings(QString content, QString indicator, QString newSetting);
    void setFolderLocations(QString indicator);
    QString readSettings();

    bool initialMusicPlay;

    // Characters
    void updateCharacters();
    QTimer* updateCharactersTimer;
    bool listenForCharacterUpdaters = true;

    // ThumbnailToolbar
    QWinThumbnailToolButton* playToolButton;
    QWinThumbnailToolButton* pauseToolButton;
    QWinThumbnailToolButton* nextToolButton;
};

#endif // MAINWINDOW_H

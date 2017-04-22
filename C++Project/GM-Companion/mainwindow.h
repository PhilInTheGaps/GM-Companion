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

QStringList getFiles(QString folder);
QStringList getFolders(QString path);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addToPlaylist(QUrl url, bool music);

    void setVersion(QString);
    QString getVersion();

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
    void on_setDatabasePath_clicked();

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

    void rollDice(int);
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
private:
    Ui::MainWindow *ui;

    void generateNamesTab();

    int versionNumber;
    QString versionString;

    QString musicPath;
    QString soundPath;
    QString mapsPath;
    QString resourcesPath;
    QString checkForUpdatesOnStart;

    QSignalMapper *signalMapperMusic;
    QSignalMapper *signalMapperSound;
    QSignalMapper *signalMapperMaps;
    QSignalMapper *signalMapperDice;
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

    QTableWidget *musicTable;
    QTreeWidgetItem *folderItem;

    QMediaPlayer *soundPlayer;
    QMediaPlaylist *soundPlaylist;

    void generateDiceFrame();

    QScrollArea *mapsScrollArea;
    QLabel *mapsImageLabel;
    double mapsZoomFactor = 1.0;

    void writeSettings(QString content, QString indicator, QString newSetting);
    void setFolderLocations(QString indicator);
    QString readSettings();

    bool initialMusicPlay;
};

#endif // MAINWINDOW_H

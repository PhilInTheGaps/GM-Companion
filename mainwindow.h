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
#include <QFileSystemWatcher>

#include "settingsmanager.h"
#include "dicemanager.h"
#include "functions.h"

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addToPlaylist(QUrl url, bool music); // Add audio file to playlist. Bool true if music, false if sound

    void setVersion(QString); // Set Version Number (String)
    QString getVersion(); // Get Version Number (String), e.g. 1.3.1.0
    int getVersionNumber(); // Get Version Number (int), e.g. 1310
    void updateSettingsVersion(); // Sets version in settings to current version

    void regenerateButtons();

    void getNotes();

    #ifdef Q_OS_WIN
    void createThumbnailToolbar();
    #endif

private slots:
    // Music
    void playMusic(QString);
    void updateMetaData();
    void updateProgressBar();
    void on_musicPauseButton_clicked();
    void on_musicPlayButton_clicked();
    void on_musicReplayButton_clicked();
    void on_musicNextButton_clicked();
    void on_musicRandomButton_clicked();
    void on_tableDoubleClicked(int);
    void on_musicVolumeSlider_valueChanged(int value);
    void on_musicProgressSlider_sliderReleased();
    void on_musicTitleComboBox_currentIndexChanged(int index);

    // Sound
    void playSound(QString);
    void on_soundPlayButton_clicked();
    void on_soundPauseButton_clicked();
    void on_soundReplayButton_clicked();
    void on_soundNextButton_clicked();
    void on_soundRandomButton_clicked();
    void on_soundVolumeSlider_valueChanged(int value);

    // Maps
    void setMap(QString mapPath);
    void on_mapsZoomInButton_clicked();
    void on_mapsZoomOutButton_clicked();
    void on_mapsFitToViewButton_clicked();
    void on_mapsResetSizeButton_clicked();

    // Options
    void on_actionOptions_triggered(); // OptionsDialog
    void on_actionSet_Music_Folder_triggered(); // Music
    void on_actionSet_Sound_Folder_triggered(); // Sound
    void on_actionSet_Maps_Folder_triggered(); // Maps
    void on_actionSet_Resources_Folder_triggered(); // Resources
    void on_actionSet_Characters_Folder_triggered(); // Characters
    void on_actionCheck_for_Updates_triggered(); // Check Updates

    // Name Generator
    void on_generateNames(QString);

    // Characters
    void on_characterListClicked(int);
    void on_createCharacterButton_clicked();
    void on_updateCharactersButton_clicked();
    void on_editCharacter_clicked();
    void on_deleteCharacterButton_clicked();

    // Notes
    void on_notesTree_itemClicked(QTreeWidgetItem *item, int column);
    void on_notesTextEdit_textChanged();
    void on_addNoteButton_clicked();
    void on_addCategoryButton_clicked();
    void on_deleteNoteButton_clicked();
    void on_deleteCategoryButton_clicked();
    void notesWatcher_directoryChanged();

    // Help
    void on_actionOpen_Wiki_triggered(); // Open Wiki
    void on_actionReport_a_Bug_triggered(); // Report Bug
    void actionI_want_to_use_an_older_Version_triggered(); // Older Version

    // Network
    void on_versionNetworkAccessManagerFinished(QNetworkReply*); // Get Newest Version Number
    void on_blogNetworkAccessManagerFinished(QNetworkReply*); // Get Blog Entries

    // Radío
    void on_radioMetaDataChanged();
    void displayRadios();
    void on_addRadioButton_clicked();
    void on_radioPlayButton_clicked();

    // Converter
    void on_addUnitButton_clicked();
    void on_converterTypeComboBox_currentIndexChanged(int index);
    void on_length1ComboBox_currentIndexChanged(int index);
    void on_length2ComboBox_currentIndexChanged(int index);
    void on_area1ComboBox_currentIndexChanged(int index);
    void on_area2ComboBox_currentIndexChanged(int index);
    void on_length1SpinBox_valueChanged(double value);
    void on_area1SpinBox_valueChanged(double value);
    void on_volume1ComboBox_currentIndexChanged(int index);
    void on_volume2ComboBox_currentIndexChanged(int index);
    void on_volume1SpinBox_valueChanged(double arg1);
    void on_weight1ComboBox_currentIndexChanged(int index);
    void on_weight2ComboBox_currentIndexChanged(int index);
    void on_weight1SpinBox_valueChanged(double arg1);
    void on_money1ComboBox_currentIndexChanged(int index);
    void on_money2ComboBox_currentIndexChanged(int index);
    void on_money1SpinBox_valueChanged(double arg1);

    void on_notesEncryptButton_clicked();

private:
    Ui::MainWindow *ui;

    bool programStart = true;
    bool onStartUpdateCheck = false;

    int versionNumber;
    QString versionString;

    // Music
    QTabWidget *tabWidgetMusic;
    QSignalMapper *signalMapperMusic;
    QHBoxLayout *tabMusicLayout;
    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicPlaylist;
    QTabWidget* musicTabWidget;
    QTableWidget *musicTable;
    void generateMusicButtons();
    bool initialMusicPlay;

    // Sound
    QTabWidget *tabWidgetSound;
    QSignalMapper *signalMapperSound;
    QMediaPlayer *soundPlayer;
    QMediaPlaylist *soundPlaylist;
    void generateSoundButtons();

    // Maps
    QSignalMapper *signalMapperMaps;
    QScrollArea *mapsScrollArea;
    QLabel *mapsImageLabel;
    double mapsZoomFactor = 1.0;
    void generateMaps();

    // Settings
    SettingsManager* settingsManager;
    void writeSettings(QString content, QString indicator, QString newSetting);
    void setFolderLocations(QString indicator);
    QString readSettings();

    // Dice
    DiceManager* diceManager;

    // Name Generator
    QSignalMapper *signalMapperNames;
    void generateNamesTab();

    // Notes
    void readNotes(QString file);
    void addNotes();
    void addCategory();
    void deleteNotes();
    void deleteCategory();
    QFileSystemWatcher* notesWatcher;

    // Network
    QNetworkAccessManager *versionNetworkManager;
    QNetworkAccessManager *blogNetworkManager;

    //Radio
    QMediaPlayer *radioPlayer;
    bool radioActive;
    int radioID;
    QStringList radioURLs;

    // Characters
    void updateCharacters();
    QTimer* updateCharactersTimer;
    bool listenForCharacterUpdaters = true;

    // Converter
    QList<Unit> lengthUnits;
    QList<Unit> areaUnits;
    QList<Unit> volumeUnits;
    QList<Unit> weightUnits;
    QList<Unit> moneyUnits;
    void initializeUnits();
    void convertLength(int index1, int index2, double units);
    void convertArea(int index1, int index2, double units);
    void convertVolume(int index1, int index2, double units);
    void convertWeight(int index1, int index2, double units);
    void convertMoney(int index1, int index2, double units);

    // ThumbnailToolbar (Windows Only)
    #ifdef Q_OS_WIN
    QWinThumbnailToolButton* playToolButton;
    QWinThumbnailToolButton* pauseToolButton;
    QWinThumbnailToolButton* nextToolButton;
    #endif
};

#endif // MAINWINDOW_H

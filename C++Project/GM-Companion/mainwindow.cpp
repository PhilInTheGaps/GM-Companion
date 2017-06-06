#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "characters.h"
#include "filemanager.h"
#include "optionsdialog.h"
#include "sifrp.h"

#include <QStringList>
#include <cstdlib>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QSettings>

#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    qDebug() << "Starting GM-Companion...";
    ui->setupUi(this);

    setVersion("0.3.1.0");

    // Copy files to a writable directory if they do not exist
    FileManager* fileManager = new FileManager;
    fileManager->copyFiles();

    // Initialize SettingsManager
    qDebug() << "Initializing settings...";
    settingsManager = new SettingsManager;

    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    int checkUpdates = checkSettings.value("checkForUpdatesOnStart", 1).toInt();

    // Initialize Signal Mappers
    signalMapperMusic = new QSignalMapper(this);
    signalMapperSound = new QSignalMapper(this);
    signalMapperMaps = new QSignalMapper(this);
    signalMapperNames = new QSignalMapper(this);

    // Generates the dice page
    diceManager = new DiceManager;
    ui->tabDice->layout()->addWidget(diceManager->generateDiceFrame());

    // Initialize player and playlist for music
    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setVolume(ui->musicVolumeSlider->value());

    musicPlaylist = new QMediaPlaylist();
    musicPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random);
    musicPlayer->setPlaylist(musicPlaylist);

    connect(musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updateProgressBar()));
    connect(musicPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(updateMetaData()));

    // Creates TableWidget to display song titles
    initialMusicPlay = true;
    musicTable = new QTableWidget;
    musicTable->setMaximumWidth(250);
    musicTable->setColumnWidth(0, 250);
    musicTable->setColumnCount(1);
    musicTable->setHorizontalHeaderLabels(QString("Title").split(";"));
    connect(musicTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_tableDoubleClicked(int)));

    // Generate Music Buttons
    generateMusicButtons();
    connect(signalMapperMusic, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));

    // Initialize player and playlist for sound
    soundPlayer = new QMediaPlayer(this);
    soundPlayer->setVolume(ui->soundVolumeSlider->value());

    soundPlaylist = new QMediaPlaylist();
    soundPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random);
    soundPlayer->setPlaylist(soundPlaylist);

    // Generate Sound Buttons
    generateSoundButtons();
    connect(signalMapperSound, SIGNAL(mapped(QString)), this, SLOT(playSound(QString)));

    // Notes
    getNotes();
    notesWatcher = new QFileSystemWatcher;
    notesWatcher->addPath(QDir::homePath()+"/.gm-companion/notes");
    connect(notesWatcher, SIGNAL(directoryChanged(QString)), SLOT(notesWatcher_directoryChanged()));

    // Addons
    qDebug() << "Getting Addons...";
    SIFRP* sifrp = new SIFRP;
    ui->tabWidgetGMHelp->addTab(sifrp, "SIFRP");

    // Initialize Radio
    qDebug() << "Initializing Radio...";
    radioPlayer = new QMediaPlayer(this);
    radioPlayer->setVolume(ui->musicVolumeSlider->value());

    connect(radioPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(on_radioMetaDataChanged()));

    // Initialize Name Generator
    generateNamesTab();

    // Check if openSSL is installed
    qDebug() << "SSL is supported:  " << QSslSocket::supportsSsl();
    if (!QSslSocket::supportsSsl())
        qDebug() << "Please install openSSL";

    // Initialize Version and Blog Network Managers
    versionNetworkManager = new QNetworkAccessManager;
    connect(versionNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_versionNetworkAccessManagerFinished(QNetworkReply*)));
    blogNetworkManager = new QNetworkAccessManager;
    connect(blogNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_blogNetworkAccessManagerFinished(QNetworkReply*)));

    // Initialize Maps Viewer
    generateMaps();
    connect(signalMapperMaps, SIGNAL(mapped(QString)), this, SLOT(setMap(QString)));

    // Initialize Character View
    QStringList characterList = getCharacterList();
    if (!characterList.empty()){
        for (QString character : characterList){
            QWidget* widget = getCharacterPage(character);
            ui->charactersStackedWidget->addWidget(widget);

            QListWidgetItem *listItem = new QListWidgetItem;
            listItem->setText(widget->toolTip());
            listItem->setToolTip(cleanText(character));
            ui->charactersListWidget->addItem(listItem);
        }
        on_characterListClicked(0);
    }
    connect(ui->charactersListWidget, SIGNAL(currentRowChanged(int)), SLOT(on_characterListClicked(int)));

    // Checks for updates on program start
    if (checkUpdates == 1){
        onStartUpdateCheck = true;
        on_actionCheck_for_Updates_triggered();
    }

    // Get Blog Feed
    qDebug() << "Getting blog feed...";
    blogNetworkManager->get(QNetworkRequest(QUrl("https://philinthegaps.github.io/GM-Companion/feed.xml")));

    // Some functions behave differently when the program is just starting
    programStart = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Add audio file to correct playlist
void MainWindow::addToPlaylist(QUrl url, bool music){
    if (music){
        musicPlaylist->addMedia(url);
    }
    else{
        soundPlaylist->addMedia(url);
    }
}

// Open Wiki Page in Web Browser
void MainWindow::on_actionOpen_Wiki_triggered(){
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/wiki"));
}

// Set Program version
void MainWindow::setVersion(QString versionAsString){
    versionString = versionAsString;
    versionNumber = 0;
    QString temp = versionAsString.replace(".", "");
    versionNumber = temp.toInt();

    qDebug() << "Version: "+versionString;

}

// Opens Issues GitHub Page
void MainWindow::on_actionReport_a_Bug_triggered(){
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/issues/new"));
}

// Opens Releases GitHub Page
void MainWindow::actionI_want_to_use_an_older_Version_triggered(){
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/releases"));
}

// Set Music Path
void MainWindow::on_actionSet_Music_Folder_triggered(){
    settingsManager->setSetting(Setting::musicPath, true);
    QLayoutItem *child;
    while ((child = ui->pageMusic->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    musicPlayer->stop();
    musicPlaylist->clear();
    tabWidgetMusic = NULL;
    initialMusicPlay = true;
    generateMusicButtons();
}

// Set Sound Path
void MainWindow::on_actionSet_Sound_Folder_triggered(){
    settingsManager->setSetting(Setting::soundPath, true);
    QLayoutItem *child;
    while ((child = ui->pageSound->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    soundPlayer->stop();
    soundPlaylist->clear();
    tabWidgetSound = NULL;
    generateSoundButtons();
}

// Set Maps Path
void MainWindow::on_actionSet_Maps_Folder_triggered(){
    settingsManager->setSetting(Setting::mapsPath, true);
    QLayoutItem *child;
    while ((child = ui->mapsVBox->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    generateMaps();
}

// Set Characters Path
void MainWindow::on_actionSet_Characters_Folder_triggered(){
    settingsManager->setSetting(Setting::charactersPath, true);
    updateCharacters();
}

// Set resources path
void MainWindow::on_actionSet_Resources_Folder_triggered(){
    settingsManager->setSetting(Setting::resourcesPath, true);

    QLayoutItem *child;
    while ((child = ui->pageSound->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    soundPlayer->stop();
    soundPlaylist->clear();
    tabWidgetSound = NULL;
    generateSoundButtons();

    while ((child = ui->pageMusic->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    musicPlayer->stop();
    musicPlaylist->clear();
    tabWidgetMusic = NULL;
    initialMusicPlay = true;
    generateMusicButtons();
}

//// Toggle Check for Updates on program start
//void MainWindow::on_actionCheck_for_Updates_on_Program_Start_triggered(bool checked){
//    if (checked){
//        settingsManager->setSetting(Setting::checkForUpdatesOnStart, 0);
//    }else{
//        settingsManager->setSetting(Setting::checkForUpdatesOnStart, 1);
//    }
//}

// Change to GM-Help
void MainWindow::on_menuGM_Help_triggered(){
    ui->stackedWidget->setCurrentIndex(0);
}

// Change to Dice
void MainWindow::on_actionDice_triggered(){
    ui->tabWidgetGMHelp->setCurrentIndex(0);
}

// Change to Name Generator
void MainWindow::on_actionName_Generator_triggered(){
    ui->tabWidgetGMHelp->setCurrentIndex(2);
}

// Change to Characters
void MainWindow::on_actionCharacters_triggered(){
    ui->tabWidgetGMHelp->setCurrentIndex(3);
}

// Change to Music
void MainWindow::on_menuMusic_triggered(){
    ui->stackedWidget->setCurrentIndex(1);
}

// Change to Sounds
void MainWindow::on_menuSound_triggered(){
    ui->stackedWidget->setCurrentIndex(2);
}

// Change to Maps
void MainWindow::on_menuMaps_triggered(){
    ui->stackedWidget->setCurrentIndex(3);
}

// Open Options Dialog
void MainWindow::on_actionOptions_triggered(){
    OptionsDialog* options = new OptionsDialog(this);
    options->show();
}

// Change to Notes
void MainWindow::on_actionNotes_triggered(){
    ui->tabWidgetGMHelp->setCurrentIndex(1);
}

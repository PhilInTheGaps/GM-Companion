#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools/characters.h"
#include "managers/filemanager.h"
#include "dialogs/optionsdialog.h"
#include "addontools/sifrp.h"

#include "tools/audiotool.h"
#include "tools/mapviewertool.h"
#include "tools/dicetool.h"

#include <QStringList>
#include <cstdlib>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QScrollBar>
#include <QApplication>
#include <QSplashScreen>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    qDebug() << tr("Starting GM-Companion...");
    ui->setupUi(this);

    setVersion("0.3.2.0");

    // Copy files to a writable directory if they do not exist
    FileManager* fileManager = new FileManager;
    fileManager->copyFiles();

    // Initialize SettingsManager
    qDebug() << tr("Initializing settings...");
    settingsManager = new SettingsManager;

    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    int checkUpdates = checkSettings.value("checkForUpdatesOnStart", 1).toInt();

    // Set tool tabs closeable
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    // Initialize Signal Mappers
    signalMapperNames = new QSignalMapper(this);

    // Notes
    getNotes();
    notesWatcher = new QFileSystemWatcher;
    notesWatcher->addPath(settingsManager->getSetting(Setting::notesPath));
    connect(notesWatcher, SIGNAL(directoryChanged(QString)), SLOT(notesWatcher_directoryChanged()));

    // Addons
    qDebug() << tr("Getting Addons...");
    if (settingsManager->getIsAddonEnabled("SIFRP")){
        SIFRP* sifrp = new SIFRP(this);
        ui->tabWidget->addTab(sifrp, "SIFRP");
    }

    // Initialize Name Generator
    generateNamesTab();

    // Check if openSSL is installed
    qDebug() << tr("Checking SSL installation...");
    if (!QSslSocket::supportsSsl()){
        qDebug() << tr("Please install openSSL");
    }else{
        qDebug() << tr("SSL is installed.");
    }

    // Initialize Version and Blog Network Managers
    versionNetworkManager = new QNetworkAccessManager;
    connect(versionNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_versionNetworkAccessManagerFinished(QNetworkReply*)));
    blogNetworkManager = new QNetworkAccessManager;
    connect(blogNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_blogNetworkAccessManagerFinished(QNetworkReply*)));

    //Initialize Character View
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

    // Initialize Converter
    initializeUnits();

    // Checks for updates on program start
    if (checkUpdates == 1){
        onStartUpdateCheck = true;
        on_actionCheck_for_Updates_triggered();
    }

    // Get Blog Feed
    qDebug() << tr("Getting blog feed...");
    blogNetworkManager->get(QNetworkRequest(QUrl("https://philinthegaps.github.io/GM-Companion/feed.xml")));
    ui->blogTextEdit->verticalScrollBar()->setValue(0);

    // Some functions behave differently when the program is just starting
    programStart = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Add Tools
void MainWindow::addTools()
{
    // AudioTool
    AudioTool *audioTool = new AudioTool(settingsManager, this);
    ui->tabWidget->insertTab(1, audioTool, "Audio Tool");

    // MapTool
    MapViewerTool *mapViewerTool = new MapViewerTool;
    ui->tabWidget->insertTab(2, mapViewerTool, "Map Tool");

    // DiceTool
    DiceTool *diceTool = new DiceTool;
    ui->tabWidget->insertTab(3, diceTool, "Dice Tool");
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

    qDebug() << tr("Version: ")+versionString;

}

// Returns version as String with dots
QString MainWindow::getVersion(){
    return versionString;
}

// Returns version as int
int MainWindow::getVersionNumber(){
    return versionNumber;
}

// Update the settings version
void MainWindow::updateSettingsVersion(){
    settingsManager->setSetting(Setting::version, true, QString::number(versionNumber));
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
}

// Set Sound Path
void MainWindow::on_actionSet_Sound_Folder_triggered(){
    settingsManager->setSetting(Setting::soundPath, true);
}

// Set Maps Path
void MainWindow::on_actionSet_Maps_Folder_triggered(){
    settingsManager->setSetting(Setting::mapsPath, true);
}

// Set Characters Path
void MainWindow::on_actionSet_Characters_Folder_triggered(){
    settingsManager->setSetting(Setting::charactersPath, true);
    updateCharacters();
}

// Set resources path
void MainWindow::on_actionSet_Resources_Folder_triggered(){
    settingsManager->setSetting(Setting::resourcesPath, true);
}

// Set Notes Path
void MainWindow::on_actionSet_Notes_Folder_triggered()
{
    settingsManager->setSetting(Setting::notesPath, true);

    getNotes();
}

// Set Audio Projects Path
void MainWindow::on_actionSet_Audio_Projects_Folder_triggered()
{
    settingsManager->setSetting(audioPath, true);
}

// Set Radio Playlists Path
void MainWindow::on_actionSet_Radio_Playlists_Folder_triggered()
{
    settingsManager->setSetting(radioPath, true);
}

// Open Options Dialog
void MainWindow::on_actionOptions_triggered(){
    OptionsDialog* options = new OptionsDialog(this);
    options->show();
}

// Remove a tab from the tab widget
void MainWindow::closeTab(int index){
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionToggle_Maps_Tool_triggered()
{
    qDebug() << "Adding Map Viewer Tool...";
    MapViewerTool *mapViewerTool = new MapViewerTool;
    ui->tabWidget->addTab(mapViewerTool, "Map Tool");
}

void MainWindow::on_actionToggle_Dice_Tool_triggered()
{
    qDebug() << "Adding AudioTool ...";
    AudioTool *audioTool = new AudioTool(settingsManager, this);
    ui->tabWidget->addTab(audioTool, "Audio Tool");
}

void MainWindow::on_actionToggle_Name_Generator_Tool_toggled(bool arg1)
{
    if (arg1){
        qDebug() << "Adding name generator tool...";
    }
}

void MainWindow::on_actionToggle_Characters_Tool_toggled(bool arg1)
{
    if (arg1){
        qDebug() << "Adding characters tool...";
    }
}

void MainWindow::on_actionToggle_Notes_Tool_toggled(bool arg1)
{
    if (arg1){
        qDebug() << "Adding notes tool...";
    }
}

void MainWindow::on_actionToggle_Unit_Converter_Tool_toggled(bool arg1)
{
    if (arg1){
        qDebug() << "Adding unit converter tool...";
    }
}

// Add Audio Tool
void MainWindow::on_actionAdd_Audio_Tool_triggered()
{
    qDebug() << "Adding AudioTool ...";
    AudioTool *audioTool = new AudioTool(settingsManager, this);
    ui->tabWidget->addTab(audioTool, "Audio Tool");
}

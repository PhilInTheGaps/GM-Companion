#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogs/optionsdialog.h"
#include "addontools/sifrp.h"
#include "dialogs/aboutdialog.h"

#include "tools/audiotool.h"
#include "tools/mapviewertool.h"
#include "tools/dicetool.h"
#include "tools/characterviewertool.h"
#include "tools/notestool.h"
#include "tools/convertertool.h"

#include "managers/filemanager.h"
#include "managers/generatormanager.h"
#include "managers/updatemanager.h"

#include <QStringList>
#include <cstdlib>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QScrollBar>
#include <QApplication>
#include <QSplashScreen>
#include <QPlainTextEdit>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    // Copy files to a writable directory if they do not exist
    FileManager* fileManager = new FileManager;
    fileManager->copyFiles();

    // Initialize SettingsManager
    qDebug() << "Initializing settings...";
    settingsManager = new SettingsManager;

    // Set tool tabs closeable
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    // Initialize Blog Network Manager
    blogNetworkManager = new QNetworkAccessManager;
    connect(blogNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_blogNetworkAccessManagerFinished(QNetworkReply*)));

    // Get Blog Feed
    qDebug() << "Getting blog feed...";
    blogNetworkManager->get(QNetworkRequest(QUrl("https://gm-companion.github.io/feed.xml")));
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
    qDebug() << "Loading tools ...";

    // AudioTool
    AudioTool *audioTool = new AudioTool(settingsManager, this);
    ui->tabWidget->insertTab(1, audioTool, tr("Audio Tool"));

    // MapTool
    MapViewerTool *mapViewerTool = new MapViewerTool;
    ui->tabWidget->insertTab(2, mapViewerTool, tr("Map Tool"));

    // DiceTool
    DiceTool *diceTool = new DiceTool;
    ui->tabWidget->insertTab(3, diceTool, tr("Dice Tool"));

    // Character Viewer
    CharacterViewerTool *characterViewer = new CharacterViewerTool;
    ui->tabWidget->insertTab(4, characterViewer, tr("Characters"));

    // Generator Manager
    GeneratorManager *generatorManager = new GeneratorManager;
    ui->tabWidget->insertTab(5, generatorManager, tr("Generators"));

    // NotesTool
    NotesTool *notesTool = new NotesTool;
    ui->tabWidget->insertTab(6, notesTool, tr("Notes"));

    // ConverterTool
    ConverterTool *converterTool = new ConverterTool;
    ui->tabWidget->insertTab(7, converterTool, tr("Unit Converter"));
}

// Open Wiki Page in Web Browser
void MainWindow::on_actionOpen_Wiki_triggered(){
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/wiki"));
}

// Open Wiki
void MainWindow::on_pushButton_documentation_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/wiki"));
}

// Set Program version
void MainWindow::setVersion(QString versionAsString){
    versionString = versionAsString;
    versionNumber = 0;
    QString temp = versionAsString.replace(".", "");
    versionNumber = temp.toInt();

    qDebug().noquote() << "Version: " + versionString;
}

int MainWindow::getVersionNumber()
{
    return versionNumber;
}

QString MainWindow::getVersion()
{
    return versionString;
}

// Check for Updates
void MainWindow::on_actionCheck_for_Updates_triggered(){
    UpdateManager *updateManger = new UpdateManager(versionNumber);
    updateManger->checkForUpdates();
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
}

// Set resources path
void MainWindow::on_actionSet_Resources_Folder_triggered(){
    settingsManager->setSetting(Setting::resourcesPath, true);
}

// Set Notes Path
void MainWindow::on_actionSet_Notes_Folder_triggered()
{
    settingsManager->setSetting(Setting::notesPath, true);
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
    qDebug() << "Opening options window ...";
    OptionsDialog* options = new OptionsDialog(this);
    options->show();
}

// Remove a tab from the tab widget
void MainWindow::closeTab(int index){
    qDebug().noquote() << "Closing tab:" << ui->tabWidget->tabText(index) << "...";
    ui->tabWidget->removeTab(index);
}

// Add Maps Tool
void MainWindow::on_actionToggle_Maps_Tool_triggered()
{
    qDebug() << "Adding Map Viewer Tool...";
    MapViewerTool *mapViewerTool = new MapViewerTool;
    ui->tabWidget->addTab(mapViewerTool, tr("Map Tool"));
}

// Add Dice Tool
void MainWindow::on_actionToggle_Dice_Tool_triggered()
{
    qDebug() << "Adding AudioTool ...";
    AudioTool *audioTool = new AudioTool(settingsManager, this);
    ui->tabWidget->addTab(audioTool, tr("Audio Tool"));
}

// Add Character Tool
void MainWindow::on_actionToggle_Characters_Tool_triggered()
{
    qDebug() << "Adding characters tool...";
    CharacterViewerTool *characterViewer = new CharacterViewerTool;
    ui->tabWidget->addTab(characterViewer, tr("Characters"));
}

// Add Audio Tool
void MainWindow::on_actionAdd_Audio_Tool_triggered()
{
    qDebug() << "Adding AudioTool ...";
    AudioTool *audioTool = new AudioTool(settingsManager, this);
    ui->tabWidget->addTab(audioTool, tr("Audio Tool"));
}

// Add Notes Tool
void MainWindow::on_actionNotes_triggered()
{
    qDebug() << "Adding NotesTool ...";
    NotesTool *notesTool = new NotesTool;
    ui->tabWidget->addTab(notesTool, tr("Notes"));
}

// Add Converter Tool
void MainWindow::on_actionUnit_Converter_triggered()
{
    qDebug() << "Adding ConverterTool ...";
    ConverterTool *converterTool = new ConverterTool;
    ui->tabWidget->addTab(converterTool, tr("Unit Converter"));
}

// Change blog settings
void MainWindow::on_radioButton_allEntries_toggled(bool checked)
{
    if (checked)
    {
        qDebug() << "Getting blog feed ...";
        blogNetworkManager->get(QNetworkRequest(QUrl("https://gm-companion.github.io/feed.xml")));
    }
}

void MainWindow::on_radioButton_releaseOnly_toggled(bool checked)
{
    if (checked)
    {
        qDebug() << "Getting blog feed ...";
        blogNetworkManager->get(QNetworkRequest(QUrl("https://gm-companion.github.io/releases.xml")));
    }
}

void MainWindow::on_actionView_on_GitHub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion"));
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *about = new AboutDialog(versionString);
    about->show();
}

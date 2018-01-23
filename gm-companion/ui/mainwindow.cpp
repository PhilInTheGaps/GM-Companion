#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gm-companion/settings/optionsdialog.h"
#include "gm-companion/ui/aboutdialog.h"

#include "gm-companion/tools/addons/sifrp.h"
#include "gm-companion/tools/audio/audiotool.h"
#include "gm-companion/tools/mapviewertool.h"
#include "gm-companion/tools/dicetool.h"
#include "gm-companion/tools/characters/characterviewertool.h"
#include "gm-companion/tools/notes/notestool.h"
#include "gm-companion/tools/convertertool.h"
#include "gm-companion/tools/combattracker.h"
#include "gm-companion/tools/items/shoptool.h"
#include "gm-companion/tools/generators/generatormanager.h"

#include "gm-companion/managers/filemanager.h"
#include "gm-companion/managers/updatemanager.h"

#include <QStringList>
#include <cstdlib>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QApplication>
#include <QSplashScreen>
#include <QPlainTextEdit>
#include <QDebug>
#include <QPushButton>
#include <QSizePolicy>

MainWindow::MainWindow(QSplashScreen *splash, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    // Copy files to a writable directory if they do not exist
    FileManager* fileManager = new FileManager;
    fileManager->run();

    // Initialize SettingsManager
    qDebug() << "Initializing settings...";
    settingsManager = new SettingsManager;

    // Hide menu labels and resize buttons
    if (settingsManager->getSetting(Setting::showToolNames).toInt() == 0)
    {
        ui->label_audio->hide();
        ui->label_maps->hide();
        ui->label_combat->hide();
        ui->label_dice->hide();
        ui->label_shop->hide();
        ui->label_characters->hide();
        ui->label_generators->hide();
        ui->label_notes->hide();
        ui->label_converter->hide();
        ui->label_options->hide();
    }

    // Some functions behave differently when the program is just starting
    programStart = false;

    qDebug().noquote() << "Closing splash screen ...";
    splash->close();
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
    ui->stackedWidget->addWidget(audioTool);

    // MapTool
    MapViewerTool *mapViewerTool = new MapViewerTool;
    ui->stackedWidget->addWidget(mapViewerTool);

    // Combat Tracker
    CombatTracker *combatTracker = new CombatTracker;
    ui->stackedWidget->addWidget(combatTracker);

    // Character Viewer
    CharacterViewerTool *characterViewer = new CharacterViewerTool;
    ui->stackedWidget->addWidget(characterViewer);

    // Generator Manager
    GeneratorManager *generatorManager = new GeneratorManager;
    ui->stackedWidget->addWidget(generatorManager);

    // NotesTool
    NotesTool *notesTool = new NotesTool;
    ui->stackedWidget->addWidget(notesTool);

    // ConverterTool
    ConverterTool *converterTool = new ConverterTool;
    ui->stackedWidget->addWidget(converterTool);

    // Shop Tool
    ShopTool *shopTool = new ShopTool;
    ui->stackedWidget->addWidget(shopTool);

    // Addons Tool
    // TODO

    // Options Dialog
    OptionsDialog *optionsDialog = new OptionsDialog(this);
    ui->stackedWidget->addWidget(optionsDialog);

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

// Update the settings version
void MainWindow::updateSettingsVersion(){
    settingsManager->setSetting(Setting::version, true, QString::number(versionNumber));
}

/// Tool Navigation
// Audio Tool
void MainWindow::on_pushButton_audioTool_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// Map Viewer Tool
void MainWindow::on_pushButton_mapTool_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Combat Tracker
void MainWindow::on_pushButton_combatTracker_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Characters
void MainWindow::on_pushButton_characters_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// Generators
void MainWindow::on_pushButton_generators_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

// Notes
void MainWindow::on_pushButton_notes_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

// Unit Converter
void MainWindow::on_pushButton_unitConverter_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

// Item Shop
void MainWindow::on_pushButton_shopTool_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

//// Addons
//void MainWindow::on_pushButton_addons_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(8);
//}

// Settings
void MainWindow::on_pushButton_settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count()-1);
}

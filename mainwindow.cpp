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
    ui->stackedWidget->addWidget(audioTool);

    // MapTool
    MapViewerTool *mapViewerTool = new MapViewerTool;
    ui->stackedWidget->addWidget(mapViewerTool);

    // DiceTool
    DiceTool *diceTool = new DiceTool;
    ui->stackedWidget->addWidget(diceTool);

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

// Dice Tool
void MainWindow::on_pushButton_diceTool_clicked()
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

// Settings
void MainWindow::on_pushButton_settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->count()-1);
}

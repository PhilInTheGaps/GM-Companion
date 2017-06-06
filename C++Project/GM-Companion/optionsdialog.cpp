#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "ui_mainwindow.h"
#include "functions.h"

#include <QDebug>
#include <QSettings>
#include <QLabel>

OptionsDialog::OptionsDialog(MainWindow *parent) : QDialog(parent), ui(new Ui::OptionsDialog){
    ui->setupUi(this);

    w = parent;

    settings = new SettingsManager;

    connect(this, SIGNAL(finished(int)), SLOT(onClose()));

    updatePaths();
    getAddons();

    // Check or uncheck "Check for updates on program start"
    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    int checkOnStart = checkSettings.value("checkForUpdatesOnStart", 1).toInt();
    if (checkOnStart == 1){
        ui->checkForUpdatesOnProgramStart->setChecked(true);
    } else{
        ui->checkForUpdatesOnProgramStart->setChecked(false);
    }
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

// Executed when windows closes
void OptionsDialog::onClose(){
    if (pathsChanged)
        w->regenerateButtons();
    writeAddonSettings();
}

// Gets every installed addon
void OptionsDialog::getAddons(){
    ui->addonsFrame->layout()->setAlignment(Qt::AlignTop);

    QStringList officialAddonNames = settings->getOfficialAddons();

    for (QString folder : getFolders(QDir::homePath()+"/.gm-companion/addons")){
        if (!folder.contains(".")){
            QCheckBox* b = new QCheckBox;
            b->setText(folder);

            b->setChecked(settings->getIsAddonEnabled(folder));

            if (officialAddonNames.contains(folder)){
                officialAddons.push_back(b);
            }else{
                inofficialAddons.push_back(b);
            }
        }
    }

    ui->addonsFrame->layout()->addWidget(new QLabel("Official Addons"));
    for (QCheckBox* b : officialAddons){
        ui->addonsFrame->layout()->addWidget(b);
    }
    ui->addonsFrame->layout()->addWidget(new QLabel("Inofficial Addons"));
    for (QCheckBox* b : inofficialAddons){
        ui->addonsFrame->layout()->addWidget(b);
    }
}

// Save Addon Settings
void OptionsDialog::writeAddonSettings(){
    for (QCheckBox* b : officialAddons){
        settings->setAddonEnabled(b->text(), b->isChecked());
    }
    for (QCheckBox* b : inofficialAddons){
        settings->setAddonEnabled(b->text(), b->isChecked());
    }
    w->getNotes();
}

// Generates Music and Sound Buttons
void MainWindow::regenerateButtons(){
//    qDebug() << "Paths changed, generating buttons...";

//    QLayoutItem *child;
//    while ((child = ui->pageSound->layout()->takeAt(0)) != 0) {
//        delete child->widget();
//        delete child;
//    }
//    soundPlayer->stop();
//    soundPlaylist->clear();
//    tabWidgetSound = NULL;
//    generateSoundButtons();

//    while ((child = ui->pageMusic->layout()->takeAt(0)) != 0) {
//        delete child->widget();
//        delete child;
//    }
//    musicPlayer->stop();
//    musicPlaylist->clear();
//    tabWidgetMusic = NULL;
//    initialMusicPlay = true;
//    generateMusicButtons();
}

void OptionsDialog::on_checkForUpdatesOnProgramStart_toggled(bool checked)
{
    settings->setSetting(Setting::checkForUpdatesOnStart, checked);
}

void OptionsDialog::on_setMusicPath_clicked()
{
    settings->setSetting(Setting::musicPath);
    pathsChanged = true;
    updatePaths();
}

void OptionsDialog::on_setSoundPath_clicked()
{
    settings->setSetting(Setting::soundPath);
    pathsChanged = true;
    updatePaths();
}

void OptionsDialog::on_setResourcesPath_clicked()
{
    settings->setSetting(Setting::resourcesPath);
    pathsChanged = true;
    updatePaths();
}

void OptionsDialog::on_setMapsPath_clicked()
{
    settings->setSetting(Setting::mapsPath);
    pathsChanged = true;
    updatePaths();
}

void OptionsDialog::on_setCharactersPath_clicked()
{
    settings->setSetting(Setting::charactersPath);
    pathsChanged = true;
    updatePaths();
}

void OptionsDialog::updatePaths(){
    ui->musicPath->setText(settings->getSetting(Setting::musicPath));
    ui->soundPath->setText(settings->getSetting(Setting::soundPath));
    ui->mapsPath->setText(settings->getSetting(Setting::mapsPath));
    ui->resourcesPath->setText(settings->getSetting(Setting::resourcesPath));
    ui->charactersPath->setText(settings->getSetting(Setting::charactersPath));
}


void OptionsDialog::on_closeButton_clicked()
{
    this->close();
}

void OptionsDialog::on_selectAll_clicked()
{
    for (QCheckBox* b : officialAddons){
        b->setChecked(true);
    }
    for (QCheckBox* b : inofficialAddons){
        b->setChecked(true);
    }
}

void OptionsDialog::on_deselectAll_clicked()
{
    for (QCheckBox* b : officialAddons){
        b->setChecked(false);
    }
    for (QCheckBox* b : inofficialAddons){
        b->setChecked(false);
    }
}

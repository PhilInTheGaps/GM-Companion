#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "ui_mainwindow.h"
#include "functions.h"

#include <QDebug>
#include <QSettings>
#include <QLabel>
#include <QFileDialog>
#include <QProcess>

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

    // Display correct style
    for (QString style : getFiles(QDir::homePath()+"/.gm-companion/styles")){
        bool exists = false;
        for (int i = 0; i<ui->styleComboBox->count(); i++){
            if (!exists && ui->styleComboBox->itemText(0) == cleanText(style)){
                exists = true;
            }
        }
        if (!exists){
            ui->styleComboBox->addItem(cleanText(style));
        }

        QString currentStyle = checkSettings.value("uiMode", "DarkStyle").toString();
        ui->styleComboBox->setCurrentText(currentStyle);
    }

    // Display current language
    QString lang = settings->getSetting(Setting::language);
    if (lang == "en"){
        ui->languageComboBox->setCurrentText("English");
    }else if (lang == "de"){
        ui->languageComboBox->setCurrentText("Deutsch");
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
    qDebug() << tr("Paths changed, generating buttons...");

    QLayoutItem *child;
    while ((child = ui->tabSound->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    soundPlayer->stop();
    soundPlaylist->clear();
    tabWidgetSound = NULL;
    generateSoundButtons();

    while ((child = ui->tabMusic->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    musicPlayer->stop();
    musicPlaylist->clear();
    tabWidgetMusic = NULL;
    initialMusicPlay = true;
    generateMusicButtons();
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

void OptionsDialog::on_setNotesPath_clicked()
{
    settings->setSetting(Setting::notesPath);
    pathsChanged = true;
    updatePaths();
}

void OptionsDialog::on_setAudioPath_clicked()
{
    settings->setSetting(audioPath);
}

void OptionsDialog::on_setRadioPath_clicked()
{
    settings->setSetting(radioPath);
}


void OptionsDialog::updatePaths(){
    ui->musicPath->setText(settings->getSetting(musicPath));
    ui->soundPath->setText(settings->getSetting(soundPath));
    ui->mapsPath->setText(settings->getSetting(mapsPath));
    ui->resourcesPath->setText(settings->getSetting(resourcesPath));
    ui->charactersPath->setText(settings->getSetting(charactersPath));
    ui->notesPath->setText(settings->getSetting(notesPath));
    ui->audioPath->setText(settings->getSetting(audioPath));
    ui->radioPath->setText(settings->getSetting(radioPath));
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

// Set new Style Sheet when another one is selected
void OptionsDialog::on_styleComboBox_currentTextChanged(const QString &arg1){
    settings->setSetting(Setting::uiMode, true, arg1);
}

// Set New Language
void OptionsDialog::on_languageComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        settings->setSetting(Setting::language, true, "en");
        break;
    case 1:
        settings->setSetting(Setting::language, true, "de");
        break;
    default:
        settings->setSetting(Setting::language, true, "en");
        break;
    }
}

// Open Addon Manager
void OptionsDialog::on_addonManagerButton_clicked()
{
    #ifdef __linux__
        QProcess::startDetached("java", {"-jar", "/usr/share/gm-companion/AddonManager.jar"});
    #elif _WIN32
        QProcess::startDetached("java", {"-jar", QApplication::applicationDirPath()+"/AddonManager.jar"});
    #else
        QProcess::startDetached("java", {"-jar", QApplication::applicationDirPath()+"/AddonManager.jar"});
        //qDebug() << QCoreApplication::translate("AddonManager", "This OS is not supported. Cannot launch Addon Manager.");
    #endif
}

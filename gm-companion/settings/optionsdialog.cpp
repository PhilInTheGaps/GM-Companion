#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "ui_mainwindow.h"
#include "gm-companion/functions.h"
#include "gm-companion/ui/aboutdialog.h"
#include "gm-companion/managers/updatemanager.h"
#include "gm-companion/tools/addons/addonmanager.h"

#include <QDebug>
#include <QSettings>
#include <QLabel>
#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>
#include <QComboBox>

OptionsDialog::OptionsDialog(MainWindow *parent) : QDialog(parent), ui(
        new Ui::OptionsDialog)
{
    ui->setupUi(this);

    w = parent;

    settings = new SettingsManager;

    updatePaths();

    getAddons();
    getStyleSheets();
    getLanguage();

    if (settings->getSetting(Setting::showToolNames).toInt() ==
        1) ui->checkBox_showToolNames->setChecked(true);
    else ui->checkBox_showToolNames->setChecked(false);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

// Display current language
void OptionsDialog::getLanguage()
{
    QString lang = settings->getSetting(Setting::language);

    if (lang == "en") ui->languageComboBox->setCurrentText("English");
    else if (lang == "de") ui->languageComboBox->setCurrentText("Deutsch");
}

// Add all custom StyleSheets to the combo box
void OptionsDialog::getStyleSheets()
{
    QStringList styles = getFiles(QDir::homePath() + "/.gm-companion/styles");

    qDebug().noquote() << "Found the following stylesheets:";

    for (int i = 0; i < styles.size(); i++)
    {
        qDebug().noquote() <<  "    " << styles.at(i);
        ui->styleComboBox->addItem(cleanText(styles.at(i)));
    }

    // Set combobox to default style
    int index = ui->styleComboBox->findText(settings->getSetting(uiMode));
    ui->styleComboBox->setCurrentIndex(index);
    qDebug().noquote() << "Default style:" << settings->getSetting(uiMode);
}

// Gets every installed addon
void OptionsDialog::getAddons()
{
    ui->addonsFrame->layout()->setAlignment(Qt::AlignTop);

    QStringList officialAddonNames = settings->getOfficialAddons();

    for (QString folder :
         getFolders(QDir::homePath() + "/.gm-companion/addons")) {
        if (!folder.contains(".")) {
            QCheckBox *b = new QCheckBox;
            b->setText(folder);

            b->setChecked(settings->getIsAddonEnabled(folder));

            if (officialAddonNames.contains(folder)) {
                officialAddons.push_back(b);
            } else {
                inofficialAddons.push_back(b);
            }
        }
    }

    ui->addonsFrame->layout()->addWidget(new QLabel(tr("Official Addons")));

    for (QCheckBox *b : officialAddons) {
        ui->addonsFrame->layout()->addWidget(b);
    }
    ui->addonsFrame->layout()->addWidget(new QLabel(tr("Inofficial Addons")));

    for (QCheckBox *b : inofficialAddons) {
        ui->addonsFrame->layout()->addWidget(b);
    }
}

// Save Addon Settings
void OptionsDialog::writeAddonSettings()
{
    for (QCheckBox *b : officialAddons) settings->setAddonEnabled(
            b->text(), b->isChecked());

    for (QCheckBox *b : inofficialAddons) settings->setAddonEnabled(
            b->text(), b->isChecked());
}

void OptionsDialog::on_setMusicPath_clicked()
{
    setPath(musicPath);
}

void OptionsDialog::on_setSoundPath_clicked()
{
    setPath(soundPath);
}

void OptionsDialog::on_setResourcesPath_clicked()
{
    setPath(resourcesPath);
}

void OptionsDialog::on_setMapsPath_clicked()
{
    setPath(mapsPath);
}

void OptionsDialog::on_setCharactersPath_clicked()
{
    setPath(charactersPath);
}

void OptionsDialog::on_setNotesPath_clicked()
{
    setPath(notesPath);
}

void OptionsDialog::on_setAudioPath_clicked()
{
    setPath(audioPath);
}

void OptionsDialog::on_setRadioPath_clicked()
{
    setPath(radioPath);
}

void OptionsDialog::on_setShopPath_clicked()
{
    setPath(shopPath);
}

void OptionsDialog::setPath(Setting setting)
{
    settings->setSetting(setting);
    pathsChanged = true;
    updatePaths();
}

void OptionsDialog::updatePaths()
{
    ui->musicPath->setText(settings->getSetting(musicPath));
    ui->soundPath->setText(settings->getSetting(soundPath));
    ui->mapsPath->setText(settings->getSetting(mapsPath));
    ui->resourcesPath->setText(settings->getSetting(resourcesPath));
    ui->charactersPath->setText(settings->getSetting(charactersPath));
    ui->notesPath->setText(settings->getSetting(notesPath));
    ui->audioPath->setText(settings->getSetting(audioPath));
    ui->radioPath->setText(settings->getSetting(radioPath));
    ui->shopPath->setText(settings->getSetting(shopPath));
}

void OptionsDialog::on_selectAll_clicked()
{
    for (QCheckBox *b : officialAddons) {
        b->setChecked(true);
    }

    for (QCheckBox *b : inofficialAddons) {
        b->setChecked(true);
    }
}

void OptionsDialog::on_deselectAll_clicked()
{
    for (QCheckBox *b : officialAddons) {
        b->setChecked(false);
    }

    for (QCheckBox *b : inofficialAddons) {
        b->setChecked(false);
    }
}

// Set new Style Sheet when another one is selected
void OptionsDialog::on_styleComboBox_currentTextChanged(const QString& arg1)
{
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
    AddonManager *manager = new AddonManager;

    manager->show();
}

// Help-Buttons
void OptionsDialog::on_pushButton_openWiki_clicked()
{
    QDesktopServices::openUrl(QUrl(
                                  "https://github.com/PhilInTheGaps/GM-Companion/wiki"));
}

void OptionsDialog::on_pushButton_reportABug_clicked()
{
    QDesktopServices::openUrl(QUrl(
                                  "https://github.com/PhilInTheGaps/GM-Companion/issues/new"));
}

void OptionsDialog::on_pushButton_downloadOlderVersions_clicked()
{
    QDesktopServices::openUrl(QUrl(
                                  "https://github.com/PhilInTheGaps/GM-Companion/releases"));
}

void OptionsDialog::on_pushButton_viewOnGitHub_clicked()
{
    QDesktopServices::openUrl(QUrl(
                                  "https://github.com/PhilInTheGaps/GM-Companion"));
}

void OptionsDialog::on_pushButton_about_clicked()
{
    AboutDialog *about = new AboutDialog(w->getVersion());

    about->show();
}

void OptionsDialog::on_pushButton_checkForUpdates_clicked()
{
    UpdateManager *updateManger = new UpdateManager(w->getVersionNumber());

    updateManger->checkForUpdates();
}

void OptionsDialog::on_pushButton_saveAddons_clicked()
{
    writeAddonSettings();
}

void OptionsDialog::on_checkBox_showToolNames_toggled(bool checked)
{
    settings->setSetting(Setting::showToolNames, checked);
}

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

    // Check or uncheck "enableMusicTrackList"
    int enableTrackList = checkSettings.value("enableMusicTrackList", 0).toInt();
    if (enableTrackList == 1){
        ui->musicTrackListCheckBox->setChecked(true);
    } else{
        ui->musicTrackListCheckBox->setChecked(false);
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

    // Display correct button style
    if (checkSettings.value("buttonStyle", "small").toString() == "small"){
        ui->buttonStyleComboBox->setCurrentIndex(0);
    }else{
        ui->buttonStyleComboBox->setCurrentIndex(1);
    }

    // Show Music Folders and Groups in ComboBox
    for (QString folder : getFolders(checkSettings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString())){
        if (!folder.contains("."))
            ui->groupsMusicFolderComboBox->addItem(folder);
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

// Set new Style Sheet when another one is selected
void OptionsDialog::on_styleComboBox_currentTextChanged(const QString &arg1){
    settings->setSetting(Setting::uiMode, true, arg1);
}

// Set Button Style
void OptionsDialog::on_buttonStyleComboBox_currentIndexChanged(int index){
    switch (index) {
    case 0:
        settings->setSetting(Setting::buttonStyle, true, "small");
        break;
    case 1:
        settings->setSetting(Setting::buttonStyle, true, "large");
        break;
    default:
        break;
    }
}

void OptionsDialog::on_musicTrackListCheckBox_toggled(bool checked){
    if (checked)
        settings->setSetting(Setting::enableMusicTrackList, true);
    else{
        settings->setSetting(Setting::enableMusicTrackList, false);
    }
}

// When folder is changed, get groups
void OptionsDialog::on_groupsMusicFolderComboBox_currentTextChanged(const QString &arg1)
{
    ui->groupsComboBox->clear();

    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    QString iniPath =  checkSettings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString()+"/"+arg1+"/groups.ini";

    if (QFile(iniPath).exists()){
        QSettings groupSettings(iniPath, QSettings::IniFormat);

        int groupsCount = groupSettings.beginReadArray("Groups");

        for (int i = 0; i<groupsCount; i++){
            groupSettings.setArrayIndex(i);

            ui->groupsComboBox->addItem(groupSettings.value("group", tr("Error: Group Corrupted")).toString());
        }

        groupSettings.endArray();
    }
}

// When Group is changed, display folders
void OptionsDialog::on_groupsComboBox_currentTextChanged(const QString &arg1)
{
    ui->groupsFoldersEdit->clear();

    if (arg1.length() > 0){
        QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
        QString iniPath =  checkSettings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString()+
                                               "/"+ui->groupsMusicFolderComboBox->currentText()+"/groups.ini";

        if (QFile(iniPath).exists()){
            QSettings groupSettings(iniPath, QSettings::IniFormat);

            int folderCount = groupSettings.beginReadArray(arg1);

            if (folderCount<1)
                folderCount = 0;

            for (int i = 0; i<folderCount; i++){
                groupSettings.setArrayIndex(i);

                QString folder = groupSettings.value("folder", tr("Error: Bad Folder Name")).toString();

                if (i == folderCount-1){
                    ui->groupsFoldersEdit->append(folder);
                }else{
                    ui->groupsFoldersEdit->append(folder+",");
                }
            }

            groupSettings.endArray();
        }
    }
}

// Add Music Group
void OptionsDialog::on_addGroupButton_clicked()
{
    QString group = ui->groupsNameLine->text();
    ui->groupsComboBox->addItem(group);

    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    QString iniPath =  checkSettings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString()+
                                           "/"+ui->groupsMusicFolderComboBox->currentText()+"/groups.ini";

    QSettings groupSettings(iniPath, QSettings::IniFormat);

    int groupCount = groupSettings.beginReadArray("Groups");
    groupSettings.endArray();

    groupSettings.beginWriteArray("Groups");
    groupSettings.setArrayIndex(groupCount);

    groupSettings.setValue("group", group);

    groupSettings.endArray();
}

// Save folders in group
void OptionsDialog::on_groupsSaveFolders_clicked()
{
    QStringList folders = ui->groupsFoldersEdit->toPlainText().split(",");

    QString group = ui->groupsComboBox->currentText();

    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    QString iniPath =  checkSettings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString()+
                                           "/"+ui->groupsMusicFolderComboBox->currentText()+"/groups.ini";

    QSettings groupSettings(iniPath, QSettings::IniFormat);

    groupSettings.beginWriteArray(group);

    for (int i = 0; i<folders.size(); i++){
        groupSettings.setArrayIndex(i);

        QString folder = folders.at(i);
        folder = folder.simplified();

        groupSettings.setValue("folder", folders.at(i));
    }

    groupSettings.endArray();
}

// Remove group
void OptionsDialog::on_removeGroupButton_clicked()
{
    QString group = ui->groupsComboBox->currentText();

    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    QString iniPath =  checkSettings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString()+
                                           "/"+ui->groupsMusicFolderComboBox->currentText()+"/groups.ini";

    QSettings groupSettings(iniPath, QSettings::IniFormat);

    int groupCount = groupSettings.beginReadArray("Groups");

    QStringList groups;

    for (int i = 0; i<groupCount; i++){
        groupSettings.setArrayIndex(i);
        QString temp = groupSettings.value("group", tr("Error")).toString();

        if (temp != group)
            groups.push_back(temp);
    }

    groupSettings.endArray();

    groupSettings.beginWriteArray("Groups");

    for (int i = 0; i<groups.size(); i++){
        QString temp = groups.at(i);

        groupSettings.setArrayIndex(i);

        groupSettings.setValue("group", temp);
    }

    groupSettings.endArray();

    ui->groupsComboBox->removeItem(ui->groupsComboBox->currentIndex());
}

// Select folders with directory chooser and add them to the group
void OptionsDialog::on_groupsSelectFoldersButton_clicked()
{
    QSettings checkSettings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    QString directory =  checkSettings.value("musicPath", QDir::homePath()+"/.gm-companion/music").toString()+
                                           "/"+ui->groupsMusicFolderComboBox->currentText();

    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle(tr("Folder(s) to add to the group"));
    fileDialog->setDirectory(directory);
    fileDialog->setOption(QFileDialog::DontUseNativeDialog, true);
    QListView *l = fileDialog->findChild<QListView*>("listView");
        if (l) {
             l->setSelectionMode(QAbstractItemView::MultiSelection);
         }
        QTreeView *t = fileDialog->findChild<QTreeView*>();
         if (t) {
           t->setSelectionMode(QAbstractItemView::MultiSelection);
       }

    if (fileDialog->exec() == QDialog::Accepted){
        QStringList paths = fileDialog->selectedFiles();

        QString newText = ui->groupsFoldersEdit->toPlainText();

        for (int i = 0; i<paths.size(); i++){
            QString folder = paths.at(i);
            folder = folder.mid(folder.lastIndexOf("/")+1);
            folder = folder.simplified();

            if (!folder.contains(".") && folder.length()>0){
                if (i == paths.size()-1){
                    newText.append(folder);
                }else{
                    newText.append(folder+",");
                }
            }
        }

        ui->groupsFoldersEdit->setText(newText);
    }
}

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

void OptionsDialog::on_addonManagerButton_clicked()
{
    #ifdef __linux__
        QProcess::startDetached("java", {"-jar", "/usr/share/gm-companion/AddonManager.jar"});
    #elif _WIN32
        QProcess::startDetached("java", {"-jar", QApplication::applicationDirPath()+"/AddonManager.jar"});
    #else
    qDebug() << QCoreApplication::translate("","This OS is not supported. Cannot launch Addon Manager.");
    #endif
}

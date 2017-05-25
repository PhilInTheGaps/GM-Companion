#include "characters.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flowlayout.h"
#include "characterpage.h"
#include "settingsmanager.h"
#include "chareditor.h"
#include "deletecharacterdialog.h"

#include <QDir>
#include <QListWidget>
#include <QScrollArea>
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>

// When New Button is clicked
void MainWindow::on_createCharacterButton_clicked()
{
    CharEditor* charEditor = new CharEditor;
    charEditor->showMaximized();
}

// Update list of characters
void MainWindow::updateCharacters(){
    for(int i = ui->charactersStackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = ui->charactersStackedWidget->widget(i);
        ui->charactersStackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    ui->charactersListWidget->clear();

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
}

// When Update Button is clicked
void MainWindow::on_updateCharactersButton_clicked()
{
    updateCharacters();
}

// Edit currently selected character
void MainWindow::on_editCharacter_clicked()
{
    int index = ui->charactersListWidget->currentRow();
    if (index >= 0){
        CharEditor* charEditor = new CharEditor;
        charEditor->load(index);

        charEditor->showMaximized();
    }
}

// Delete currently selected character
void MainWindow::on_deleteCharacterButton_clicked()
{
    QStringList characters = getCharacterList();
    QString characterFile = characters.at(ui->charactersListWidget->currentRow());

    DeleteCharacterDialog* dialog = new DeleteCharacterDialog;
    dialog->setCharacterFile(settingsManager->getSetting(Setting::charactersPath)+"/"+characterFile);

    dialog->show();
}

// Change selected character
void MainWindow::on_characterListClicked(int index){
    if (index >= 0){
        ui->charactersStackedWidget->setCurrentIndex(index);

        QStringList list = ui->charactersStackedWidget->currentWidget()->accessibleName().split(",");

        if (QFile(settingsManager->getSetting(Setting::charactersPath)+"/"+list.at(0)+".png").exists()){
            QPixmap charIcon(settingsManager->getSetting(Setting::charactersPath)+"/"+list.at(0)+".png");
            ui->characterIconLabel->setPixmap(charIcon.scaled(180, 180));
            ui->characterIconLabel->adjustSize();
        }
        else{
            ui->characterIconLabel->clear();
        }
    }
}

// Returns list of all character files in character path
QStringList getCharacterList(){
    qDebug() << "Getting Characters...";
    SettingsManager* settingsManager = new SettingsManager;
    QString folderPath = settingsManager->getSetting(Setting::charactersPath);
    QStringList files = getFiles(folderPath);
    QStringList characterFileNames;

    for (QString file : files){
        if (file.contains(".ini")){
            characterFileNames.push_back(file);
        }
    }

    qDebug() << "Found "+QString::number(characterFileNames.length())+" characters.";
    return characterFileNames;
}

// Writes everyhing from an array into a ListView
QList<QStringList>* writeList(QString character, QString indicator, int columns){
    SettingsManager* settingsManager = new SettingsManager;
    QSettings settings(settingsManager->getSetting(Setting::charactersPath)+"/"+character, QSettings::IniFormat);
    QList<QStringList>* list = new QList<QStringList>;
    int size = settings.beginReadArray(indicator);
    for (int row = 0; row<size; row++){
        settings.setArrayIndex(row);
        QStringList subList;
        for (int column = 0; column<columns; column++){

            subList.push_back(settings.value("entry"+QString::number(column+1)).toString());
        }
        list->push_back(subList);
    }
    settings.endArray();
    return list;
}

// Returns the completed character page with all the information
QWidget* getCharacterPage(QString character){
    CharacterPage* charPage = new CharacterPage;

    SettingsManager* settingsManager = new SettingsManager;
    QSettings charSettings(settingsManager->getSetting(Setting::charactersPath)+"/"+character, QSettings::IniFormat);

    QString version = charSettings.value("Version", "SHOULD NOT BE VISIBLE").toString();
    QString name = charSettings.value("Name", character).toString();
    QString player = charSettings.value("Player", "").toString();
    int systemID = charSettings.value("System", 0).toInt();
    charPage->systemID = systemID;

    QString iconPath = charSettings.value("Icon", " ").toString();

    charPage->name = name;
    charPage->player = player;
    charPage->setToolTip(name+" ("+player+")");
    charPage->setAccessibleName(name+","+QString::number(systemID));

    switch (systemID) {
    case 0:{
        // General Character Info
        charPage->generalInfo_generic = writeList(character, "generalInfos", 1);

        // Skills
        charPage->skills1_generic = writeList(character, "skills1", 2);
        charPage->skills2_generic = writeList(character, "skills2", 2);
        charPage->skills3_generic = writeList(character, "skills3", 2);

        // Weapons
        charPage->weapons_generic = writeList(character, "weapons", 3);

        // Armor
        charPage->armor_generic = writeList(character, "armor", 3);

        // Inventory
        charPage->inv1_generic = writeList(character, "inventory1", 2);
        charPage->inv2_generic = writeList(character, "inventory2", 2);

        break;
    }
    case 1:{
        // Persönliche Daten
        charPage->persDaten1_dsa5 = writeList(character, "persDaten1", 1);
        charPage->persDaten2_dsa5 = writeList(character, "persDaten2", 1);

        // Eigenschaften
        charPage->eigenschaften_dsa5 = writeList(character, "eigenschaften", 8);

        // Vorteile
        charPage->vorteile_dsa5 = writeList(character, "vorteile", 1);

        // Nachteile
        charPage->nachteile_dsa5 = writeList(character, "nachteile", 1);

        // Sonderfertigkeiten
        charPage->sonderf_dsa5 = writeList(character, "sonderfertigkeiten", 1);

        // Allgemeine Werte
        charPage->allgemein_dsa5 = writeList(character, "allgemein", 4);

        // AP
        charPage->ap_dsa5 = writeList(character, "ap", 4);

        // Schicksalspunkte
        charPage->schicksalsp_dsa5 = writeList(character, "schicksalsp", 4);

        // Fertigkeiten
        charPage->fertigkeiten1_dsa5 = writeList(character, "fertigkeiten1", 6);
        charPage->fertigkeiten2_dsa5 = writeList(character, "fertigkeiten2", 6);

        // Sprachen
        charPage->sprachen_dsa5 = writeList(character, "sprachen", 1);

        // Schriften
        charPage->schriften_dsa5 = writeList(character, "schriften", 1);

        // Kampftechniken
        charPage->ktw_dsa5 = writeList(character, "kampftechniken", 5);

        // LEP
        charPage->lep_dsa5 = writeList(character, "lep", 2);

        // Nahkampfwaffen
        charPage->nahkampfwaffen_dsa5 = writeList(character, "nahkampfwaffen", 9);

        // Fernkampfwaffen
        charPage->fernkampfwaffen_dsa5 = writeList(character, "fernkampfwaffen", 8);

        // Rüstungen
        charPage->ruestungen_dsa5 = writeList(character, "ruestungen", 6);

        // Schild / Parierwaffe
        charPage->schild_dsa5 = writeList(character, "schild", 4);

        // Kampfsonderfertigkeiten
        charPage->kSonderf_dsa5 = writeList(character, "kampfsonderfertigkeiten", 1);

        // Ausrüstung
        charPage->ausruestung1_dsa5 = writeList(character, "ausruestung1", 3);
        charPage->ausruestung2_dsa5 = writeList(character, "ausruestung2", 3);

        // Geldbeutel
        charPage->geld_dsa5 = writeList(character, "geldbeutel", 1);

        // Tier
        charPage->tierAllgemein_dsa5 = writeList(character, "tierAllgemein", 1);

        // Tier Angriff
        charPage->tierAngriff_dsa5 = writeList(character, "tierAngriff", 5);

        // Tier Aktionen
        charPage->tierAktionen_dsa5 = writeList(character, "tierAktionen", 1);

        // Tier Sonderf
        charPage->tierSonderf_dsa5 = writeList(character, "tierSonderfertigkeiten", 1);

        // Zauber
        charPage->zauber_dsa5 = writeList(character, "zauber", 11);

        // Zaubertricks
        charPage->zaubertricks_dsa5 = writeList(character, "zaubertricks", 1);

        // Magische Sonderfertigkeiten
        charPage->magischeSonderfertigkeiten_dsa5 = writeList(character, "magischeSonderfertigkeiten", 1);

        // Leiteigenschaft / Merkmal
        charPage->leiteigenschaftMerkmal_dsa5 = writeList(character, "leiteigenschaftMerkmal", 2);

        // Magische Tradition
        charPage->mTradition_dsa5 = writeList(character, "mTradition", 1);

        // Liturgien
        charPage->liturgien_dsa5 = writeList(character, "liturgien", 11);

        // Segnungen
        charPage->segnungen_dsa5 = writeList(character, "segnungen", 1);

        // Klerikale Sonderfertigkeiten
        charPage->klerikaleSonderfertigkeiten_dsa5 = writeList(character, "klerikaleSonderfertigkeiten", 1);

        // Leiteigenschaft / Aspekt
        charPage->leiteigenschaftAspekt_dsa5 = writeList(character, "leiteigenschaftAspekt", 2);

        // Klerikale Tradition
        charPage->kTradition_dsa5 = writeList(character, "kTradition", 1);

        break;
    }

    case 2:
    {
        // Name
        charPage->name_sifrp = charSettings.value("charName", " ").toString();

        // Age
        charPage->age_sifrp = charSettings.value("Age", " ").toString();

        // Gender
        charPage->gender_sifrp = charSettings.value("Gender", " ").toString();

        // House
        charPage->house_sifrp = charSettings.value("House", " ").toString();

        // Abilities
        charPage->abilities1_sifrp = writeList(character, "abilities1", 2);
        charPage->abilities2_sifrp = writeList(character, "abilities2", 2);

        // Qualities
        charPage->qualities_sifrp = writeList(character, "qualities", 1);
        charPage->intrigueCombat_sifrp = writeList(character, "intrigueCombat", 2);

        // Destiny Points
        charPage->dest1_sifrp = charSettings.value("dest1", false).toBool();
        charPage->dest2_sifrp = charSettings.value("dest2", false).toBool();
        charPage->dest3_sifrp = charSettings.value("dest3", false).toBool();
        charPage->dest4_sifrp = charSettings.value("dest4", false).toBool();
        charPage->dest5_sifrp = charSettings.value("dest5", false).toBool();
        charPage->dest6_sifrp = charSettings.value("dest6", false).toBool();
        charPage->dest7_sifrp = charSettings.value("dest7", false).toBool();
        charPage->dest8_sifrp = charSettings.value("dest8", false).toBool();

        // Weapons
        charPage->weapons_sifrp = writeList(character, "weapons", 3);
        charPage->armor_sifrp = writeList(character, "armor", 1);

        // Wounds
        charPage->wounds_sifrp = writeList(character, "wounds", 1);

        // Equipment
        charPage->equipment_sifrp = writeList(character, "equipment", 1);

        // Appearance
        charPage->appearance_sifrp = writeList(character, "appearance", 4);

        // Mannerisms
        charPage->mannerisms_sifrp = charSettings.value("mannerisms", " ").toString();

        // Distinguishing Features
        charPage->distFeatures_sifrp = charSettings.value("distFeatures", " ").toString();

        // Retainers
        charPage->retainers_sifrp = charSettings.value("retainers", " ").toString();

        // Personal History
        charPage->personalHistory_sifrp = charSettings.value("personalHistory", " ").toString();

        // Allies
        charPage->allies_sifrp = charSettings.value("allies", " ").toString();

        // Heraldry
        charPage->heraldry_sifrp = charSettings.value("heraldry", " ").toString();

        // Enemies
        charPage->enemies_sifrp = charSettings.value("enemies", " ").toString();

        // Portrait
        charPage->portrait_sifrp = charSettings.value("portrait", " ").toString();

        // Oaths
        charPage->oaths_sifrp = charSettings.value("oaths", " ").toString();

        // Motto
        charPage->motto_sifrp = charSettings.value("motto", " ").toString();

        break;
    }
    default:
        break;
    }

    charPage->updateUi();
    return charPage;
}

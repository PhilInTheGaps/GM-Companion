#include "chareditor.h"
#include "ui_chareditor.h"
#include "tools/characters.h"
#include <QSettings>
#include <QFileDialog>
#include <QDebug>

CharEditor::CharEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CharEditor)
{
    ui->setupUi(this);
    ui->generalInfoTable->verticalHeader()->setVisible(true);
    settingsManager = new SettingsManager;

}

CharEditor::~CharEditor()
{
    delete ui;
}

void CharEditor::writeTable(QString indicator, int columns, QTableWidget* table, bool updateTableSize){
    QSettings settings(settingsManager->getSetting(Setting::charactersPath)+"/"+loadCharacterFile, QSettings::IniFormat);

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

    if (updateTableSize){
        table->setRowCount(size);
        table->setColumnCount(columns);
    }

    for (int row = 0; row<size; row++){
        for (int column = 0; column<columns; column++){
            QString entry = list->at(row).at(column);

            QTableWidgetItem* item = new QTableWidgetItem;
            item->setText(entry);
            table->setItem(row, column, item);
        }
    }
}

void CharEditor::load(int index){
    QStringList characters = getCharacterList();
    loadCharacterFile = characters.at(index);

    QSettings settings(settingsManager->getSetting(Setting::charactersPath)+"/"+loadCharacterFile, QSettings::IniFormat);

    qDebug() << tr("Loading Character") << settings.value("Name").toString() << "...";
    ui->displayNameLineEdit->setText(settings.value("Name").toString());
    ui->playerLineEdit->setText(settings.value("Player").toString());

    int systemID = settings.value("System", 0).toInt();
    ui->systemComboBox->setCurrentIndex(systemID);
    ui->stackedWidget->setCurrentIndex(systemID);
    ui->iconLineEdit->setText(settings.value("Icon", " ").toString());

    switch (systemID) {
    case 0:{
        // General Character Information
        writeTable("generalInfos", 1, ui->generalInfoTable);

        // Skills
        writeTable("skills1", 2, ui->skillsTable1_generic, true);
        writeTable("skills2", 2, ui->skillsTable2_generic, true);
        writeTable("skills3", 2, ui->skillsTable3_generic, true);

        // Weapons
        writeTable("weapons", 3, ui->weaponsTable_generic, true);

        // Armor
        writeTable("armor", 3, ui->armorTable_generic, true);

        // Inventory
        writeTable("inventory1", 2, ui->inv1Table_generic, true);
        writeTable("inventory2", 2, ui->inv2Table_generic, true);

        break;
    }
    case 1:{
        // Persönliche Daten
        writeTable("persDaten1", 1, ui->persInf1Table_dsa5);
        writeTable("persDaten2", 1, ui->persInf2Table_dsa5);

        // Vorteile
        writeTable("vorteile", 1, ui->vorteileTable_dsa5, true);

        // Nachteile
        writeTable("nachteile", 1, ui->nachteileTable_dsa5, true);

        // Sonderfertigkeiten
        writeTable("sonderfertigkeiten", 1,ui->sonderfTable_dsa5, true);

        // Eigenschaften
        writeTable("eigenschaften", 8,ui->eigenschaftenTable_dsa5);

        // AP
        writeTable("ap", 4, ui->apTable_dsa5);

        // Allgemeine Werte
        writeTable("allgemein", 4, ui->allgWerteTable_dsa5);

        // Schicksalspunkte
        writeTable("schicksalsp", 4, ui->schicksalspunkteTable_dsa5);

        // Fertigkeiten
        writeTable("fertigkeiten1", 6, ui->fertigkeiten1Table_dsa5);
        writeTable("fertigkeiten2", 6, ui->fertigkeiten2Table_dsa5);

        // Sprachen
        writeTable("sprachen", 1, ui->sprachenTable_dsa5, true);

        // Schriften
        writeTable("schriften", 1, ui->schriftenTable_dsa5, true);

        // Kampftechniken
        writeTable("kampftechniken", 5, ui->ktwTable_dsa5);

        // Lebenspunkte
        writeTable("lep", 2, ui->lepTable_dsa5);

        // Nahkampfwaffen
        writeTable("nahkampfwaffen", 9, ui->nahkampfTable_dsa5, true);

        // Fernkampfwaffen
        writeTable("fernkampfwaffen", 8,ui->fernkampfTable_dsa5, true);

        // Rüstungen
        writeTable("ruestungen", 6, ui->ruestungenTable_dsa5, true);

        // Schild / Parierwaffe
        writeTable("schild", 4, ui->schildTable_dsa5, true);

        // Kampfsonderfertigkeiten
        writeTable("kampfsonderfertigkeiten", 1, ui->kSonderfTable_dsa5, true);

        // Ausrüstung
        writeTable("ausruestung1", 3, ui->ausruestung1Table_dsa5, true);
        writeTable("ausruestung2", 3, ui->ausruestung2Table_dsa5, true);

        // Geldbeutel
        writeTable("geldbeutel", 1, ui->geldTable_dsa5);

        // Tier Allgemein
        writeTable("tierAllgmein", 1, ui->tierAllgemeinTable_dsa5);

        // Tier Angriff
        writeTable("tierAngriff", 5, ui->tierAngriffTable_dsa5, true);

        // Tier Sonderfertigkeiten
        writeTable("tierSonderfertigkeiten", 1, ui->tierSonderfTable_dsa5, true);

        // Tier Aktionen
        writeTable("tierAktionen", 1, ui->tierAktionenTable_dsa5, true);

        // Zauber
        writeTable("zauber", 11, ui->zauberTable_dsa5, true);

        // Zaubertricks
        writeTable("zaubertricks", 1, ui->zaubertricksTable_dsa5, true);

        // Magische Sonderfertigkeiten
        writeTable("magischeSonderfertigkeiten", 1, ui->mSonderfTable_dsa5, true);

        // Leiteigenschaft / Merkmal
        writeTable("leiteigenschaftMerkmal", 2, ui->leiteigMerkmalTable_dsa5);

        // Magische Tradition
        writeTable("mTradition", 1, ui->magiTraditionTable_dsa5);

        // Liturgien
        writeTable("liturgien", 11, ui->liturgienTable_dsa5, true);

        // Segnungen
        writeTable("segnungen", 1, ui->segnungenTable_dsa5, true);

        // Klerikale Sonderfertigkeiten
        writeTable("klerikaleSonderfertigkeiten", 1, ui->kleriSonderfTable_dsa5, true);

        // Leiteigenschaft / Aspekt
        writeTable("leiteigenschaftAspekt", 2, ui->leiteigAspektTable_dsa5);

        // Klerikale Tradition
        writeTable("kTradition", 1, ui->kleriTraditionTable_dsa5);

        break;
    }
    case 2:{
        // Name
        ui->nameLineEdit_sifrp->setText(settings.value("charName").toString());

        // Age
        ui->ageLineEdit_sirfp->setText(settings.value("age").toString());

        // Gender
        ui->genderLineEdit_sifrp->setText(settings.value("gender").toString());

        // House
        ui->houseLineEdit_sifrp->setText(settings.value("house").toString());

        // Abilities
        writeTable("abilities1", 2, ui->abilitiesTable1_sifrp);
        writeTable("abilities2", 2, ui->abilitiesTable2_sifrp);

        // Qualities
        writeTable("qualities", 1, ui->qualitiesTable_sifrp, true);

        // Destiny Points
        ui->destPoints1_sifrp->setChecked(settings.value("dest1", false).toBool());
        ui->destPoints2_sifrp->setChecked(settings.value("dest2", false).toBool());
        ui->destPoints3_sifrp->setChecked(settings.value("dest3", false).toBool());
        ui->destPoints4_sifrp->setChecked(settings.value("dest4", false).toBool());
        ui->destPoints5_sifrp->setChecked(settings.value("dest5", false).toBool());
        ui->destPoints6_sifrp->setChecked(settings.value("dest6", false).toBool());
        ui->destPoints7_sifrp->setChecked(settings.value("dest7", false).toBool());
        ui->destPoints8_sifrp->setChecked(settings.value("dest8", false).toBool());

        // Intrigue / Combat
        writeTable("intrigueCombat", 2, ui->intrigueCombatTable_sifrp);

        // Weapons
        writeTable("weapons", 3, ui->weaponsTable_sifrp);

        // Armor
        writeTable("armor", 1, ui->armorTable_sifrp);

        // Wounds
        writeTable("wounds", 1, ui->woundsTable_sifrp);

        // Equipment
        writeTable("equipment", 1, ui->equipmentTable_sifrp, true);

        // Appearance
        writeTable("appearance", 4, ui->appearanceTable_sifrp);

        // Mannerisms
        ui->mannerismsTextEdit_sifrp->setText(settings.value("mannerisms", " ").toString());

        // Distinguishing Features
        ui->distFeaturesTextEdit_sifrp->setText(settings.value("distFeatures", " ").toString());

        // Retainers
        ui->retainersTextEdit_sifrp->setText(settings.value("retainers", " ").toString());

        // Personal History
        ui->personalHistoryTextEdit_sifrp->setText(settings.value("personalHistory", " ").toString());

        // Allies
        ui->alliesTextEdit_sifrp->setText(settings.value("allies", " ").toString());

        // Heraldry
        ui->heraldryTextEdit_sifrp->setText(settings.value("heraldry", " ").toString());

        // Enemies
        ui->enemiesTextEdit_sifrp->setText(settings.value("enemies", " ").toString());

        // Portrait
        ui->portraitTextEdit_sifrp->setText(settings.value("portrait", " ").toString());

        // Oaths
        ui->oathsTextEdit_sifrp->setText(settings.value("oaths", " ").toString());

        // Motto
        ui->mottoTextEdit_sifrp->setText(settings.value("motto", " ").toString());

        break;
    }
    default:
        break;
    }
    qDebug() << tr("Done.");
}

void CharEditor::writeToFile(QTableWidget* table, QString indicator, int columns){
    QSettings settings(settingsManager->getSetting(Setting::charactersPath)+"/"+displayName+".ini", QSettings::IniFormat);
    QList<QStringList> list;
    for (int i = 0; i<table->rowCount(); i++){
        QStringList subList;
        for (int column = 0; column<columns; column++){
            if (table->item(i, 0) != 0){
                if (!table->item(i, column) || table->item(i, column)->text().isEmpty()){
                    subList.push_back(" ");
                }
                else{
                    subList.push_back(table->item(i, column)->text());
                }
            }
            else{
                subList.push_back(" ");
            }
        }
        list.push_back(subList);
    }

    settings.beginWriteArray(indicator);
    for (int i = 0; i < list.size(); ++i) {
        settings.setArrayIndex(i);
        for (int column = 0; column < columns; column++){
            settings.setValue("entry"+QString::number(column+1), list.at(i).at(column));
        }
    }
    settings.endArray();
}

void CharEditor::save(){

    displayName = ui->displayNameLineEdit->text();

    qDebug() << tr("Saving Character") << displayName << "...";

    QString playerName = ui->playerLineEdit->text();
    int systemID = ui->systemComboBox->currentIndex();

    QSettings settings(settingsManager->getSetting(Setting::charactersPath)+"/"+displayName+".ini", QSettings::IniFormat);

    settings.setValue("Version", "1.0.0");
    settings.setValue("Name", displayName);
    settings.setValue("Player", playerName);
    settings.setValue("System", systemID);
    settings.setValue("Icon", ui->iconLineEdit->text());

    switch (systemID) {
    case 0:{
        // General Character Info
        writeToFile(ui->generalInfoTable, "generalInfos", 1);

        // Skills
        writeToFile(ui->skillsTable1_generic, "skills1", 2);
        writeToFile(ui->skillsTable2_generic, "skills2", 2);
        writeToFile(ui->skillsTable3_generic, "skills3", 2);

        // Weapons
        writeToFile(ui->weaponsTable_generic, "weapons", 3);

        // Armor
        writeToFile(ui->armorTable_generic, "armor", 3);

        // Inventory
        writeToFile(ui->inv1Table_generic, "inventory1", 2);
        writeToFile(ui->inv2Table_generic, "inventory2", 2);

        break;
    }
    case 1:{
        // Persönliche Daten
        writeToFile(ui->persInf1Table_dsa5, "persDaten1", 1);
        writeToFile(ui->persInf2Table_dsa5, "persDaten2", 1);

        // Vorteile
        writeToFile(ui->vorteileTable_dsa5, "vorteile", 1);

        // Nachteile
        writeToFile(ui->nachteileTable_dsa5, "nachteile", 1);

        // Sonderfertigkeiten
        writeToFile(ui->sonderfTable_dsa5, "sonderfertigkeiten", 1);

        // Eigenschaften
        writeToFile(ui->eigenschaftenTable_dsa5, "eigenschaften", 8);

        // AP
        writeToFile(ui->apTable_dsa5, "ap", 4);

        // Allgemeine Werte
        writeToFile(ui->allgWerteTable_dsa5, "allgemein", 4);

        // Schicksalspunkte
        writeToFile(ui->schicksalspunkteTable_dsa5, "schicksalsp", 4);

        // Fertigkeiten
        writeToFile(ui->fertigkeiten1Table_dsa5, "fertigkeiten1", 6);
        writeToFile(ui->fertigkeiten2Table_dsa5, "fertigkeiten2", 6);

        // Sprachen
        writeToFile(ui->sprachenTable_dsa5, "sprachen", 1);

        // Schriften
        writeToFile(ui->schriftenTable_dsa5, "schriften", 1);

        // Kampftechniken
        writeToFile(ui->ktwTable_dsa5, "kampftechniken", 5);

        // Lebenspunkte
        writeToFile(ui->lepTable_dsa5, "lep", 2);

        // Nahkampfwaffen
        writeToFile(ui->nahkampfTable_dsa5, "nahkampfwaffen", 9);

        // Fernkampfwaffen
        writeToFile(ui->fernkampfTable_dsa5, "fernkampfwaffen", 8);

        // Rüstungen
        writeToFile(ui->ruestungenTable_dsa5, "ruestungen", 6);

        // Schild / Parierwaffe
        writeToFile(ui->schildTable_dsa5, "schild", 4);

        // Kampfsonderfertigkeiten
        writeToFile(ui->kSonderfTable_dsa5, "kampfsonderfertigkeiten", 1);

        // Ausrüstung
        writeToFile(ui->ausruestung1Table_dsa5, "ausruestung1", 3);
        writeToFile(ui->ausruestung2Table_dsa5, "ausruestung2", 3);

        // Geldbeutel
        writeToFile(ui->geldTable_dsa5, "geldbeutel", 1);

        // Tier Allgemein
        writeToFile(ui->tierAllgemeinTable_dsa5, "tierAllgmein", 1);

        // Tier Angriff
        writeToFile(ui->tierAngriffTable_dsa5, "tierAngriff", 5);

        // Tier Sonderfertigkeiten
        writeToFile(ui->tierSonderfTable_dsa5, "tierSonderfertigkeiten", 1);

        // Tier Aktionen
        writeToFile(ui->tierAktionenTable_dsa5, "tierAktionen", 1);

        // Zauber
        writeToFile(ui->zauberTable_dsa5, "zauber", 11);

        // Zaubertricks
        writeToFile(ui->zaubertricksTable_dsa5, "zaubertricks", 1);

        // Magische Sonderfertigkeiten
        writeToFile(ui->mSonderfTable_dsa5, "magischeSonderfertigkeiten", 1);

        // Leiteigenschaft / Merkmal
        writeToFile(ui->leiteigMerkmalTable_dsa5, "leiteigenschaftMerkmal", 2);

        // Magische Tradition
        writeToFile(ui->magiTraditionTable_dsa5, "mTradition", 1);

        // Liturgien
        writeToFile(ui->liturgienTable_dsa5, "liturgien", 11);

        // Segnungen
        writeToFile(ui->segnungenTable_dsa5, "segnungen", 1);

        // Klerikale Sonderfertigkeiten
        writeToFile(ui->kleriSonderfTable_dsa5, "klerikaleSonderfertigkeiten", 1);

        // Leiteigenschaft / Aspekt
        writeToFile(ui->leiteigAspektTable_dsa5, "leiteigenschaftAspekt", 2);

        // Klerikale Tradition
        writeToFile(ui->kleriTraditionTable_dsa5, "kTradition", 1);

        break;
    }

    case 2:
    {
        // Name
        settings.setValue("charName", ui->nameLineEdit_sifrp->text());

        // Age
        settings.setValue("age", ui->ageLineEdit_sirfp->text());

        // Gender
        settings.setValue("gender", ui->genderLineEdit_sifrp->text());

        // House
        settings.setValue("house", ui->houseLineEdit_sifrp->text());

        // Abilities
        writeToFile(ui->abilitiesTable1_sifrp, "abilities1", 2);
        writeToFile(ui->abilitiesTable2_sifrp, "abilities2", 2);

        // Qualities
        writeToFile(ui->qualitiesTable_sifrp, "qualities", 1);

        // Intrigue / Combat
        writeToFile(ui->intrigueCombatTable_sifrp, "intrigueCombat", 2);

        // Destiny Points
        settings.setValue("dest1", ui->destPoints1_sifrp->isChecked());
        settings.setValue("dest2", ui->destPoints2_sifrp->isChecked());
        settings.setValue("dest3", ui->destPoints3_sifrp->isChecked());
        settings.setValue("dest4", ui->destPoints4_sifrp->isChecked());
        settings.setValue("dest5", ui->destPoints5_sifrp->isChecked());
        settings.setValue("dest6", ui->destPoints6_sifrp->isChecked());
        settings.setValue("dest7", ui->destPoints7_sifrp->isChecked());
        settings.setValue("dest8", ui->destPoints8_sifrp->isChecked());

        // Weapons
        writeToFile(ui->weaponsTable_sifrp, "weapons", 3);

        // Armor
        writeToFile(ui->armorTable_sifrp, "armor", 1);

        // Wounds
        writeToFile(ui->woundsTable_sifrp, "wounds", 1);

        // Equipment
        writeToFile(ui->equipmentTable_sifrp, "equipment", 1);

        // Appearance
        writeToFile(ui->appearanceTable_sifrp, "appearance", 4);

        // Mannerisms
        settings.setValue("mannerisms", ui->mannerismsTextEdit_sifrp->toPlainText());

        // Distinguishing Features
        settings.setValue("distFeatures", ui->distFeaturesTextEdit_sifrp->toPlainText());

        // Retainers
        settings.setValue("retainers", ui->retainersTextEdit_sifrp->toPlainText());

        // Personal History
        settings.setValue("personalHistory", ui->personalHistoryTextEdit_sifrp->toPlainText());

        // Allies
        settings.setValue("allies", ui->alliesTextEdit_sifrp->toPlainText());

        // Heraldry
        settings.setValue("heraldry", ui->heraldryTextEdit_sifrp->toPlainText());

        // Enemies
        settings.setValue("enemies", ui->enemiesTextEdit_sifrp->toPlainText());

        // Portrait
        settings.setValue("portrait", ui->portraitTextEdit_sifrp->toPlainText());

        // Oaths
        settings.setValue("oaths", ui->oathsTextEdit_sifrp->toPlainText());

        // Motto
        settings.setValue("motto", ui->mottoTextEdit_sifrp->toPlainText());

        break;
    }
    default:
        break;
    }

    qDebug() << tr("Done.");
}

void CharEditor::addRow(QTableWidget* table){
    table->insertRow(table->rowCount());
}

void CharEditor::removeRow(QTableWidget* table){
    if (table->rowCount() > 0){
        table->removeRow(table->rowCount()-1);
    }
}

void CharEditor::on_systemComboBox_currentIndexChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void CharEditor::on_saveButton_clicked()
{
    save();
    this->close();
}

void CharEditor::on_cancelButton_clicked()
{
    this->close();
}

void CharEditor::on_addRowSkillsTable1_generic_clicked()
{
    addRow(ui->skillsTable1_generic);
}

void CharEditor::on_removeRowSkillsTable1_generic_clicked()
{
    removeRow(ui->skillsTable1_generic);
}

void CharEditor::on_addRowSkillsTable2_generic_clicked()
{
    addRow(ui->skillsTable2_generic);
}

void CharEditor::on_removeRowSkillsTable2_generic_clicked()
{
    removeRow(ui->skillsTable2_generic);
}

void CharEditor::on_addRowSkillsTable3_generic_clicked()
{
    addRow(ui->skillsTable3_generic);
}

void CharEditor::on_removeRowSkillsTable3_generic_clicked()
{
    removeRow(ui->skillsTable3_generic);
}

void CharEditor::on_pushButton_9_clicked()
{
    addRow(ui->weaponsTable_generic);
}

void CharEditor::on_pushButton_8_clicked()
{
    removeRow(ui->weaponsTable_generic);
}

void CharEditor::on_pushButton_11_clicked()
{
    addRow(ui->armorTable_generic);
}

void CharEditor::on_pushButton_10_clicked()
{
    removeRow(ui->armorTable_generic);
}

void CharEditor::on_pushButton_13_clicked()
{
    addRow(ui->inv1Table_generic);
}

void CharEditor::on_pushButton_12_clicked()
{
    removeRow(ui->inv1Table_generic);
}

void CharEditor::on_pushButton_15_clicked()
{
    addRow(ui->inv2Table_generic);
}

void CharEditor::on_pushButton_14_clicked()
{
    removeRow(ui->inv2Table_generic);
}

void CharEditor::on_iconButton_clicked()
{
    QString path;
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setNameFilter("Images (*.jpg, *jpeg, *.png)");
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle("Set Character Icon");
    if (fileDialog->exec() == QDialog::Accepted){
        QStringList paths = fileDialog->selectedFiles();
        path = paths.at(0);
        if (ui->displayNameLineEdit->text().length()>1){
            QFile::copy(path, settingsManager->getSetting(Setting::charactersPath)+"/temp.png");
            if (QFile(settingsManager->getSetting(Setting::charactersPath)+"/"+ui->displayNameLineEdit->text()+".png").exists())
                QFile::remove(settingsManager->getSetting(Setting::charactersPath)+"/"+ui->displayNameLineEdit->text()+".png");

            QFile::copy(settingsManager->getSetting(Setting::charactersPath)+"/temp.png", settingsManager->getSetting(Setting::charactersPath)+"/"+ui->displayNameLineEdit->text()+".png");
            QFile::remove(settingsManager->getSetting(Setting::charactersPath)+"/temp.png");

            if (path.length() > 1){
                ui->iconLineEdit->setText(path);
            }
        }
    }
}

void CharEditor::on_addVorteil_dsa5_clicked()
{
    addRow(ui->vorteileTable_dsa5);
}

void CharEditor::on_removeVorteil_dsa5_clicked()
{
    removeRow(ui->vorteileTable_dsa5);
}

void CharEditor::on_addNachteil_dsa5_clicked()
{
    addRow(ui->nachteileTable_dsa5);
}

void CharEditor::on_removeNachteil_dsa5_clicked()
{
    removeRow(ui->nachteileTable_dsa5);
}

void CharEditor::on_addSonderf_dsa5_clicked()
{
    addRow(ui->sonderfTable_dsa5);
}

void CharEditor::on_removeSonderf_dsa5_clicked()
{
    removeRow(ui->sonderfTable_dsa5);
}

void CharEditor::on_addSprache_dsa5_clicked()
{
    addRow(ui->sprachenTable_dsa5);
}

void CharEditor::on_removeSprache_dsa5_clicked()
{
    removeRow(ui->sprachenTable_dsa5);
}

void CharEditor::on_addSchrift_dsa5_clicked()
{
    addRow(ui->schriftenTable_dsa5);
}

void CharEditor::on_removeSchrift_dsa5_clicked()
{
    removeRow(ui->schriftenTable_dsa5);
}

void CharEditor::on_addKSonderf_dsa5_clicked()
{
    addRow(ui->kSonderfTable_dsa5);
}

void CharEditor::on_removeKSonderf_dsa5_clicked()
{
    removeRow(ui->kSonderfTable_dsa5);
}

void CharEditor::on_addNahkampf_dsa5_clicked()
{
    addRow(ui->nahkampfTable_dsa5);
}

void CharEditor::on_removeNahkampf_dsa5_clicked()
{
    removeRow(ui->nahkampfTable_dsa5);
}

void CharEditor::on_addRuestung_dsa5_clicked()
{
    addRow(ui->ruestungenTable_dsa5);
}

void CharEditor::on_removeRuestung_dsa5_clicked()
{
    removeRow(ui->ruestungenTable_dsa5);
}

void CharEditor::on_addFernkampf_dsa5_clicked()
{
    addRow(ui->fernkampfTable_dsa5);
}

void CharEditor::on_removeFernkampf_dsa5_clicked()
{
    removeRow(ui->fernkampfTable_dsa5);
}

void CharEditor::on_addSchild_dsa5_clicked()
{
    addRow(ui->schildTable_dsa5);
}

void CharEditor::on_removeSchild_dsa5_clicked()
{
    removeRow(ui->schildTable_dsa5);
}

void CharEditor::on_addAusruestung1_dsa5_clicked()
{
    addRow(ui->ausruestung1Table_dsa5);
}

void CharEditor::on_removeAusruestung1_dsa55_clicked()
{
    removeRow(ui->ausruestung1Table_dsa5);
}

void CharEditor::on_addAusruestung2_dsa5_clicked()
{
    addRow(ui->ausruestung2Table_dsa5);
}

void CharEditor::on_removeAusruestung2_dsa5_clicked()
{
    removeRow(ui->ausruestung2Table_dsa5);
}

void CharEditor::on_addTierSonderf_dsa5_clicked()
{
    addRow(ui->tierSonderfTable_dsa5);
}

void CharEditor::on_removeTierSonderf_dsa5_clicked()
{
    removeRow(ui->tierSonderfTable_dsa5);
}

void CharEditor::on_addTierAngriff_dsa5_clicked()
{
    addRow(ui->tierAngriffTable_dsa5);
}

void CharEditor::on_removeTierAngriff_dsa5_clicked()
{
    removeRow(ui->tierAngriffTable_dsa5);
}

void CharEditor::on_addTierAktionen_dsa5_clicked()
{
    addRow(ui->tierAktionenTable_dsa5);
}

void CharEditor::on_removeTierAktionen_dsa5_clicked()
{
    removeRow(ui->tierAktionenTable_dsa5);
}

void CharEditor::on_addZauber_dsa5_clicked()
{
    addRow(ui->zauberTable_dsa5);
}

void CharEditor::on_removeZauber_dsa5_clicked()
{
    removeRow(ui->zauberTable_dsa5);
}

void CharEditor::on_addZaubertrick_dsa5_clicked()
{
    addRow(ui->zaubertricksTable_dsa5);
}

void CharEditor::on_removeZaubertrick_dsa5_clicked()
{
    removeRow(ui->zaubertricksTable_dsa5);
}

void CharEditor::on_addMSonderf_dsa5_clicked()
{
    addRow(ui->mSonderfTable_dsa5);
}

void CharEditor::on_removeMSonderf_dsa5_clicked()
{
    removeRow(ui->mSonderfTable_dsa5);
}

void CharEditor::on_addLitugie_dsa5_clicked()
{
    addRow(ui->liturgienTable_dsa5);
}

void CharEditor::on_removeLiturgie_dsa5_clicked()
{
    removeRow(ui->liturgienTable_dsa5);
}

void CharEditor::on_addSegnung_dsa5_clicked()
{
    addRow(ui->segnungenTable_dsa5);
}

void CharEditor::on_removeSegnung_dsa5_clicked()
{
    removeRow(ui->segnungenTable_dsa5);
}

void CharEditor::on_addKleriSonderf_dsa5_clicked()
{
    addRow(ui->kleriSonderfTable_dsa5);
}

void CharEditor::on_removeKleriSonderf_dsa5_clicked()
{
    removeRow(ui->kleriSonderfTable_dsa5);
}

void CharEditor::on_addLineQualities_sifrp_clicked()
{
    addRow(ui->qualitiesTable_sifrp);
}

void CharEditor::on_removeLineQualities_sifrp_clicked()
{
    removeRow(ui->qualitiesTable_sifrp);
}

void CharEditor::on_addRowEquipment_sifrp_clicked()
{
    addRow(ui->equipmentTable_sifrp);
}

void CharEditor::on_removeRowEquipment_sifrp_clicked()
{
    removeRow(ui->equipmentTable_sifrp);
}

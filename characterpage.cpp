#include "characterpage.h"
#include "ui_characterpage.h"

#include "QTableWidgetItem"

CharacterPage::CharacterPage(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::CharacterPage)
{
    ui->setupUi(this);
}

CharacterPage::~CharacterPage()
{
    delete ui;
}

void CharacterPage::writeTable(QList<QStringList>* list, int columns, QTableWidget* table, bool updateTableSize){
    int size = list->size();

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

void CharacterPage::updateUi(){

    switch (systemID) {
    case 0:
    {
        this->setCurrentIndex(0);


        // General Info
        writeTable(generalInfo_generic, 1, ui->generalInfoTable_generic);

        // Skills
        writeTable(skills1_generic, 2, ui->skills1Table_generic, true);
        writeTable(skills2_generic, 2, ui->skills2Table_generic, true);
        writeTable(skills3_generic, 2, ui->skills3Table_generic, true);

        // Weapons
        writeTable(weapons_generic, 3, ui->weaponsTable_generic, true);

        // Armor
        writeTable(armor_generic, 3, ui->armorTable_generic, true);

        // Inventory
        writeTable(inv1_generic, 2, ui->inv1Table_generic, true);
        writeTable(inv2_generic, 2, ui->inv2Table_generic, true);

        break;
    }
    case 1:{

        this->setCurrentIndex(1);

        // Persönliche Daten
        writeTable(persDaten1_dsa5, 1, ui->persDaten1Table_dsa5);
        writeTable(persDaten2_dsa5, 1, ui->persDaten2Table_dsa5);

        // Eigenschaften
        writeTable(eigenschaften_dsa5, 8, ui->eigenschaftenTable_dsa5);

        // Vorteile
        writeTable(vorteile_dsa5, 1, ui->vorteileTable_dsa5, true);

        // Nachteile
        writeTable(nachteile_dsa5, 1, ui->nachteileTable_dsa5, true);

        // Sonderfertigkeiten
        writeTable(sonderf_dsa5, 1, ui->sonderfTable_dsa5, true);

        // AP
        writeTable(ap_dsa5, 4, ui->apTable_dsa5);

        // Schicksalspunke
        writeTable(schicksalsp_dsa5, 4, ui->schicksalspTable_dsa5);

        // Allgemeine Werte
        writeTable(allgemein_dsa5, 4, ui->allgemeinTable_dsa5);

        // Fertigkeiten        
        writeTable(fertigkeiten1_dsa5, 6, ui->fertigkeiten1Table_dsa5);
        writeTable(fertigkeiten2_dsa5, 6, ui->fertigkeiten2Table_dsa5);
        ui->fertigkeiten1Table_dsa5->resizeColumnsToContents();
        ui->fertigkeiten2Table_dsa5->resizeColumnsToContents();

        // Sprachen
        writeTable(sprachen_dsa5, 1, ui->sprachenTable_dsa5, true);

        // Schriften
        writeTable(schriften_dsa5, 1, ui->schriftenTable_dsa5, true);

        // Kampftechniken
        writeTable(ktw_dsa5, 5, ui->ktwTable_dsa5);
        ui->ktwTable_dsa5->resizeColumnsToContents();

        // LEP
        writeTable(lep_dsa5, 2, ui->lepTable_dsa5);

        // Nahkampfwaffen
        writeTable(nahkampfwaffen_dsa5, 9, ui->nahkampfwaffenTable_dsa5, true);
        ui->nahkampfwaffenTable_dsa5->resizeColumnsToContents();

        // Fernkampfwaffen
        writeTable(fernkampfwaffen_dsa5, 8, ui->fernkampfwaffenTable_dsa5, true);
        ui->fernkampfwaffenTable_dsa5->resizeColumnsToContents();

        // Rüstungen
        writeTable(ruestungen_dsa5, 6, ui->ruestungenTable_dsa5, true);
        ui->ruestungenTable_dsa5->resizeColumnsToContents();

        // Schild / Parierwaffe
        writeTable(schild_dsa5, 4, ui->schildTable_dsa5, true);
        ui->schildTable_dsa5->resizeColumnsToContents();

        // Kampfsonderfertigkeiten
        writeTable(kSonderf_dsa5, 1, ui->kSonderfTable_dsa5, true);

        // Ausrüstung
        writeTable(ausruestung1_dsa5, 3, ui->ausruestung1Table_dsa5, true);
        writeTable(ausruestung2_dsa5, 3, ui->ausruestung2Table_dsa5, true);
        ui->ausruestung1Table_dsa5->resizeColumnsToContents();
        ui->ausruestung2Table_dsa5->resizeColumnsToContents();

        // Geldbeutel
        writeTable(geld_dsa5, 1, ui->geldTable_dsa5);

        // Tier Allgemein
        writeTable(tierAllgemein_dsa5, 1, ui->tierAllgemeinTable_dsa5);

        // Tier Angriff
        writeTable(tierAngriff_dsa5, 5, ui->tierAngrifTable_dsa5);
        ui->tierAngrifTable_dsa5->resizeColumnsToContents();

        // Tier Aktionen
        writeTable(tierAktionen_dsa5, 1, ui->tierAktionen_dsa5);

        // Tier Sonderf
        writeTable(tierSonderf_dsa5, 1, ui->tierSonderfTable_dsa5);

        // Zauber
        writeTable(zauber_dsa5, 11, ui->zauberTable_dsa5, true);
        ui->zauberTable_dsa5->resizeColumnsToContents();

        // Zaubertricks
        writeTable(zaubertricks_dsa5, 1, ui->zaubertricksTable_dsa5, true);

        // Magische Sonderfertigkeiten
        writeTable(magischeSonderfertigkeiten_dsa5, 1, ui->magiSonderfTable_dsa5, true);

        // Leiteigenschaft / Merkmal
        writeTable(leiteigenschaftMerkmal_dsa5, 2, ui->leiteigMerkmalTable_dsa5);

        // Magische Tradition
        writeTable(mTradition_dsa5, 1, ui->mTraditionTable_dsa5);

        // Liturgien
        writeTable(liturgien_dsa5, 11, ui->liturgienTable_dsa5, true);
        ui->liturgienTable_dsa5->resizeColumnsToContents();

        // Segnungen
        writeTable(segnungen_dsa5, 1, ui->segnungenTable_dsa5, true);

        // Klerikale Sonderfertigkeiten
        writeTable(klerikaleSonderfertigkeiten_dsa5, 1, ui->kleriSonderfTable_dsa5, true);

        // Leiteigenschaft / Aspekt
        writeTable(leiteigenschaftAspekt_dsa5, 2, ui->leiteigAspektTable_dsa5);

        // Klerikale Tradition
        writeTable(kTradition_dsa5, 1, ui->kTraditionTable_dsa5);

        break;
    }

    case 2:
    {
        this->setCurrentIndex(2);

        // Name
        ui->nameLineEdit_sifrp->setText(name_sifrp);

        // Age
        ui->ageLineEdit_sifrp->setText(age_sifrp);

        // Gender
        ui->genderLineEdit_sifrp->setText(gender_sifrp);

        // House
        ui->houseLineEdit_sifrp->setText(house_sifrp);

        // Abilities
        writeTable(abilities1_sifrp, 2, ui->abilitiesTable1_sifrp);
        writeTable(abilities2_sifrp, 2, ui->abilitiesTable2_sifrp);

        // Qualities
        writeTable(qualities_sifrp, 1, ui->qualitiesTable_sifrp, true);

        // Intrigue / Combat
        writeTable(intrigueCombat_sifrp, 2, ui->intrigueCombatTable_sifrp, true);

        // Destiny Points
        ui->dest1_sifrp->setChecked(dest1_sifrp);
        ui->dest2_sifrp->setChecked(dest2_sifrp);
        ui->dest3_sifrp->setChecked(dest3_sifrp);
        ui->dest4_sifrp->setChecked(dest4_sifrp);
        ui->dest5_sifrp->setChecked(dest5_sifrp);
        ui->dest6_sifrp->setChecked(dest6_sifrp);
        ui->dest7_sifrp->setChecked(dest7_sifrp);
        ui->dest8_sifrp->setChecked(dest8_sifrp);

        // Weapons
        writeTable(weapons_sifrp, 3, ui->weaponsTable_sifrp);

        // Armor
        writeTable(armor_sifrp, 1, ui->armorTable_sifrp, true);

        // Wounds
        writeTable(wounds_sifrp, 1, ui->woundsTable_sifrp);

        // Equipment
        writeTable(equipment_sifrp, 1, ui->equipmentTable_sifrp, true);

        // Appearance
        writeTable(appearance_sifrp, 4, ui->apperaranceTable_sifrp);

        // Mannerisms
        ui->mannerismsTextEdit_sifrp->setText(mannerisms_sifrp);

        // Distinguishing Features
        ui->distFeaturesTextEdit_sifrp->setText(distFeatures_sifrp);

        // Retainers
        ui->retainersTextEdit_sifrp->setText(retainers_sifrp);

        // Personal History
        ui->personalHistoryTextEdit_sifrp->setText(personalHistory_sifrp);

        // Allies
        ui->alliesTextEdit_sifrp->setText(allies_sifrp);

        // Heraldry
        ui->heraldryTextEdit_sifrp->setText(heraldry_sifrp);

        // Enemies
        ui->enemiesTextEdit_sifrp->setText(enemies_sifrp);

        // Portrait
        ui->portraitTextEdit_sifrp->setText(portrait_sifrp);

        // Oaths
        ui->oathsTextEdit_sifrp->setText(oaths_sifrp);

        // Motto
        ui->mottoTextEdit_sifrp->setText(motto_sifrp);

        break;
    }
    default:
        break;
    }
}

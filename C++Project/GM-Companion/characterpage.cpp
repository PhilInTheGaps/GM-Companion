#include "characterpage.h"
#include "ui_characterpage.h"

#include "QTableWidgetItem"
#include <QDebug>

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
            qDebug() << row << column << entry;

            QTableWidgetItem* item = new QTableWidgetItem;
            item->setText(entry);
            table->setItem(row, column, item);
        }
    }
}

void CharacterPage::updateUi(){
    qDebug() << "Updating UI";
    qDebug() << "SystemID" << systemID;

    switch (systemID) {
    case 0:
    {
        this->setCurrentIndex(0);

        qDebug() << "Starting to write tables";

        // General Info
        qDebug() << "Writing General Info";
        writeTable(generalInfo_generic, 1, ui->generalInfoTable_generic);

        // Skills
        qDebug() << "Writing Skills";
        writeTable(skills1_generic, 2, ui->skills1Table_generic, true);
        writeTable(skills2_generic, 2, ui->skills2Table_generic, true);
        writeTable(skills3_generic, 2, ui->skills3Table_generic, true);

        // Weapons
        qDebug() << "Writing Weapons";
        writeTable(weapons_generic, 3, ui->weaponsTable_generic, true);

        // Armor
        qDebug() << "Writing Armor";
        writeTable(armor_generic, 3, ui->armorTable_generic, true);

        // Inventory
        qDebug() << "Writing Inventory";
        writeTable(inv1_generic, 2, ui->inv1Table_generic, true);
        writeTable(inv2_generic, 2, ui->inv2Table_generic, true);

        qDebug() << "Finished Updating UI";
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
    default:
        break;
    }
}

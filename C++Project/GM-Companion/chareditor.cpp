#include "chareditor.h"
#include "ui_chareditor.h"
#include "characters.h"
#include <QSettings>
#include <QFileDialog>

CharEditor::CharEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CharEditor)
{
    ui->setupUi(this);
    ui->generalInfoTable->verticalHeader()->setVisible(true);
}

CharEditor::~CharEditor()
{
    delete ui;
}

void CharEditor::save(){

    QString displayName = ui->displayNameLineEdit->text();
    QString playerName = ui->playerLineEdit->text();
    int systemID = ui->systemComboBox->currentIndex();

    QSettings settings("characters/"+displayName+".ini", QSettings::IniFormat);

    settings.setValue("Version", "1.0.0");
    settings.setValue("Name", displayName);
    settings.setValue("Player", playerName);
    settings.setValue("System", systemID);
    settings.setValue("Icon", ui->iconLineEdit->text());

    switch (systemID) {
    case 0:{
        // General Character Info
        QList<TableContent> generalInfos;
        for (int i = 0; i<ui->generalInfoTable->rowCount(); i++){
            if (ui->generalInfoTable->item(i, 0) != 0){
                TableContent content;
                content.leftEntry = ui->generalInfoTable->verticalHeaderItem(i)->text();
                content.rightEntry = ui->generalInfoTable->item(i, 0)->text();
                generalInfos.push_back(content);
            }
        }

        settings.beginWriteArray("generalInfos");
        for (int i = 0; i < generalInfos.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("leftEntry", generalInfos.at(i).leftEntry);
            settings.setValue("rightEntry", generalInfos.at(i).rightEntry);
        }
        settings.endArray();

        // Skills
        QList<TableContent> skills1;
        for (int i = 0; i<ui->skillsTable1_generic->rowCount(); i++){
            if (ui->skillsTable1_generic->item(i, 0) != 0 && ui->skillsTable1_generic->item(i, 1) != 0){
                TableContent content;
                content.leftEntry = ui->skillsTable1_generic->item(i, 0)->text();
                content.rightEntry = ui->skillsTable1_generic->item(i, 1)->text();
                skills1.push_back(content);
            }
        }

        settings.beginWriteArray("skills1");
        for (int i = 0; i < skills1.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("leftEntry", skills1.at(i).leftEntry);
            settings.setValue("rightEntry", skills1.at(i).rightEntry);
        }
        settings.endArray();

        QList<TableContent> skills2;
        for (int i = 0; i<ui->skillsTable2_generic->rowCount(); i++){
            if (ui->skillsTable2_generic->item(i, 0) != 0 && ui->skillsTable2_generic->item(i, 1) != 0){
                TableContent content;
                content.leftEntry = ui->skillsTable2_generic->item(i, 0)->text();
                content.rightEntry = ui->skillsTable2_generic->item(i, 1)->text();
                skills2.push_back(content);
            }
        }

        settings.beginWriteArray("skills2");
        for (int i = 0; i < skills2.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("leftEntry", skills2.at(i).leftEntry);
            settings.setValue("rightEntry", skills2.at(i).rightEntry);
        }
        settings.endArray();

        QList<TableContent> skills3;
        for (int i = 0; i<ui->skillsTable3_generic->rowCount(); i++){
            if (ui->skillsTable3_generic->item(i, 0) != 0 && ui->skillsTable3_generic->item(i, 1) != 0){
                TableContent content;
                content.leftEntry = ui->skillsTable3_generic->item(i, 0)->text();
                content.rightEntry = ui->skillsTable3_generic->item(i, 1)->text();
                skills3.push_back(content);
            }
        }

        settings.beginWriteArray("skills3");
        for (int i = 0; i < skills3.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("leftEntry", skills3.at(i).leftEntry);
            settings.setValue("rightEntry", skills3.at(i).rightEntry);
        }
        settings.endArray();

        // Weapons
        QList<TableContent3C> weapons;
        for (int i = 0; i<ui->weaponsTable_generic->rowCount(); i++){
            if (ui->weaponsTable_generic->item(i, 0) != 0 && ui->weaponsTable_generic->item(i, 1) != 0 && ui->weaponsTable_generic->item(i, 2) != 0){
                TableContent3C content;
                content.leftEntry = ui->weaponsTable_generic->item(i, 0)->text();
                content.midEntry = ui->weaponsTable_generic->item(i, 1)->text();
                content.rightEntry = ui->weaponsTable_generic->item(i, 2)->text();
                weapons.push_back(content);
            }
        }

        settings.beginWriteArray("weapons");
        for (int i = 0; i < weapons.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("leftEntry", weapons.at(i).leftEntry);
            settings.setValue("midEntry", weapons.at(i).midEntry);
            settings.setValue("rightEntry", weapons.at(i).rightEntry);
        }
        settings.endArray();

        // Armor
        QList<TableContent3C> armor;
        for (int i = 0; i<ui->armorTable_generic->rowCount(); i++){
            if (ui->armorTable_generic->item(i, 0) != 0 && ui->armorTable_generic->item(i, 1) != 0 && ui->armorTable_generic->item(i, 2) != 0){
                TableContent3C content;
                content.leftEntry = ui->armorTable_generic->item(i, 0)->text();
                content.midEntry = ui->armorTable_generic->item(i, 1)->text();
                content.rightEntry = ui->armorTable_generic->item(i, 2)->text();
                armor.push_back(content);
            }
        }

        settings.beginWriteArray("armor");
        for (int i = 0; i < armor.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("leftEntry", armor.at(i).leftEntry);
            settings.setValue("midEntry", armor.at(i).midEntry);
            settings.setValue("rightEntry", armor.at(i).rightEntry);
        }
        settings.endArray();

        // Inventory
        QList<TableContent3C> inventory1;
        for (int i = 0; i<ui->inv1Table_generic->rowCount(); i++){
            if (ui->inv1Table_generic->item(i, 0) != 0){
                TableContent3C content;
                content.midEntry = " ";
                content.rightEntry = " ";

                content.leftEntry = ui->inv1Table_generic->item(i, 0)->text();
                if (ui->inv1Table_generic->item(i, 1) != 0){
                    content.midEntry = ui->inv1Table_generic->item(i, 1)->text();
                }
                if (ui->inv1Table_generic->item(i, 2) != 0){
                    content.rightEntry = ui->inv1Table_generic->item(i, 2)->text();
                }
                inventory1.push_back(content);
            }
        }

        settings.beginWriteArray("inventory1");
        for (int i = 0; i < inventory1.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("leftEntry", inventory1.at(i).leftEntry);
            settings.setValue("midEntry", inventory1.at(i).midEntry);
            settings.setValue("rightEntry", inventory1.at(i).rightEntry);
        }
        settings.endArray();

        QList<TableContent3C> inventory2;
        for (int i = 0; i<ui->inv2Table_generic->rowCount(); i++){
            if (ui->inv2Table_generic->item(i, 0) != 0){
                TableContent3C content;
                content.midEntry = " ";
                content.rightEntry = " ";

                content.leftEntry = ui->inv2Table_generic->item(i, 0)->text();
                if (ui->inv2Table_generic->item(i, 1) != 0){
                    content.midEntry = ui->inv2Table_generic->item(i, 1)->text();
                }
                if (ui->inv2Table_generic->item(i, 2) != 0){
                    content.rightEntry = ui->inv2Table_generic->item(i, 2)->text();
                }
                inventory2.push_back(content);
            }
        }

        settings.beginWriteArray("inventory2");
        for (int i = 0; i < inventory2.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("leftEntry", inventory2.at(i).leftEntry);
            settings.setValue("midEntry", inventory2.at(i).midEntry);
            settings.setValue("rightEntry", inventory2.at(i).rightEntry);
        }
        settings.endArray();

        break;
    }
    case 1:
        break;
    default:
        break;
    }

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
        if (path.length() > 1){
            ui->iconLineEdit->setText(path);
        }
    }
}

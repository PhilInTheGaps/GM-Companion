#include "chareditor.h"
#include "ui_chareditor.h"

CharEditor::CharEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CharEditor)
{
    ui->setupUi(this);
}

CharEditor::~CharEditor()
{
    delete ui;
}

void CharEditor::save(){

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

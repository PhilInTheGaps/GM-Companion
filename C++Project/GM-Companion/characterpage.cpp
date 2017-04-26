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

void CharacterPage::updateUi(){
    qDebug() << systemID;

    switch (systemID) {
    case 0:
    {
        this->setCurrentIndex(0);

        qDebug() << generalInfo_generic->count();

        // General Info
        ui->generalInfoTable_generic->setRowCount(generalInfo_generic->size());
        for (int i = 0; i<generalInfo_generic->size(); i++){
            TableContent c = generalInfo_generic->at(i);

            QTableWidgetItem* header = new QTableWidgetItem;
            header->setText(c.leftEntry);
            ui->generalInfoTable_generic->setVerticalHeaderItem(i, header);

            QTableWidgetItem* right = new QTableWidgetItem;
            right->setText(c.rightEntry);
            ui->generalInfoTable_generic->setItem(i, 0, right);
        }

        // Skills
        ui->skills1Table_generic->setRowCount(skills1_generic->size());
        qDebug() << skills1_generic->size();
        for (int i = 0; i<skills1_generic->size(); i++){
            TableContent c = skills1_generic->at(i);

            QTableWidgetItem* left = new QTableWidgetItem;
            left->setText(c.leftEntry);
            ui->skills1Table_generic->setItem(i, 0, left);

            QTableWidgetItem* right = new QTableWidgetItem;
            right->setText(c.rightEntry);
            ui->skills1Table_generic->setItem(i, 1, right);
        }

        ui->skills2Table_generic->setRowCount(skills2_generic->size());
        for (int i = 0; i<skills2_generic->size(); i++){
            TableContent c = skills2_generic->at(i);

            QTableWidgetItem* left = new QTableWidgetItem;
            left->setText(c.leftEntry);
            ui->skills2Table_generic->setItem(i, 0, left);

            QTableWidgetItem* right = new QTableWidgetItem;
            right->setText(c.rightEntry);
            ui->skills2Table_generic->setItem(i, 1, right);
        }

        ui->skills3Table_generic->setRowCount(skills3_generic->size());
        for (int i = 0; i<skills3_generic->size(); i++){
            TableContent c = skills3_generic->at(i);

            QTableWidgetItem* left = new QTableWidgetItem;
            left->setText(c.leftEntry);
            ui->skills3Table_generic->setItem(i, 0, left);

            QTableWidgetItem* right = new QTableWidgetItem;
            right->setText(c.rightEntry);
            ui->skills3Table_generic->setItem(i, 1, right);
        }

        // Weapons
        ui->weaponsTable_generic->setRowCount(weapons_generic->size());
        for (int i = 0; i<weapons_generic->size(); i++){
            TableContent3C c = weapons_generic->at(i);

            QTableWidgetItem* left = new QTableWidgetItem;
            left->setText(c.leftEntry);
            ui->weaponsTable_generic->setItem(i, 0, left);

            QTableWidgetItem* mid = new QTableWidgetItem;
            mid->setText(c.midEntry);
            ui->weaponsTable_generic->setItem(i, 1, mid);

            QTableWidgetItem* right = new QTableWidgetItem;
            right->setText(c.rightEntry);
            ui->weaponsTable_generic->setItem(i, 2, right);
        }

        // Armor
        ui->armorTable_generic->setRowCount(armor_generic->size());
        for (int i = 0; i<armor_generic->size(); i++){
            TableContent3C c = armor_generic->at(i);

            QTableWidgetItem* left = new QTableWidgetItem;
            left->setText(c.leftEntry);
            ui->armorTable_generic->setItem(i, 0, left);

            QTableWidgetItem* mid = new QTableWidgetItem;
            mid->setText(c.midEntry);
            ui->armorTable_generic->setItem(i, 1, mid);

            QTableWidgetItem* right = new QTableWidgetItem;
            right->setText(c.rightEntry);
            ui->armorTable_generic->setItem(i, 2, right);
        }

        // Inventory
        ui->inv1Table_generic->setRowCount(inv1_generic->size());
        for (int i = 0; i<inv1_generic->size(); i++){
            TableContent3C c = inv1_generic->at(i);

            QTableWidgetItem* left = new QTableWidgetItem;
            left->setText(c.leftEntry);
            ui->inv1Table_generic->setItem(i, 0, left);

            QTableWidgetItem* mid = new QTableWidgetItem;
            mid->setText(c.midEntry);
            ui->inv1Table_generic->setItem(i, 1, mid);

            QTableWidgetItem* right = new QTableWidgetItem;
            right->setText(c.rightEntry);
            ui->inv1Table_generic->setItem(i, 2, right);
        }

        ui->inv2Table_generic->setRowCount(inv2_generic->size());
        for (int i = 0; i<inv2_generic->size(); i++){
            TableContent3C c = inv2_generic->at(i);

            QTableWidgetItem* left = new QTableWidgetItem;
            left->setText(c.leftEntry);
            ui->inv2Table_generic->setItem(i, 0, left);

            QTableWidgetItem* mid = new QTableWidgetItem;
            mid->setText(c.midEntry);
            ui->inv2Table_generic->setItem(i, 1, mid);

            QTableWidgetItem* right = new QTableWidgetItem;
            right->setText(c.rightEntry);
            ui->inv2Table_generic->setItem(i, 2, right);
        }

        break;
    }
    case 1:

        this->setCurrentIndex(1);

        break;
    default:
        break;
    }
}

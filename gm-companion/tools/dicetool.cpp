#include "dicetool.h"
#include "ui_dicetool.h"
#include "gm-companion/ui/flowlayout.h"

#include <QTableWidgetItem>
#include <QDebug>

DiceTool::DiceTool(QWidget *parent) : QWidget(parent), ui(new Ui::DiceTool)
{
    qDebug() << "Loading Dice Tool ...";

    ui->setupUi(this);

    generateDice();

    ui->comboBox_dice->setCurrentIndex(5);
}

DiceTool::~DiceTool()
{
    delete ui;
}

// Rolling dice
void DiceTool::rollDice(int sides){

    int amount = ui->spinBox_amount->value();
    int modifier = ui->spinBox_modifier->value();
    int result = 0;

    ui->tableWidget_results->insertRow(0);

    QList<int> results;

    for (int i = 0; i<amount; i++){
        QTableWidgetItem *i1 = new QTableWidgetItem;   // dice
        QTableWidgetItem *i2 = new QTableWidgetItem;   // roll
        QTableWidgetItem *i3 = new QTableWidgetItem;   // result

        // Generate random integer
        int temp = rand() % sides+1;

        // Set dice table item
        i1->setText(QString::number(i+1)+tr("D")+QString::number(sides));

        // Set the roll table item
        i2->setText(QString::number(temp));

        // Add the temporary result to the overall result
        result += temp;
        i3->setText(QString::number(result));

        // Insert a new table row
        ui->tableWidget_results->insertRow(0);

        // Add items to first row
        ui->tableWidget_results->setItem(0, 0, i1);
        ui->tableWidget_results->setItem(0, 1, i2);
        ui->tableWidget_results->setItem(0, 2, i3);

        // Add temp result to results list
        results.push_back(temp);
    }

    if (ui->spinBox_bonusDice->value() > ui->spinBox_amount->value())
        ui->spinBox_bonusDice->setValue(ui->spinBox_amount->value());

    // Remove bonus dice
    for (int i = 0; i<ui->spinBox_bonusDice->value(); i++)
    {
        QTableWidgetItem *i1 = new QTableWidgetItem;   // dice
        QTableWidgetItem *i2 = new QTableWidgetItem;   // roll
        QTableWidgetItem *i3 = new QTableWidgetItem;   // result

        // Get smallest result
        int min = *std::min_element(results.begin(), results.end());
        result -= min;

        // Set dice table item
        i1->setText("Bonus #"+QString::number(i+1));

        // Set the roll table item
        i2->setText("-" + QString::number(min));

        // Display result
        i3->setText(QString::number(result));

        // Insert a new table row
        ui->tableWidget_results->insertRow(0);

        // Add items to first row
        ui->tableWidget_results->setItem(0, 0, i1);
        ui->tableWidget_results->setItem(0, 1, i2);
        ui->tableWidget_results->setItem(0, 2, i3);

        results.removeOne(min);
    }

    // Add modifier
    result += modifier;

    // Display result
    ui->label_result->setText(QString::number(result));
}

// Generating the die buttons
void DiceTool::generateDice()
{
    for (int sides : sidesList)
    {
        ui->comboBox_dice->addItem(QString::number(sides));
    }

}

// Add a custom die
void DiceTool::on_pushButton_addDie_clicked()
{
    sidesList.push_back(ui->spinBox_customDie->value());
    ui->comboBox_dice->clear();
    generateDice();
}

// Reset dice
void DiceTool::on_pushButton_resetDice_clicked()
{
    sidesList = {2, 3, 4, 6, 12, 20};
    ui->comboBox_dice->clear();
    generateDice();
}

void DiceTool::on_pushButton_roll_clicked()
{
    rollDice(ui->comboBox_dice->currentText().toInt());
}

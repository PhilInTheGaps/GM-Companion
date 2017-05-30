#include "sifrp.h"
#include "ui_sifrp.h"

#include <QDir>
#include <QDebug>
#include <QTableWidgetItem>

SIFRP::SIFRP(QWidget *parent) : QWidget(parent), ui(new Ui::SIFRP){
    ui->setupUi(this);

    on_realmComboBox_currentIndexChanged(0);
    ui->eventComboBox->setEnabled(false);
    ui->eventRandomButton->setEnabled(false);
    ui->eventApplyButton->setDisabled(true);

    resourceModifiers = {
        { 5,  -5,  -5,  20,   5,  -5,  -5},
        {20,  -5,  -5,   5,   0,   0,  -5},
        { 5,  10,  20, -10,  -5,  -5,  -5},
        {10,  -5,  -5,   0,   0,  10,   0},
        {-5,  -5,   5,   0,  10,   0,   5},
        {20,  10,  -5, -10,  -5,   0,   0},
        {-5,  10,  -5,  -5,  -5,   0,  20},
        {-5,  10,   0,  -5,  -5,   0,   5},
        { 5,   0,  -5,  10,  -5,   5,   0},
        { 0,  -5,  10,  -5,   0,  10,   0}
    };

    updateFoundingExample(0);
}

SIFRP::~SIFRP(){
    delete ui;
}

int SIFRP::rollDice(int amount){
    int result = 0;
    for (int i = 0; i<amount; i++){
        int temp = rand() % 6;
        temp += 1;
        result += temp;
    }
    return result;
}

QString SIFRP::getString(QString s){
    QSettings settings(QDir::homePath()+"/.gm-companion/addons/SIFRP/house/generator.ini", QSettings::IniFormat);
    QString setting;

    setting = settings.value(s, " ").toString();

    return setting;
}

// When Realm Combo Box Changes
void SIFRP::on_realmComboBox_currentIndexChanged(int index){
    QString info;
    switch (index) {
    case 0:
        info = getString("KingsLanding");
        break;
    case 1:
        info = getString("Dragonstone");
        break;
    case 2:
        info = getString("TheNorth");
        break;
    case 3:
        info = getString("TheIronIslands");
        break;
    case 4:
        info = getString("TheRiverlands");
        break;
    case 5:
        info = getString("TheMountainsOfTheMoon");
        break;
    case 6:
        info = getString("TheWesterlands");
        break;
    case 7:
        info = getString("TheReach");
        break;
    case 8:
        info = getString("TheStormlands");
        break;
    case 9:
        info = getString("Dorne");
        break;
    default:
        break;
    }

    ui->realmInfo->setText(info);

    QTableWidgetItem* i = new QTableWidgetItem;
    i->setText(ui->realmComboBox->currentText());
    ui->resourcesTable->setItem(0, 0, i);

    QTableWidgetItem* i2 = new QTableWidgetItem;
    i2->setText(info);
    ui->resourcesTable->setItem(0, 1, i2);
}

void SIFRP::on_randomRealm_clicked(){
    int i = rollDice(3);
    int index;

    if (i == 3){
        index = 0;
    }else if (i == 4){
        index = 1;
    }else if (i == 5 || i == 6){
        index = 2;
    }else if (i == 7){
        index = 3;
    }else if (i == 8 || i== 9){
        index = 4;
    }else if (i == 10 || i == 11){
        index = 5;
    }else if (i == 12 || i == 13){
        index = 6;
    }else if (i == 14 || i == 15){
        index = 7;
    }else if (i == 16 || i == 17){
        index = 8;
    }else {
        index = 9;
    }

    ui->realmComboBox->setCurrentIndex(index);

    ui->randomRealm->setDisabled(true);
}

// Next Button
void SIFRP::on_nextButton_clicked(){
    if (ui->stackedWidget->currentIndex() == 3 && ui->step4StackedWidget->currentIndex() != ui->step4StackedWidget->count()-1){
        ui->step4StackedWidget->setCurrentIndex(ui->step4StackedWidget->currentIndex()+1);

    }else{
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
    }

    if (ui->stackedWidget->currentIndex() == 1){
        generateStartingResources();
    }

    if (ui->stackedWidget->currentIndex() == ui->stackedWidget->count()-1){
        ui->nextButton->setDisabled(true);
    }
}

void SIFRP::on_step2SetPlayersButton_clicked(){
    playerCount = ui->playersSpinBox->value();
    ui->playersSpinBox->setDisabled(true);
    ui->step2SetPlayersButton->setDisabled(true);

    currentResourceModifyingPlayer = 1;
    ui->step2RolledValue->setValue(rollDice(1));

    ui->step2AddButton->setEnabled(true);
    ui->stepTwoResourceComboBox->setEnabled(true);
}

void SIFRP::on_step2AddButton_clicked(){
    int index = ui->stepTwoResourceComboBox->currentIndex();

    int oldValue = ui->resourcesTable->item(index+1, 0)->text().toInt();
    int newValue = oldValue+ui->step2RolledValue->value();

    QTableWidgetItem* i = new QTableWidgetItem;
    i->setText(QString::number(newValue));
    ui->resourcesTable->setItem(index+1, 0, i);

    resources[index] = newValue;

    currentResourceModifyingPlayer += 1;
    if (currentResourceModifyingPlayer > playerCount){
        ui->step2AddButton->setDisabled(true);
        ui->stepTwoResourceComboBox->setDisabled(true);
        ui->step2RolledValue->setValue(0);
    }else {
        ui->step2RolledValue->setValue(rollDice(1));
    }

    updateResourceDescriptions();
}

void SIFRP::updateResourceDescriptions(){
    QSettings settings(QDir::homePath()+"/.gm-companion/addons/SIFRP/house/generator.ini", QSettings::IniFormat);

    for (int j = 0; j<resourcesList.size(); j++){
        int value = resources[j];
        int descIndex = 0;

        if (value < 1){
            descIndex = 0;
        } else if (value < 11){
            descIndex = 1;
        } else if (value < 21){
            descIndex = 2;
        } else if (value < 31){
            descIndex = 3;
        } else if (value < 41){
            descIndex = 4;
        } else if (value < 51){
            descIndex = 5;
        } else if (value < 61){
            descIndex = 6;
        } else if (value < 71){
            descIndex = 7;
        }

        settings.beginReadArray(resourcesList.at(j));
        settings.setArrayIndex(descIndex);

        QString description = settings.value("description").toString();

        settings.endArray();

        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(description);
        ui->resourcesTable->setItem(j+1, 1, item);
    }
}

void SIFRP::generateStartingResources(){
    for (int i = 0; i<resourcesList.size(); i++){
        int value = rollDice(7);

        int mod = resourceModifiers.at(ui->realmComboBox->currentIndex()).at(i);

        value += mod;

        resources[i] = value;

        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString::number(value));
        ui->resourcesTable->setItem(i+1, 0, item);
    }

    updateResourceDescriptions();
}

void SIFRP::updateFoundingExample(int index){
    ui->foundingExampleLineEdit->setText(foundingExamples.at(index));
}

void SIFRP::on_foundingComboBox_currentIndexChanged(int index){
    updateFoundingExample(index);
}

void SIFRP::on_foundingRandomButton_clicked(){
    int index = rollDice(1);
    ui->foundingComboBox->setCurrentIndex(index-1);
    updateFoundingExample(index-1);

    ui->eventComboBox->setEnabled(true);
    ui->eventRandomButton->setEnabled(true);
    ui->eventApplyButton->setEnabled(true);

    ui->foundingApplyButton->setDisabled(true);
    ui->foundingComboBox->setDisabled(true);
    ui->foundingRandomButton->setDisabled(true);

    calculateMinEventCount();
}

void SIFRP::on_eventComboBox_currentIndexChanged(int index){
    updateEventDescription(index);
}

void SIFRP::updateEventDescription(int index){
    QSettings settings(QDir::homePath()+"/.gm-companion/addons/SIFRP/house/generator.ini", QSettings::IniFormat);
    settings.beginReadArray("Events");
    settings.setArrayIndex(index);
    ui->eventDescriptionTextEdit->setText(settings.value("description").toString());
    settings.endArray();
}

void SIFRP::on_foundingApplyButton_clicked(){
    ui->eventComboBox->setEnabled(true);
    ui->eventRandomButton->setEnabled(true);
    ui->eventApplyButton->setEnabled(true);

    ui->foundingApplyButton->setDisabled(true);
    ui->foundingComboBox->setDisabled(true);
    ui->foundingRandomButton->setDisabled(true);

    updateEventDescription(ui->eventComboBox->currentIndex());

    calculateMinEventCount();
}

void SIFRP::calculateMinEventCount(){
    int index = ui->foundingComboBox->currentIndex();

    switch (index) {
    case 0:
        remainingMinEventCount = rollDice(1)+3;
        break;
    case 1:
        remainingMinEventCount = rollDice(1)+2;
        break;
    case 2:
        remainingMinEventCount = rollDice(1)+1;
        break;
    case 3:
        remainingMinEventCount = rollDice(1);
        break;
    case 4:
        remainingMinEventCount = rollDice(1)-1;
        break;
    case 5:
        remainingMinEventCount = rollDice(1)-2;
        break;
    default:
        break;
    }

    if (remainingMinEventCount < 1)
        remainingMinEventCount = 1;
    remainingMinEventCount += 1;
    calculateEventCount();
}

void SIFRP::calculateEventCount(){
    remainingMinEventCount -= 1;
    if (remainingMinEventCount < 1)
        remainingMinEventCount = 0;

    ui->eventInfoLineEdit->setText("Minimum required events: "+QString::number(remainingMinEventCount));
}

void SIFRP::on_eventApplyButton_clicked(){
    calculateEventMods();
}

void SIFRP::on_eventRandomButton_clicked(){
    int index = rollDice(3)-3;
    ui->eventComboBox->setCurrentIndex(index);

    calculateEventMods();
}

void SIFRP::calculateEventMods(){
    calculateEventCount();

    QTableWidgetItem* i = new QTableWidgetItem;
    i->setText(ui->eventComboBox->currentText());
    ui->eventTableWidget->insertRow(0);
    ui->eventTableWidget->setItem(0, 0, i);

    QTableWidgetItem* i2 = new QTableWidgetItem;
    ui->eventTableWidget->setItem(0, 1, i2);

    switch (ui->eventComboBox->currentIndex()) {
    case 0: // Doom
        resources[0] -= rollDice(2);
        resources[1] -= rollDice(2);
        resources[2] -= rollDice(2);
        resources[3] -= rollDice(2);
        resources[4] -= rollDice(2);
        resources[5] -= rollDice(2);
        resources[6] -= rollDice(2);
        break;
    case 1: // Defeat
        resources[0] -= rollDice(1);
        resources[1] -= rollDice(1);
        resources[2] -= rollDice(1);
        resources[4] -= rollDice(1);
        resources[5] -= rollDice(1);
        resources[6] -= rollDice(1);
        break;
    case 2: // Catastrophe
        resources[3] -= rollDice(1);
        resources[4] -= rollDice(1);
        resources[5] -= rollDice(1);
        resources[6] -= rollDice(1);
        break;
    case 3: // Madness
        resources[0] += 6-rollDice(2);
        resources[1] += 6-rollDice(2);
        resources[2] += 6-rollDice(2);
        resources[3] += 6-rollDice(2);
        resources[4] += 6-rollDice(2);
        resources[5] += 6-rollDice(2);
        resources[6] += 6-rollDice(2);
        break;
    case 4: // Invasion / Revolt
        resources[3] -= rollDice(2);
        resources[4] -= rollDice(1);
        resources[5] -= rollDice(1);
        resources[6] -= rollDice(1);
        break;
    case 5: // Scandal
        resources[1] -= rollDice(1);
        resources[2] -= rollDice(1);
        resources[5] -= rollDice(1);
        break;
    case 6: // Treachery
        resources[1] -= rollDice(1);
        resources[3] -= rollDice(1);
        resources[5] -= rollDice(1);
        break;
    case 7: // Decline
        resources[1] -= rollDice(1);
        resources[2] -= rollDice(1);
        resources[5] -= rollDice(1);
        resources[6] -= rollDice(1);
        break;
    case 8: // Infrastructure
        resources[rand() % 7] += rollDice(1);
        resources[rand() % 7] += rollDice(1);
        break;
    case 9: // Ascend
        resources[1] += rollDice(1);
        resources[2] += rollDice(1);
        resources[5] += rollDice(1);
        resources[6] += rollDice(1);
        break;
    case 10: // Favor
        resources[1] += rollDice(1);
        resources[2] += rollDice(1);
        resources[3] += rollDice(1);
        resources[5] += rollDice(1);
        break;
    case 11: // Victory
        resources[0] += rollDice(1);
        resources[1] += rollDice(1);
        resources[5] += rollDice(1);
        break;
    case 12: // Villain
        resources[1] += rollDice(1);
        resources[3] -= rollDice(1);
        resources[4] -= rollDice(1);
        resources[5] += rollDice(1);
        break;
    case 13: // Glory
        resources[0] += rollDice(1);
        resources[1] += rollDice(1);
        resources[3] += rollDice(1);
        resources[5] += rollDice(1);
        break;
    case 14: // Conquest
        resources[0] -= rollDice(1);
        resources[1] += rollDice(1);
        resources[2] += rollDice(1);
        resources[3] += rollDice(1);
        resources[4] += rollDice(1);
        resources[6] += rollDice(1);
        break;
    case 15: // Windfall
        resources[0] += rollDice(1);
        resources[1] += rollDice(2);
        resources[2] += rollDice(1);
        resources[3] += rollDice(1);
        resources[4] += rollDice(1);
        resources[5] += rollDice(2);
        resources[6] += rollDice(2);
        break;
    default:
        break;
    }

    for (int i = 0; i<7; i++){
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString::number(resources[i]));
        ui->step3ResourcesTable->setItem(0, i, item);
    }

    ui->step4DefenseLine->setText(QString::number(resources[0]));
}

void SIFRP::on_step4InvestInDefenseButton_clicked(){
    int defense = resources[0];
    int invDefense = ui->step4InvestedDefenseLine->text().toInt();
    int cost = 0;
    QString secondColumn;
    QString thirdColumn;

    switch (ui->step4DefenseHoldingComboBox->currentIndex()) {
    case 0:
        cost = 50;
        thirdColumn = QString::number(114+rollDice(10))+" Months";
        break;
    case 1:
        cost = 40;
        thirdColumn = QString::number(96+rollDice(10))+" Months";
        break;
    case 2:
        cost = 30;
        thirdColumn = QString::number(72+rollDice(10))+" Months";
        break;
    case 3:
        cost = 20;
        thirdColumn = QString::number(60+rollDice(10))+" Months";
        break;
    case 4:
        cost = 10;
        thirdColumn = QString::number(36+rollDice(10))+" Months";
        break;
    default:
        break;
    }

    secondColumn = QString::number(cost);

    if (defense-invDefense >= cost){
        ui->step4InvestedDefenseLine->setText(QString::number(invDefense+cost));
        addHolding(Defense, secondColumn, thirdColumn);
    }
}

void SIFRP::addHolding(Holding holding, QString secondColumn, QString thirdColumn){
    QTableWidget* table;
    QComboBox* box;

    switch (holding) {
    case Defense:
        table = ui->step4InvestedDefenseHoldingsTable;
        box = ui->step4DefenseHoldingComboBox;
        break;
    default:
        table = ui->step4InvestedDefenseHoldingsTable;
        box = ui->step4DefenseHoldingComboBox;
        break;
    }

    QTableWidgetItem* i = new QTableWidgetItem;
    i->setText(box->currentText());
    table->insertRow(0);
    table->setItem(0, 0, i);

    QTableWidgetItem* i2 = new QTableWidgetItem;
    i2->setText(secondColumn);
    table->setItem(0, 1, i2);

    QTableWidgetItem* i3 = new QTableWidgetItem;
    i3->setText(thirdColumn);
    table->setItem(0, 2, i3);

}

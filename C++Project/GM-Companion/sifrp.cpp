#include "sifrp.h"
#include "ui_sifrp.h"

#include <QDir>
#include <QDebug>
#include <QTableWidgetItem>

SIFRP::SIFRP(QWidget *parent) : QWidget(parent), ui(new Ui::SIFRP){
    ui->setupUi(this);

    on_realmComboBox_currentIndexChanged(0);
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

    setting = settings.value(s, "").toString();

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
}

void SIFRP::on_nextButton_clicked(){
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);

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
    QTableWidgetItem* i = new QTableWidgetItem;
    int oldValue = ui->resourcesTable->item(index+1, 0)->text().toInt();
    int newValue = oldValue+ui->step2RolledValue->value();
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
        resources[i] = value;

        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString::number(value));
        ui->resourcesTable->setItem(i+1, 0, item);
    }

    updateResourceDescriptions();
}

#include "sifrp.h"
#include "ui_sifrp.h"

#include <QDir>
#include <QDebug>
#include <QTableWidgetItem>

// Start
SIFRP::SIFRP(QWidget *parent) : QWidget(parent), ui(new Ui::SIFRP){
    qDebug().noquote() << "Loading SIFRP tool ...";

    ui->setupUi(this);

    on_realmComboBox_currentIndexChanged(0);
    ui->eventComboBox->setEnabled(false);
    ui->eventRandomButton->setEnabled(false);
    ui->eventApplyButton->setDisabled(true);
    ui->nextButton->setEnabled(false);

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

    ui->stackedWidget->setCurrentIndex(0);
    ui->step4StackedWidget->setCurrentIndex(0);
}

SIFRP::~SIFRP(){
    delete ui;
}

// Roll D6 dice
int SIFRP::rollDice(int amount){
    qDebug().noquote() << "Rolling" << amount << "D6 ...";

    int result = 0;
    for (int i = 0; i<amount; i++){
        int temp = rand() % 6;
        temp += 1;
        result += temp;
    }

    qDebug().noquote() << "Result:" << result;

    return result;
}

// Returns a string from a .ini file
QString SIFRP::getString(QString s){
    qDebug().noquote() << "Reading" << s << "from generator.ini";

    QSettings settings(QDir::homePath()+"/.gm-companion/addons/SIFRP/house/generator.ini", QSettings::IniFormat);
    QString setting;

    setting = settings.value(s, " ").toString();

    qDebug().noquote() << "Result:" << setting;

    return setting;
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
    }else if (ui->stackedWidget->currentIndex() == 3){
        initializeStepFour();
    }

    ui->nextButton->setEnabled(false);

    if (ui->step4StackedWidget->currentIndex() == 3){
        ui->nextButton->setEnabled(true);
    }
}

/* Step 1 - Select Realm */

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

// Select a random realm
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
    ui->step1SetRealmButton->setDisabled(true);
    ui->nextButton->setEnabled(true);
}

// When a realm is manually set
void SIFRP::on_step1SetRealmButton_clicked(){
    ui->nextButton->setEnabled(true);
    ui->randomRealm->setEnabled(false);
    ui->realmComboBox->setEnabled(false);
    ui->step1SetRealmButton->setEnabled(false);
}

/* Step 2 - Generate Resources */

// Set Player count
void SIFRP::on_step2SetPlayersButton_clicked(){
    playerCount = ui->playersSpinBox->value();
    ui->playersSpinBox->setDisabled(true);
    ui->step2SetPlayersButton->setDisabled(true);

    currentResourceModifyingPlayer = 1;
    ui->step2RolledValue->setValue(rollDice(1));

    ui->step2AddButton->setEnabled(true);
    ui->stepTwoResourceComboBox->setEnabled(true);
}

// Add modifier to a resource
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

        ui->nextButton->setEnabled(true);
    }else {
        ui->step2RolledValue->setValue(rollDice(1));
    }

    updateResourceDescriptions();
}

// Display the description
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

// Generate the starting resources
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

// Display a founding example
void SIFRP::updateFoundingExample(int index){
    ui->foundingExampleLineEdit->setText(foundingExamples.at(index));
}

void SIFRP::on_foundingComboBox_currentIndexChanged(int index){
    updateFoundingExample(index);
}

// Choose a random founding time
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

// Apply founding time
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

/* Step 3 - Events */

// Calculate the minimum amount of events the house must have
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
    if (remainingMinEventCount == 0){
        ui->nextButton->setEnabled(true);
    }
}

void SIFRP::on_eventRandomButton_clicked(){
    int index = rollDice(3)-3;
    ui->eventComboBox->setCurrentIndex(index);

    calculateEventMods();

    if (remainingMinEventCount == 0){
        ui->nextButton->setEnabled(true);
    }
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

// Invest in defense
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

    ui->nextButton->setEnabled(true);
}

// Add holding to a table
void SIFRP::addHolding(Holding holding, QString secondColumn, QString thirdColumn){
    QTableWidget* table;
    QComboBox* box;

    switch (holding) {
    case Defense:
        table = ui->step4InvestedDefenseHoldingsTable;
        box = ui->step4DefenseHoldingComboBox;
        break;
    case Influence:
        table = ui->step4HeirTable;
        box = ui->step4HeirComboBox;
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

// invest in a heir
void SIFRP::on_step4InvestInHeirButton_clicked(){
    ui->nextButton->setEnabled(true);

    int influence = resources[1];
    int invInfluence = ui->step4InvestedInfluenceLine->text().toInt();
    int cost = 0;
    QString secondColumn;

    switch (ui->step4HeirComboBox->currentIndex()) {
    case 0:
        cost = 20;
        secondColumn = "Minimum-1";
        break;
    case 1:
        cost = 10;
        secondColumn = "Minimum-2";
        break;
    case 2:
        cost = 5;
        secondColumn = "Minimum-3";
        break;
    default:
        break;
    }

    if (invInfluence+cost <= influence){
        addHolding(Holding::Influence, secondColumn, QString::number(cost));
        ui->step4InvestedInfluenceLine->setText(QString::number(invInfluence+cost));
    }
}

// Land Holdings

void SIFRP::initializeStepFour(){
    // Influence Holdings
    ui->step4InfluenceLine->setText(QString::number(resources[1]));
    ui->step4InvestedInfluenceLine->setText(QString::number(0));

    // Land Holdings
    QStringList possibleTerrains;
    QStringList possibleFeatures;
    ui->step4AddFeatureButton->setEnabled(false);

    switch (ui->realmComboBox->currentIndex()) {
    case 0: // King's Landing
        possibleTerrains.append("Plains (5)");
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Grassland (1)", "Road (5)", "Ruin (3)", "Water (1-7)", "Woods (3-5)"});
        break;
    case 1: // Dragonstone
        possibleTerrains.append({"Hills (7)", "Plains (5)", "Wetlands (3)"});
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Grassland (1)", "Island (10)", "Road (5)", "Ruin (3)"});
        break;
    case 2: // The North
        possibleTerrains.append({"Hills (7)", "Mountains (9)", "Plains (5)", "Wetlands (3)"});
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Grassland (1)", "Island (10)", "Road (5)", "Ruin (3)", "Water (1-7)", "Woods (3-5)"});
        break;
    case 3: // The Iron Islands
        possibleTerrains.append({"Hills (7)", "Plains (5)"});
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Grassland (1)", "Island (10)", "Road (5)", "Ruin (3)"});
        break;
    case 4: // The Riverlands
        possibleTerrains.append({"Hills (7)", "Plains (5)", "Wetlands (3)"});
        possibleFeatures.append({"Community (10-50)", "Grassland (1)", "Road (5)", "Ruin (3)", "Water (1-7)"});
        break;
    case 5: // The Mountains of the Moon
        possibleTerrains.append({"Hills (7)", "Mountains (9)"});
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Grassland (1)", "Island (10)", "Road (5)", "Ruin (3)", "Water (1-7)"});
        break;
    case 6: // The Westerlands
        possibleTerrains.append({"Hills (7)", "Mountains (9)", "Plains (5)"});
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Grassland (1)", "Island (10)", "Road (5)", "Ruin (3)", "Water (1-7)"});
        break;
    case 7: // The Reach
        possibleTerrains.append("Plains (5)");
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Grassland (1)", "Island (10)", "Road (5)", "Ruin (3)", "Water (1-7)"});
        break;
    case 8: // The Stormlands
        possibleTerrains.append({"Hills (7)", "Mountains (9)", "Plains (5)", "Wetlands (3)"});
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Grassland (1)", "Island (10)", "Road (5)", "Ruin (3)", "Water (1-7)", "Woods (3-5)"});
        break;
    case 9: // Dorne
        possibleTerrains.append({"Hills (7)", "Mountains (9)", "Plains (5)"});
        possibleFeatures.append({"Coast (3)", "Community (10-50)", "Island (10)", "Road (5)", "Ruin (3)", "Water (1-7)"});
        break;
    default:
        break;
    }

    // Add Terrains to ComboBox
    ui->step4TerrainComboBox->clear();
    for (int i = 0; i<possibleTerrains.size(); i++){
        ui->step4TerrainComboBox->addItem(possibleTerrains.at(i));
    }

    // Add Features to ComboBox
    for (int i = 0; i<possibleFeatures.size(); i++){
        QString feature = possibleFeatures.at(i);

        if (feature == "Community (10-50)"){
            ui->step4FeatureComboBox->addItem("Hamlet (10)");
            ui->step4FeatureComboBox->addItem("Small Town (20)");
            ui->step4FeatureComboBox->addItem("Large Town (30)");
            ui->step4FeatureComboBox->addItem("Small City (40)");
            ui->step4FeatureComboBox->addItem("Large City (50)");

        }else if (feature == "Water (1-7)"){
            ui->step4FeatureComboBox->addItem("Stream (1)");
            ui->step4FeatureComboBox->addItem("River (3)");
            ui->step4FeatureComboBox->addItem("Pond (5)");
            ui->step4FeatureComboBox->addItem("Lake (7)");

        }else if (feature == "Woods (3-5)"){
            ui->step4FeatureComboBox->addItem("Light Woods (3)");
            ui->step4FeatureComboBox->addItem("Dense Woods (5)");

        }else {
            ui->step4FeatureComboBox->addItem(feature);
        }
    }

    ui->step4LandsLine->setText(QString::number(resources[2]));
    ui->step4InvLandsLine->setText(QString::number(0));

    ui->step4CreateDomainButton->setDisabled(true);

    // Law Holdings
    int law = resources[3];
    int lawHouseFortunes = 0;

    if (law == 0){
        lawHouseFortunes = -20;
    }else if (law <11){
        lawHouseFortunes = -10;
    }else if (law <21){
        lawHouseFortunes = -5;
    }else if (law <31){
        lawHouseFortunes = -2;
    }else if (law <41){
        lawHouseFortunes = -1;
    }else if (law <51){
        lawHouseFortunes = 0;
    }else if (law <61){
        lawHouseFortunes = 1;
    }else if (law <71){
        lawHouseFortunes = 2;
    }else {
        lawHouseFortunes = 5;
    }

    ui->step4LawScoreLine->setText(QString::number(law));
    ui->step4LawFortuneModifierLine->setText(QString::number(lawHouseFortunes));

    // Population Holdings
    int population = resources[4];
    int populationHouseFortunes = 0;

    if (population == 0){
        populationHouseFortunes = -10;
    }else if (population <11){
        populationHouseFortunes = -5;
    }else if (population <21){
        populationHouseFortunes = 0;
    }else if (population <31){
        populationHouseFortunes = 1;
    }else if (population <41){
        populationHouseFortunes = 3;
    }else if (population <51){
        populationHouseFortunes = 1;
    }else if (population <61){
        populationHouseFortunes = 0;
    }else if (population <71){
        populationHouseFortunes = -5;
    }else {
        populationHouseFortunes = -10;
    }

    ui->step4PopulationLine->setText(QString::number(population));
    ui->step4PopulationFortuneModifierLine->setText(QString::number(populationHouseFortunes));

    // Power Holdings
    ui->step4PowerLine->setText(QString::number(resources[5]));
    ui->step4InvestedPower->setText(QString::number(0));
    ui->step4UnitsComboBox->setCurrentIndex(1);
    ui->step4UnitsComboBox->setCurrentIndex(0);

    // Wealth Holdings
    ui->step4WealthLine->setText(QString::number(resources[6]));
    ui->step4InvestedWealthLine->setText(QString::number(0));
    ui->step4WealthHoldingComboBox->setCurrentIndex(1);
    ui->step4WealthHoldingComboBox->setCurrentIndex(0);
}

// Set terrain of a domain
void SIFRP::on_step4SetTerrainButton_clicked()
{
    int cost = 0;
    QString terrainName = ui->step4TerrainComboBox->currentText();

    if (terrainName == "Hills (7)"){
        cost = 7;
    }else if (terrainName == "Mountains (9)"){
        cost = 9;
    }else if (terrainName == "Plains (5)"){
        cost = 5;
    }else { // Wetlands
        cost = 3;
    }

    if (ui->step4InvLandsLine->text().toInt()+cost <= resources[2]){
        ui->step4TerrainComboBox->setDisabled(true);
        ui->step4CreateDomainButton->setEnabled(true);
        ui->step4SetTerrainButton->setDisabled(true);

        ui->step4TerrainLine->setText(terrainName);

        ui->step4InvLandsLine->setText(QString::number(ui->step4InvLandsLine->text().toInt()+cost));

        ui->step4AddFeatureButton->setEnabled(true);
    }
}

// Add a feature to a domain
void SIFRP::on_step4AddFeatureButton_clicked()
{
    int cost = 0;
    QString feature = ui->step4FeatureComboBox->currentText();

    QString temp;
    for (int i = feature.indexOf("(")+1; i<feature.lastIndexOf(")"); i++){
        temp.append(feature.at(i));
    }
    cost = temp.toInt();

    if (ui->step4InvLandsLine->text().toInt()+cost <= resources[2]){
        if (ui->step4FeaturesLine->text() != "" || ui->step4FeaturesLine->text() != NULL){
            ui->step4FeaturesLine->setText(ui->step4FeaturesLine->text()+", "+ui->step4FeatureComboBox->currentText());
        }else {
            ui->step4FeaturesLine->setText(ui->step4FeaturesLine->text()+ui->step4FeatureComboBox->currentText());
        }

        ui->step4InvLandsLine->setText(QString::number(ui->step4InvLandsLine->text().toInt()+cost));
    }
}

// Create Domain
void SIFRP::on_step4CreateDomainButton_clicked()
{
    ui->step4TerrainComboBox->setEnabled(true);
    ui->step4SetTerrainButton->setEnabled(true);
    ui->nextButton->setEnabled(true);

    ui->step4DomainsTable->insertRow(0);

    QTableWidgetItem* i1 = new QTableWidgetItem();
    QTableWidgetItem* i2 = new QTableWidgetItem();

    i1->setText(ui->step4TerrainLine->text());
    i2->setText(ui->step4FeaturesLine->text());

    ui->step4DomainsTable->setItem(0, 0, i1);
    ui->step4DomainsTable->setItem(0, 1, i2);

    // Clear Line Edits
    ui->step4TerrainLine->clear();
    ui->step4FeaturesLine->clear();

    ui->step4CreateDomainButton->setDisabled(true);
    ui->step4AddFeatureButton->setDisabled(true);

    ui->nextButton->setEnabled(true);
}

// When a unit type is selected
void SIFRP::on_step4UnitsComboBox_currentIndexChanged(int index)
{
    int cost = 0;
    int disciplineMod = 0;
    QString abilities;

    switch (index) {
    case 0: // Archers
        cost = 3;
        disciplineMod = 3;
        abilities = "Agility, Awareness, Marksmanship";
        break;
    case 1: // Cavalry
        cost = 5;
        disciplineMod = -3;
        abilities = "Agility, Animal Handling, Fighting";
        break;
    case 2: // Criminals
        cost = 1;
        disciplineMod = 6;
        abilities = "Endurance, Fighting, Stealth";
        break;
    case 3: // Crusaders
        cost = 4;
        disciplineMod = 0;
        abilities = "Athletics, Endurance, Fighting";
        break;
    case 4: // Engineers
        cost = 2;
        disciplineMod = 3;
        abilities = "Endurance, Fighting, Warfare";
        break;
    case 5: // Garrison
        cost = 2;
        disciplineMod = -3;
        abilities = "Awareness, Endurance, Fighting";
        break;
    case 6: // Guerillas
        cost = 2;
        disciplineMod = 3;
        abilities = "Athletics, Marksmanship, Stealth";
        break;
    case 7: // Infantry
        cost = 4;
        disciplineMod = 0;
        abilities = "Athletics, Endurance, Fighting";
        break;
    case 8: // Peasant Levies
        cost = 0;
        disciplineMod = 6;
        abilities = "Animal Handling, Awareness, Survival";
        break;
    case 9: // Personal Guards
        cost = 6;
        disciplineMod = -6;
        abilities = "Athletics, Endurance, Fighting";
        break;
    case 10: // Raiders
        cost = 3;
        disciplineMod = 3;
        abilities = "Agility, Endurance, Fighting";
        break;
    case 11: // Sailors
        cost = 4;
        disciplineMod = 0;
        abilities = "Agility, Awareness, Fighting";
        break;
    case 12: // Scouts
        cost = 2;
        disciplineMod = 3;
        abilities = "Awareness, Endurance, Stealth";
        break;
    case 13: // Special
        cost = 4;
        disciplineMod = 0;
        abilities = "Any Three";
        break;
    case 14: // Support
        cost = 2;
        disciplineMod = 3;
        abilities = "Animal Handling, Endurance, Healing";
        break;
    case 15: // Warships
        cost = 7;
        disciplineMod = 0;
        abilities = "Awareness, Fighting, Marksmanship";
        break;
    default:
        break;
    }

    ui->step4PowerCostLine->setText(QString::number(cost));
    ui->step4DisciplineModifierLine->setText(QString::number(disciplineMod));

    ui->step4AbilitiesEdit->clear();

    ui->step4AbilitiesEdit->setText(abilities);

}

// Investing in a Unit
void SIFRP::on_step4BuyUnitsButton_clicked()
{
    int cost = 0;
    int discipline = 0;
    int power = resources[5];
    int investedPower = ui->step4InvestedPower->text().toInt();
    QString training;
    QString abilities;

    switch (ui->step4TrainingComboBox->currentIndex()) {
    case 0:
        training = "Green";
        cost = 1;
        discipline = 9;
        break;
    case 1:
        training = "Trained";
        cost = 3;
        discipline = 6;
        break;
    case 2:
        training = "Veteran";
        cost = 5;
        discipline = 3;
        break;
    case 3:
        training = "Elite";
        cost = 7;
        discipline = 0;
        break;
    default:
        break;
    }

    cost += ui->step4PowerCostLine->text().toInt();
    discipline += ui->step4DisciplineModifierLine->text().toInt();
    abilities = ui->step4AbilitiesEdit->toPlainText();

    if (cost+investedPower <= power){
        ui->step4UnitsTable->insertRow(0);

        QTableWidgetItem* i1 = new QTableWidgetItem;
        i1->setText(ui->step4UnitsComboBox->currentText());

        QTableWidgetItem* i2 = new QTableWidgetItem;
        i2->setText(training);

        QTableWidgetItem* i3 = new QTableWidgetItem;
        i3->setText(QString::number(cost));

        QTableWidgetItem* i4 = new QTableWidgetItem;
        i4->setText(QString::number(discipline));

        QTableWidgetItem* i5 = new QTableWidgetItem;
        i5->setText(abilities);

        ui->step4UnitsTable->setItem(0, 0, i1);
        ui->step4UnitsTable->setItem(0, 1, i2);
        ui->step4UnitsTable->setItem(0, 2, i3);
        ui->step4UnitsTable->setItem(0, 3, i4);
        ui->step4UnitsTable->setItem(0, 4, i5);

        ui->step4InvestedPower->setText(QString::number(investedPower+cost));

        ui->nextButton->setEnabled(true);
    }
}

// Invest in a Banner House
void SIFRP::on_step4InvestInBannerHouse_clicked()
{
    int power = resources[5];
    int investedPower = ui->step4InvestedPower->text().toInt();

    int bannerHouses = ui->step4AmountOfBannerHousesLine->text().toInt();
    int cost = 0;

    switch (bannerHouses) {
    case 0:
        cost = 20;
        break;
    case 1:
        cost = 10;
        break;
    default:
        cost = 5;
        break;
    }

    if (cost+investedPower <= power){
        ui->step4AmountOfBannerHousesLine->setText(QString::number(bannerHouses+1));

        ui->step4InvestedPower->setText(QString::number(cost+investedPower));

        ui->nextButton->setEnabled(true);
    }
}

void SIFRP::on_step4WealthHoldingComboBox_currentIndexChanged(int index)
{
    QString description;
    QString time;
    QString requirement;

    switch (index) {
    case 0: // Artisan
        description = "Your house aquires the service of a master artisan.";
        requirement = "Hall or larger defensive structure";
        time = "2D6 Months";
        break;
    case 1: // Godswood
        description = "Your home has a godswood, a place sacred to the old gods.";
        requirement = "Realm: The North";
        time = "24+2D6 Months";
        break;
    case 2: // Guilds
        description = "A Guild controls manufacturing and pricing of commodities.";
        requirement = "Small town or larger community";
        time = "2D6 Months";
        break;
    case 3: // Maester
        description = "All houses can benefit from the wisdom and learning of a maester.";
        requirement = "Influence 20+";
        time = "1D6 Months";
        break;
    case 4: // Marketplace
        description = "A Marketplace facilitates trade and draws merchants from abroad.";
        requirement = "Small town or larger community";
        time = "1D6 Months";
        break;
    case 5: // Mine
        description = "You open mines on your lands to generate additional income.";
        requirement = "Mountains or hills";
        time = "24+2D6 Months";
        break;
    case 6: // Port
        description = "A port enables merchant ships to come to your lands.";
        requirement = "Coastline";
        time = "3D6 Months";
        break;
    case 7: // Sept
        description = "You erect a sept of the Faith to show your family's piety.";
        requirement = "Hall or larger defensive structure or small town or larger community";
        time = "12+2D6 Months";
        break;
    default:
        break;
    }

    ui->step4WealthDescriptionEdit->setText(description);
    ui->step4WealthRequirementEdit->setText(requirement);
    ui->step4WealthTimeLine->setText(time);
}

// Invest in a wealth holding
void SIFRP::on_step4InvestInWealthHoldingButton_clicked()
{
    int wealth = resources[6];
    int investedWealth = ui->step4InvestedWealthLine->text().toInt();

    int time = 0;
    int cost = 0;
    QString holding;

    switch (ui->step4WealthHoldingComboBox->currentIndex()) {
    case 0: // Artisan
        time = rollDice(2);
        cost = 10;
        holding = "Artisan";
        break;
    case 1: // Godswood
        time = 24+rollDice(2);
        cost = 5;
        holding = "Godswood";
        break;
    case 2: // Guilds
        time = rollDice(2);
        cost = 15;
        holding = "Guilds";
        break;
    case 3: // Maester
        time = rollDice(1);
        cost = 10;
        holding = "Maester";
        break;
    case 4: // Marketplace
        time = rollDice(1);
        cost = 10;
        holding = "Marketplace";
        break;
    case 5: // Mine
        time = 24+rollDice(2);
        cost = 10;
        holding = "Mine";
        break;
    case 6: // Port
        time = rollDice(3);
        cost = 10;
        holding = "Port";
        break;
    case 7: // Sept
        time = 12+rollDice(2);
        cost = 15;
        holding = "Sept";
        break;
    default:
        break;
    }

    if (cost+investedWealth <= wealth){
        QString timeText = QString::number(time)+" Months";

        QString description = ui->step4WealthDescriptionEdit->toPlainText();

        QTableWidgetItem* i1 = new QTableWidgetItem;
        i1->setText(holding);

        QTableWidgetItem* i2 = new QTableWidgetItem;
        i2->setText(QString::number(cost));

        QTableWidgetItem* i3 = new QTableWidgetItem;
        i3->setText(timeText);

        QTableWidgetItem* i4 = new QTableWidgetItem;
        i4->setText(description);
        i4->setToolTip(description);

        ui->step4WealthTable->insertRow(0);
        ui->step4WealthTable->setItem(0, 0, i1);
        ui->step4WealthTable->setItem(0, 1, i2);
        ui->step4WealthTable->setItem(0, 2, i3);
        ui->step4WealthTable->setItem(0, 3, i4);

        ui->step4InvestedWealthLine->setText(QString::number(cost+investedWealth));
    }

    ui->nextButton->setEnabled(true);
}

// Save House
void SIFRP::on_saveHouseButton_clicked()
{
    qDebug() << "Saving SIFRP House...";

    // Get House Name and generate Filename from that
    QString filename = ui->houseNameLine->text()+".txt";
    filename = filename.replace("House ", "");

    // Check if the directory "SIFRP-Houses" exists and create it if not
    QDir dir(QDir::homePath()+"/.gm-companion/notes/SIFRP-Houses");
    if (!dir.exists()){
        qDebug() << "SIFRP House directory does not exist. Creating...";
        dir.mkpath(".");
    }

    // Write everything to the file
    QFile file(QDir::homePath()+"/.gm-companion/notes/SIFRP-Houses/"+filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);

        // House Name
        stream << "Name: " << ui->houseNameLine->text() << endl;

        // House Words
        stream << "Words: " << ui->houseWordsLine->text() << endl;

        // Realm
        stream << "Realm: " << ui->realmComboBox->currentText() << endl;

        // House Founding
        stream << "Founding: " << ui->foundingComboBox->currentText() << " (Example: " << ui->foundingExampleLineEdit->text() << ")" << endl;
        stream << endl;

        // Resources
        stream << "Resources:" << endl;
        stream << " - Defense: " << resources[0] << " (Invested: " << ui->step4InvestedDefenseLine->text() << ") (" << ui->resourcesTable->item(1, 1)->text() << ")" << endl;
        stream << " - Influence: " << resources[1] << " (Invested: " << ui->step4InvestedInfluenceLine->text() << ") (" << ui->resourcesTable->item(2, 1)->text() << ")" << endl;
        stream << " - Lands: " << resources[2] << " (Invested: " << ui->step4InvLandsLine->text() << ") (" << ui->resourcesTable->item(3, 1)->text() << ")" << endl;
        stream << " - Law: " << resources[3] << " (Fortunes Modifier: " << ui->step4LawFortuneModifierLine->text() << ") (" << ui->resourcesTable->item(4, 1)->text() << ")" << endl;
        stream << " - Population: " << resources[4] << " (Fortunes Modifier: " << ui->step4PopulationFortuneModifierLine->text() << ") (" << ui->resourcesTable->item(5, 1)->text() << ")" << endl;
        stream << " - Power: " << resources[5] << " (Invested: " << ui->step4InvestedPower->text() << ") (" << ui->resourcesTable->item(6, 1)->text() << ")" << endl;
        stream << " - Wealth: " << resources[6] << " (Invested: " << ui->step4InvestedWealthLine->text() << ") (" << ui->resourcesTable->item(7, 1)->text() << ")" << endl;
        stream << endl;

        // House Fortunes
        stream << "House Fortunes:" << endl;
        stream << " - Law Modifier: " << ui->step4LawFortuneModifierLine->text() << endl;
        stream << " - Population Modifier: " << ui->step4PopulationFortuneModifierLine->text() << endl;
        stream << endl;

        // Historical events
        stream << "Historical Events:" << endl;
        for (int i = 0; i<ui->eventTableWidget->rowCount(); i++){
            QString event = ui->eventTableWidget->item(i, 0)->text();
            if (ui->eventTableWidget->item(i, 1)->text() != ""){
                event = event + " (" + ui->eventTableWidget->item(i, 1)->text() + ")";
            }
            stream << " - " << event << endl;
        }
        stream << endl;

        // Defense Holdings
        stream << "Defense Holdings:" << endl;
        for (int i = 0; i<ui->step4DefenseHoldingsTable->rowCount(); i++){
            QString holding = ui->step4DefenseHoldingsTable->item(i, 0)->text();
            holding += " (Build Time: " + ui->step4DefenseHoldingsTable->item(i, 2)->text() + ")";
            stream << " - " << holding << endl;
        }
        stream << endl;

        // Influence Holdings
        stream << "Influence Holdings:" << endl;
        for (int i = 0; i<ui->step4HeirTable->rowCount(); i++){
            QString holding = ui->step4HeirTable->item(i, 0)->text();
            holding += " (Status: " + ui->step4HeirTable->item(i, 1)->text() + ")";
            stream << " - " << holding << endl;
        }
        stream << endl;

        // Land Holdings
        stream << "Land Holdings:" << endl;
        for (int i = 0; i<ui->step4DomainsTable->rowCount(); i++){
            QString holding = ui->step4DomainsTable->item(i, 0)->text();
            holding += " (Features: " + ui->step4DomainsTable->item(i, 1)->text() + ")";
            stream << " - " << holding << endl;
        }
        stream << endl;

        // Power Holdings
        stream << "Power Holdings:" << endl;
        stream << "Banner Houses: " << ui->step4AmountOfBannerHousesLine->text() << endl;
        stream << endl;
        stream << "Units:" << endl;
        for (int i = 0; i<ui->step4UnitsTable->rowCount(); i++){
            QString holding = ui->step4UnitsTable->item(i, 0)->text();
            holding += " (Training: " + ui->step4UnitsTable->item(i, 1)->text() + ")";
            holding += " (Discipline: " + ui->step4UnitsTable->item(i, 3)->text() + ")";
            holding += " (Key Abilities: " + ui->step4UnitsTable->item(i, 4)->text() + ")";
            stream << " - " << holding << endl;
        }
        stream << endl;

        // Wealth Holdings
        stream << "Wealth Holdings:" << endl;
        for (int i = 0; i<ui->step4WealthTable->rowCount(); i++){
            QString holding = ui->step4WealthTable->item(i, 0)->text();
            holding += " (Time: " + ui->step4WealthTable->item(i, 1)->text() + ")";
            holding += " (" + ui->step4WealthTable->item(i, 2)->text() + ")";
            stream << " - " << holding << endl;
        }
        stream << endl;

    }
    file.close();
    qDebug() << "Done.";
}

// Reset everything
void SIFRP::on_startNewHouseButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->step4StackedWidget->setCurrentIndex(0);

    // Realm
    ui->step1SetRealmButton->setEnabled(true);
    ui->realmComboBox->setEnabled(true);
    ui->realmComboBox->setCurrentIndex(0);
    ui->randomRealm->setEnabled(true);

    // Starting Resources
    ui->playersSpinBox->setValue(1);
    ui->step2SetPlayersButton->setEnabled(true);
    ui->playersSpinBox->setEnabled(true);
    ui->step2AddButton->setEnabled(false);
    ui->stepTwoResourceComboBox->setEnabled(false);

    // House History
    ui->foundingApplyButton->setEnabled(true);
    ui->foundingComboBox->setEnabled(true);
    ui->foundingComboBox->setCurrentIndex(0);
    ui->foundingRandomButton->setEnabled(true);

    ui->eventApplyButton->setEnabled(false);
    ui->eventComboBox->setEnabled(false);
    ui->eventRandomButton->setEnabled(false);
    ui->eventTableWidget->setRowCount(0);

    // Defense Holdings
    ui->step4InvestedDefenseLine->setText(QString::number(0));
    ui->step4DefenseHoldingComboBox->setCurrentIndex(0);
    ui->step4InvestedDefenseHoldingsTable->setRowCount(0);

    // Influence Holdings
    ui->step4HeirComboBox->setCurrentIndex(0);
    ui->step4InvestedInfluenceLine->setText(QString::number(0));
    ui->step4HeirTable->setRowCount(0);

    // Lands Holdings
    ui->step4InvLandsLine->setText(QString::number(0));
    ui->step4DomainsTable->setRowCount(0);
    ui->step4AddFeatureButton->setEnabled(false);
    ui->step4SetTerrainButton->setEnabled(true);
    ui->step4TerrainComboBox->setEnabled(true);
    ui->step4CreateDomainButton->setEnabled(false);
    ui->step4FeaturesLine->clear();
    ui->step4TerrainLine->clear();

    // Power Holdings
    ui->step4InvestedPower->setText(QString::number(0));
    ui->step4TrainingComboBox->setCurrentIndex(0);
    ui->step4UnitsComboBox->setCurrentIndex(0);
    ui->step4UnitsTable->setRowCount(0);
    ui->step4AmountOfBannerHousesLine->setText(QString::number(0));

    // Wealth Holdings
    ui->step4InvestedWealthLine->setText(QString::number(0));
    ui->step4WealthHoldingComboBox->setCurrentIndex(0);
    ui->step4WealthTable->setRowCount(0);

    // Last Page
    ui->houseNameLine->setText("House ");
    ui->houseWordsLine->clear();

}

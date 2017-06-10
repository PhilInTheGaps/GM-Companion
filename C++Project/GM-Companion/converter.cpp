#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

void MainWindow::initializeUnits(){
    // Getting Default Values
    QSettings settings(QDir::homePath()+"/.gm-companion/units.ini", QSettings::IniFormat);
    int size = settings.beginReadArray("LengthUnits");

    // Length
    for (int i = 0; i<size; i++){
        settings.setArrayIndex(i);
        Unit unit;
        unit.name = settings.value("name", "unknown").toString();
        unit.refUnits = settings.value("refUnits", 1).toDouble();

        lengthUnits.push_back(unit);

        ui->length1ComboBox->addItem(unit.name);
        ui->length2ComboBox->addItem(unit.name);

    }
    settings.endArray();

    // Area
    size = settings.beginReadArray("AreaUnits");
    for (int i = 0; i<size; i++){
        settings.setArrayIndex(i);

        Unit unit;
        unit.name = settings.value("name", "unknown").toString();
        unit.refUnits = settings.value("refUnits", 1).toDouble();

        areaUnits.push_back(unit);

        ui->area1ComboBox->addItem(unit.name);
        ui->area2ComboBox->addItem(unit.name);
    }
    settings.endArray();

    // Volume
    size = settings.beginReadArray("VolumeUnits");
    for (int i = 0; i<size; i++){
        settings.setArrayIndex(i);

        Unit unit;
        unit.name = settings.value("name", "unknown").toString();
        unit.refUnits = settings.value("refUnits", 1).toDouble();

        volumeUnits.push_back(unit);

        ui->volume1ComboBox->addItem(unit.name);
        ui->volume2ComboBox->addItem(unit.name);
    }
    settings.endArray();

    // Weight
    size = settings.beginReadArray("WeightUnits");
    for (int i = 0; i<size; i++){
        settings.setArrayIndex(i);

        Unit unit;
        unit.name = settings.value("name", "unknown").toString();
        unit.refUnits = settings.value("refUnits", 1).toDouble();

        weightUnits.push_back(unit);

        ui->weight1ComboBox->addItem(unit.name);
        ui->weight2ComboBox->addItem(unit.name);
    }
    settings.endArray();

    // Money
    size = settings.beginReadArray("MoneyUnits");
    for (int i = 0; i<size; i++){
        settings.setArrayIndex(i);

        Unit unit;
        unit.name = settings.value("name", "unknown").toString();
        unit.refUnits = settings.value("refUnits", 1).toDouble();

        moneyUnits.push_back(unit);

        ui->money1ComboBox->addItem(unit.name);
        ui->money2ComboBox->addItem(unit.name);
    }
    settings.endArray();

    // Getting Addon Units
    for (QString addon : getFolders(QDir::homePath()+"/.gm-companion/addons")){
        if (QFile(QDir::homePath()+"/.gm-companion/addons/"+addon+"/units.ini").exists() && settingsManager->getIsAddonEnabled(addon)){
            QSettings addonSettings(QDir::homePath()+"/.gm-companion/addons/"+addon+"/units.ini", QSettings::IniFormat);

            // Length
            size = addonSettings.beginReadArray("LengthUnits");
            for (int i = 0; i<size; i++){
                addonSettings.setArrayIndex(i);
                Unit unit;
                unit.name = addonSettings.value("name", "unknown").toString();
                unit.refUnits = addonSettings.value("refUnits", 1).toDouble();

                lengthUnits.push_back(unit);

                ui->length1ComboBox->addItem(unit.name);
                ui->length2ComboBox->addItem(unit.name);

            }
            addonSettings.endArray();

            // Area
            size = addonSettings.beginReadArray("AreaUnits");
            for (int i = 0; i<size; i++){
                addonSettings.setArrayIndex(i);
                Unit unit;
                unit.name = addonSettings.value("name", "unknown").toString();
                unit.refUnits = addonSettings.value("refUnits", 1).toDouble();

                areaUnits.push_back(unit);

                ui->area1ComboBox->addItem(unit.name);
                ui->area2ComboBox->addItem(unit.name);

            }
            addonSettings.endArray();

            // Volume
            size = addonSettings.beginReadArray("VolumeUnits");
            for (int i = 0; i<size; i++){
                addonSettings.setArrayIndex(i);
                Unit unit;
                unit.name = addonSettings.value("name", "unknown").toString();
                unit.refUnits = addonSettings.value("refUnits", 1).toDouble();

                volumeUnits.push_back(unit);

                ui->volume1ComboBox->addItem(unit.name);
                ui->volume2ComboBox->addItem(unit.name);

            }
            addonSettings.endArray();

            // Weight
            size = addonSettings.beginReadArray("WeightUnits");
            for (int i = 0; i<size; i++){
                addonSettings.setArrayIndex(i);
                Unit unit;
                unit.name = addonSettings.value("name", "unknown").toString();
                unit.refUnits = addonSettings.value("refUnits", 1).toDouble();

                weightUnits.push_back(unit);

                ui->weight1ComboBox->addItem(unit.name);
                ui->weight2ComboBox->addItem(unit.name);

            }
            addonSettings.endArray();

            // Money
            size = addonSettings.beginReadArray("MoneyUnits");
            for (int i = 0; i<size; i++){
                addonSettings.setArrayIndex(i);
                Unit unit;
                unit.name = addonSettings.value("name", "unknown").toString();
                unit.refUnits = addonSettings.value("refUnits", 1).toDouble();

                moneyUnits.push_back(unit);

                ui->money1ComboBox->addItem(unit.name);
                ui->money2ComboBox->addItem(unit.name);

            }
            addonSettings.endArray();
        }
    }

    ui->length2SpinBox->setReadOnly(true);
    ui->area2SpinBox->setReadOnly(true);
    ui->volume2SpinBox->setReadOnly(true);
    ui->weight2SpinBox->setReadOnly(true);
    ui->money2SpinBox->setReadOnly(true);
}

// Add a unit
void MainWindow::on_addUnitButton_clicked(){
    QSettings settings(QDir::homePath()+"/.gm-companion/units.ini", QSettings::IniFormat);
    QString arrayName;

    QString name = ui->converterUnitName->text();
    double refUnits = ui->converterRefUnitSpinBox->value();

    Unit unit;
    unit.name = name;
    unit.refUnits = refUnits;

    switch (ui->converterTypeComboBox->currentIndex()) {
    case 0:
        arrayName = "LengthUnits";
        lengthUnits.push_back(unit);
        ui->length1ComboBox->addItem(name);
        ui->length2ComboBox->addItem(name);
        break;
    case 1:
        arrayName = "AreaUnits";
        areaUnits.push_back(unit);
        ui->area1ComboBox->addItem(name);
        ui->area2ComboBox->addItem(name);
        break;
    case 2:
        arrayName = "VolumeUnits";
        volumeUnits.push_back(unit);
        ui->volume1ComboBox->addItem(name);
        ui->volume2ComboBox->addItem(name);
        break;
    case 3:
        arrayName = "WeightUnits";
        weightUnits.push_back(unit);
        ui->weight1ComboBox->addItem(name);
        ui->weight2ComboBox->addItem(name);
        break;
    case 4:
        arrayName = "MoneyUnits";
        moneyUnits.push_back(unit);
        ui->money1ComboBox->addItem(name);
        ui->money2ComboBox->addItem(name);
        break;
    }

    int size = settings.beginReadArray(arrayName);
    settings.endArray();

    settings.beginWriteArray(arrayName);
    settings.setArrayIndex(size);
    settings.setValue("name", name);
    settings.setValue("refUnits", refUnits);
    settings.endArray();
}

// Display the correct reference unit
void MainWindow::on_converterTypeComboBox_currentIndexChanged(int index){
    switch (index) {
    case 0:
        ui->converterRefUnitName->setText("Meters");
        break;
    case 1:
        ui->converterRefUnitName->setText("Square Meters");
        break;
    case 2:
        ui->converterRefUnitName->setText("Cubic Meters");
        break;
    case 3:
        ui->converterRefUnitName->setText("Kilograms");
        break;
    case 4:
        ui->converterRefUnitName->setText("US Dollars");
        break;
    }
}

// Calculate Length
void MainWindow::convertLength(int index1, int index2, double units){
    double refUnits1 = lengthUnits.at(index1).refUnits;
    double refUnits2 = lengthUnits.at(index2).refUnits;

    double value = refUnits1*units/refUnits2;

    ui->length2SpinBox->setValue(value);
}

// Calculate Area
void MainWindow::convertArea(int index1, int index2, double units){
    double refUnits1 = areaUnits.at(index1).refUnits;
    double refUnits2 = areaUnits.at(index2).refUnits;

    double value = refUnits1*units/refUnits2;

    ui->area2SpinBox->setValue(value);
}

// Calculate Volume
void MainWindow::convertVolume(int index1, int index2, double units){
    double refUnits1 = volumeUnits.at(index1).refUnits;
    double refUnits2 = volumeUnits.at(index2).refUnits;

    double value = refUnits1*units/refUnits2;

    ui->volume2SpinBox->setValue(value);
}

// Calculate Weight
void MainWindow::convertWeight(int index1, int index2, double units){
    double refUnits1 = weightUnits.at(index1).refUnits;
    double refUnits2 = weightUnits.at(index2).refUnits;

    double value = refUnits1*units/refUnits2;

    ui->weight2SpinBox->setValue(value);
}

// Calculate Money
void MainWindow::convertMoney(int index1, int index2, double units){
    double refUnits1 = moneyUnits.at(index1).refUnits;
    double refUnits2 = moneyUnits.at(index2).refUnits;

    double value = refUnits1*units/refUnits2;

    ui->money2SpinBox->setValue(value);
}

// Length Type 1 changes
void MainWindow::on_length1ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertLength(index, ui->length2ComboBox->currentIndex(), ui->length1SpinBox->value());
}

// Length Type 2 changes
void MainWindow::on_length2ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertLength(ui->length1ComboBox->currentIndex(), index, ui->length1SpinBox->value());
}

// Area Type 1 changes
void MainWindow::on_area1ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertArea(index, ui->area2ComboBox->currentIndex(), ui->area1SpinBox->value());
}

// Area Type 2 changes
void MainWindow::on_area2ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertArea(ui->area1ComboBox->currentIndex(), index, ui->area1SpinBox->value());
}

// Length value changes
void MainWindow::on_length1SpinBox_valueChanged(double value){
    if (!programStart)
        convertLength(ui->length1ComboBox->currentIndex(), ui->length2ComboBox->currentIndex(), value);
}

// Area value changes
void MainWindow::on_area1SpinBox_valueChanged(double value){
    if (!programStart)
        convertArea(ui->area1ComboBox->currentIndex(), ui->area2ComboBox->currentIndex(), value);
}

// Volume Type 1 changes
void MainWindow::on_volume1ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertVolume(index, ui->volume2ComboBox->currentIndex(), ui->volume1SpinBox->value());
}

// Volume Type 2 changes
void MainWindow::on_volume2ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertVolume(ui->volume1ComboBox->currentIndex(), index, ui->volume1SpinBox->value());
}

// Volume value changes
void MainWindow::on_volume1SpinBox_valueChanged(double arg1){
    if (!programStart)
        convertVolume(ui->volume1ComboBox->currentIndex(), ui->volume2ComboBox->currentIndex(), arg1);
}

// Weight Type 1 changes
void MainWindow::on_weight1ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertWeight(index, ui->weight2ComboBox->currentIndex(), ui->weight1SpinBox->value());
}

// Weight Type 2 changes
void MainWindow::on_weight2ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertWeight(ui->weight1ComboBox->currentIndex(), index, ui->weight1SpinBox->value());
}

// Weight value changes
void MainWindow::on_weight1SpinBox_valueChanged(double arg1){
    if (!programStart)
        convertWeight(ui->weight1ComboBox->currentIndex(), ui->weight2ComboBox->currentIndex(), arg1);
}

// Money Type 1 changes
void MainWindow::on_money1ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertMoney(index, ui->money2ComboBox->currentIndex(), ui->money1SpinBox->value());
}

// Money Type 2 changes
void MainWindow::on_money2ComboBox_currentIndexChanged(int index){
    if (!programStart)
        convertMoney(ui->money1ComboBox->currentIndex(), index, ui->money1SpinBox->value());
}

// Money value changes
void MainWindow::on_money1SpinBox_valueChanged(double arg1){
    if (!programStart)
        convertMoney(ui->money1ComboBox->currentIndex(), ui->money2ComboBox->currentIndex(), arg1);
}

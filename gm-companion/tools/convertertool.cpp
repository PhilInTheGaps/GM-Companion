#include "convertertool.h"
#include "ui_convertertool.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QDir>

ConverterTool::ConverterTool(QWidget *parent) : QWidget(parent), ui(new Ui::ConverterTool)
{
    qDebug() << "Loading ConverterTool ...";

    ui->setupUi(this);

    settingsManager = new SettingsManager;

    on_comboBox_unitType_currentIndexChanged(0);

    getAllUnits();
}

ConverterTool::~ConverterTool()
{
    delete ui;
}

void ConverterTool::getAllUnits()
{
    qDebug() << "Loading units ...";

    for (int i = 0; i < 2; i++)
    {
        if (i == 1) // Addon Units
        {
            for (QString addon : getFolders(QDir::homePath()+"/.gm-companion/addons"))
                addUnitGroup(i, addon);
        }
        else
        {
            addUnitGroup(i, "");
        }
    }
}

void ConverterTool::addUnitGroup(int index, QString str)
{
    lengthUnits.append(getUnits("LengthUnits", index, ui->comboBox_length1, ui->comboBox_length2, str));
    areaUnits.append(getUnits("AreaUnits", index, ui->comboBox_area1, ui->comboBox_area2, str));
    volumeUnits.append(getUnits("VolumeUnits", index, ui->comboBox_volume1, ui->comboBox_volume2, str));
    weightUnits.append(getUnits("WeightUnits", index, ui->comboBox_weight1, ui->comboBox_weight2, str));
    moneyUnits.append(getUnits("MoneyUnits", index, ui->comboBox_money1, ui->comboBox_money2, str));
}

QList<ConverterTool::Unit> ConverterTool::getUnits(QString arrayName, int type, QComboBox *box1, QComboBox *box2, QString addon)
{
    QList<Unit> list;
    QString path;
    bool unitsExist = false;

    // Check if units are normal units or addon units
    switch (type) {
    case 1: // Addon
        path = QDir::homePath()+"/.gm-companion/addons/"+addon+"/units.ini";
        if (QFile(path).exists() && settingsManager->getIsAddonEnabled(addon))
            unitsExist = true;
        break;
    case 2: // Custom
        path = QDir::homePath()+"/.gm-companion/units/custom.ini";
        break;
    default: // Default
        path = ":/units/default.ini";
        unitsExist = true;
        break;
    }

    if (unitsExist)
    {
        qDebug() << path;

        QSettings settings(path, QSettings::IniFormat);
        int size = settings.beginReadArray(arrayName);

        qDebug() << "Size" << size;

        for (int i = 0; i<size; i++)
        {
            settings.setArrayIndex(i);

            qDebug() << "Index:" << i;

            Unit unit;
            unit.name = settings.value("name", "unknown").toString();
            unit.refUnits = settings.value("refUnits", 1).toDouble();

            qDebug() << "Name" << unit.name;
            qDebug() << "RefUnits" << unit.refUnits;

            list.push_back(unit);

            box1->addItem(unit.name);
            box2->addItem(unit.name);

        }
        settings.endArray();
    }

    return list;
}

// When unit type combo box is used
void ConverterTool::on_comboBox_unitType_currentIndexChanged(int index)
{
    // Display correct reference unit name
    QString refUnit;

    switch (index) {
    case 0: // Length
        refUnit = tr("Meters");
        break;
    case 1: // Area
        refUnit = tr("Square Meters");
        break;
    case 2: // Volume
        refUnit = tr("Cubic Meters");
        break;
    case 3: // Weight
        refUnit = tr("Kilograms");
        break;
    case 4: // Money
        refUnit = tr("Dollars");
        break;
    default:
        refUnit = "REF_UNIT";
        break;
    }

    ui->label_refUnit->setText(refUnit);
}

// Add a unit
void ConverterTool::on_pushButton_addUnit_clicked()
{
    if (!ui->lineEdit_unitName->text().isEmpty())
    {
        Unit unit;
        unit.name = ui->lineEdit_unitName->text();
        unit.refUnits = ui->doubleSpinBox_refUnits->value();

        switch (ui->comboBox_unitType->currentIndex()) {
        case 0: // Length
            addUnit("LengthUnits", unit, lengthUnits, ui->comboBox_length1, ui->comboBox_length2);
            break;
        case 1: // Area
            addUnit("LengthUnits", unit, areaUnits, ui->comboBox_area1, ui->comboBox_area2);
            break;
        case 2: // Volume
            addUnit("LengthUnits", unit, volumeUnits, ui->comboBox_volume1, ui->comboBox_volume2);
            break;
        case 3: // Weight
            addUnit("LengthUnits", unit, weightUnits, ui->comboBox_weight1, ui->comboBox_weight2);
            break;
        case 4: // Money
            addUnit("LengthUnits", unit, moneyUnits, ui->comboBox_money1, ui->comboBox_money2);
            break;
        default:
            break;
        }
    }
}

void ConverterTool::addUnit(QString arrayName, Unit unit, QList<Unit> list, QComboBox *box1, QComboBox *box2)
{
    list.push_back(unit);

    QSettings settings(QDir::homePath()+"/.gm-companion/custom.ini", QSettings::IniFormat);
    int size = settings.beginReadArray(arrayName);
    settings.endArray();

    settings.beginWriteArray(arrayName);
    settings.setArrayIndex(size);
    settings.setValue("name", unit.name);
    settings.setValue("refUnits", unit.refUnits);
    settings.endArray();

    box1->addItem(unit.name);
    box2->addItem(unit.name);
}

// Convert Units
void ConverterTool::convertUnits(int index1, int index2, QList<Unit> list, double units, QDoubleSpinBox *box)
{
    if (!list.isEmpty())
    {
        double refUnits1 = list.at(index1).refUnits;
        double refUnits2 = list.at(index2).refUnits;

        double value = refUnits1*units/refUnits2;
        box->setValue(value);
    }
}

// Length Units
void ConverterTool::on_comboBox_length1_currentIndexChanged(int index)
{
    convertUnits(index, ui->comboBox_length2->currentIndex(), lengthUnits, ui->doubleSpinBox_length1->value(), ui->doubleSpinBox_length2);
    qDebug() << lengthUnits.length();
}

void ConverterTool::on_comboBox_length2_currentIndexChanged(int index)
{
    convertUnits(ui->comboBox_length1->currentIndex(), index, lengthUnits, ui->doubleSpinBox_length1->value(), ui->doubleSpinBox_length2);
}

void ConverterTool::on_doubleSpinBox_length1_valueChanged(double arg1)
{
    convertUnits(ui->comboBox_length1->currentIndex(), ui->comboBox_length2->currentIndex(), lengthUnits, arg1, ui->doubleSpinBox_length2);
}

// Area Units
void ConverterTool::on_comboBox_area1_currentIndexChanged(int index)
{
    convertUnits(index, ui->comboBox_area2->currentIndex(), areaUnits, ui->doubleSpinBox_area1->value(), ui->doubleSpinBox_area2);
}

void ConverterTool::on_comboBox_area2_currentIndexChanged(int index)
{
    convertUnits(ui->comboBox_area1->currentIndex(), index, areaUnits, ui->doubleSpinBox_area1->value(), ui->doubleSpinBox_area2);
}

void ConverterTool::on_doubleSpinBox_area1_valueChanged(double arg1)
{
    convertUnits(ui->comboBox_area1->currentIndex(), ui->comboBox_area2->currentIndex(), areaUnits, arg1, ui->doubleSpinBox_area2);
}

// Volume Units
void ConverterTool::on_comboBox_volume1_currentIndexChanged(int index)
{
    convertUnits(index, ui->comboBox_volume2->currentIndex(), volumeUnits, ui->doubleSpinBox_volume1->value(), ui->doubleSpinBox_volume2);
}

void ConverterTool::on_comboBox_volume2_currentIndexChanged(int index)
{
    convertUnits(ui->comboBox_volume1->currentIndex(), index, volumeUnits, ui->doubleSpinBox_volume1->value(), ui->doubleSpinBox_volume2);
}

void ConverterTool::on_doubleSpinBox_volume1_valueChanged(double arg1)
{
    convertUnits(ui->comboBox_volume1->currentIndex(), ui->comboBox_volume2->currentIndex(), volumeUnits, arg1, ui->doubleSpinBox_volume2);
}

// Weight Units
void ConverterTool::on_comboBox_weight1_currentIndexChanged(int index)
{
    convertUnits(index, ui->comboBox_weight2->currentIndex(), weightUnits, ui->doubleSpinBox_weight1->value(), ui->doubleSpinBox_weight2);
}

void ConverterTool::on_comboBox_weight2_currentIndexChanged(int index)
{
    convertUnits(ui->comboBox_weight1->currentIndex(), index, weightUnits, ui->doubleSpinBox_weight1->value(), ui->doubleSpinBox_weight2);
}

void ConverterTool::on_doubleSpinBox_weight1_valueChanged(double arg1)
{
    convertUnits(ui->comboBox_weight1->currentIndex(), ui->comboBox_weight2->currentIndex(), weightUnits, arg1, ui->doubleSpinBox_weight2);
}

// Money Units
void ConverterTool::on_comboBox_money1_currentIndexChanged(int index)
{
    convertUnits(index, ui->comboBox_money2->currentIndex(), moneyUnits, ui->doubleSpinBox_money1->value(), ui->doubleSpinBox_money2);
}

void ConverterTool::on_comboBox_money2_currentIndexChanged(int index)
{
    convertUnits(ui->comboBox_money1->currentIndex(), index, moneyUnits, ui->doubleSpinBox_money1->value(), ui->doubleSpinBox_money2);
}

void ConverterTool::on_doubleSpinBox_money1_valueChanged(double arg1)
{
    convertUnits(ui->comboBox_money1->currentIndex(), ui->comboBox_money2->currentIndex(), moneyUnits, arg1, ui->doubleSpinBox_money2);
}
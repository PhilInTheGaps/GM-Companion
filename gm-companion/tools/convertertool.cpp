#include "convertertool.h"
#include "gm-companion/functions.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

ConverterTool::ConverterTool(QObject *parent) : QObject(parent)
{
    unitPaths.append({ ":/units/default.ini", QDir::homePath() + "/.gm-companion/units/custom.ini" });
    addAddonUnits();
    updateUnits();
}

// Find installed addons and if they have a units.ini file, add it to the unit
// paths
void ConverterTool::addAddonUnits()
{
    QString basePath = QDir::homePath() + "/.gm-companion/addons";

    for (QString addon : getFolders(basePath))
    {
        if (sManager.getIsAddonEnabled(addon))
        {
            if (QFile(basePath + "/" + addon + "/units.ini").exists())
            {
                unitPaths.append(basePath + "/" + addon + "/units.ini");
            }
        }
    }
}

// Read all the units from the .ini files
void ConverterTool::updateUnits()
{
    qDebug() << "Updating Units ...";

    l_lengthUnits.clear();
    l_lengthUnitsValues.clear();

    l_areaUnits.clear();
    l_areaUnitsValues.clear();

    l_volumeUnits.clear();
    l_volumeUnitsValues.clear();

    l_weightUnits.clear();
    l_weightUnitsValues.clear();

    l_moneyUnits.clear();
    l_moneyUnitsValues.clear();

    for (QString path : unitPaths)
    {
        QSettings settings(path, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");

        for (QString type : unitTypes)
        {
            int count = settings.beginReadArray(type);

            for (int i = 0; i < count; i++)
            {
                settings.setArrayIndex(i);

                if (type == "LengthUnits")
                {
                    l_lengthUnits.append(settings.value("name", "UNKNOWN UNIT").toString());
                    l_lengthUnitsValues.append(settings.value("refUnits", 0).toDouble());
                }
                else if (type == "AreaUnits")
                {
                    l_areaUnits.append(settings.value("name", "UNKNOWN UNIT").toString());
                    l_areaUnitsValues.append(settings.value("refUnits", 0).toDouble());
                }
                else if (type == "VolumeUnits")
                {
                    l_volumeUnits.append(settings.value("name", "UNKNOWN UNIT").toString());
                    l_volumeUnitsValues.append(settings.value("refUnits", 0).toDouble());
                }
                else if (type == "WeightUnits")
                {
                    l_weightUnits.append(settings.value("name", "UNKNOWN UNIT").toString());
                    l_weightUnitsValues.append(settings.value("refUnits", 0).toDouble());
                }
                else if (type == "MoneyUnits")
                {
                    l_moneyUnits.append(settings.value("name", "UNKNOWN UNIT").toString());
                    l_moneyUnitsValues.append(settings.value("refUnits", 0).toDouble());
                }
                else
                {
                    qDebug() << "Converter Tool: Unknown Unit Type" << type;
                }
            }

            settings.endArray();
        }
    }

    emit lengthUnitsChanged();
    emit areaUnitsChanged();
    emit volumeUnitsChanged();
    emit weightUnitsChanged();
    emit moneyUnitsChanged();
}

// Convert a string to a number, necessary, because the ui uses textfields
double ConverterTool::textToNumber(QString text)
{
    QString temp   = text.replace(",", ".");
    double  number = temp.toDouble();

    return number;
}

// Add custom unit
void ConverterTool::addUnit(QString name, QString refUnits, QString type)
{
    if (!name.isNull() && !refUnits.isNull() && !type.isNull())
    {
        QSettings settings(QDir::homePath() + "/.gm-companion/units/custom.ini", QSettings::IniFormat);
        settings.setIniCodec("UTF-8");

        QString arrayName;

        if (type == "Length")
        {
            arrayName = "LengthUnits";
        }
        else if (type == "Area")
        {
            arrayName = "AreaUnits";
        }
        else if (type == "Volume")
        {
            arrayName = "VolumeUnits";
        }
        else if (type == "Weight")
        {
            arrayName = "WeightUnits";
        }
        else if (type == "Money")
        {
            arrayName = "MoneyUnits";
        }
        else
        {
            qDebug() << "Converter Tool: Unknown Unit Type" << type;
        }

        if (!arrayName.isNull())
        {
            int index = settings.beginReadArray(arrayName);
            settings.endArray();

            settings.beginWriteArray(arrayName);
            settings.setArrayIndex(index);

            settings.setValue("name",     name);
            settings.setValue("refUnits", textToNumber(refUnits));

            settings.endArray();

            updateUnits();
        }
    }
}

// Return the name of the reference unit that belongs to the unit type.
// Example: returns "Meters" for "Length"
QString ConverterTool::refUnitName(QString unit)
{
    QString refUnit;

    if (unit == "Length")
    {
        refUnit = "Meters";
    }
    else if (unit == "Area")
    {
        refUnit = "Square Meters";
    }
    else if (unit == "Volume")
    {
        refUnit = "Cubic Meters";
    }
    else if (unit == "Weight")
    {
        refUnit = "Kilograms";
    }
    else if (unit == "Money")
    {
        refUnit = "Dollars";
    }
    else
    {
        qDebug() << "Converter Tool: Unknown Unit Type" << unit;
    }

    return refUnit;
}

QString ConverterTool::convertLength(QString unit1, QString unit2, QString value)
{
    qDebug() << "Converting Length ...";
    return QString::number(lengthUnitValue(unit1) / lengthUnitValue(unit2) * textToNumber(value), 'd', 6);
}

QString ConverterTool::convertArea(QString unit1, QString unit2, QString value)
{
    qDebug() << "Converting Area ...";
    return QString::number(areaUnitValue(unit1) / areaUnitValue(unit2) * textToNumber(value), 'd', 6);
}

QString ConverterTool::convertVolume(QString unit1, QString unit2, QString value)
{
    qDebug() << "Converting Volume ...";
    return QString::number(volumeUnitValue(unit1) / volumeUnitValue(unit2) * textToNumber(value), 'd', 6);
}

QString ConverterTool::convertWeight(QString unit1, QString unit2, QString value)
{
    qDebug() << "Converting Weight ...";
    return QString::number(weightUnitValue(unit1) / weightUnitValue(unit2) * textToNumber(value), 'd', 6);
}

QString ConverterTool::convertMoney(QString unit1, QString unit2, QString value)
{
    qDebug() << "Converting Money ...";
    return QString::number(moneyUnitValue(unit1) / moneyUnitValue(unit2) * textToNumber(value), 'd', 6);
}

QStringList ConverterTool::lengthUnits()
{
    return l_lengthUnits;
}

QStringList ConverterTool::areaUnits()
{
    return l_areaUnits;
}

QStringList ConverterTool::volumeUnits()
{
    return l_volumeUnits;
}

QStringList ConverterTool::weightUnits()
{
    return l_weightUnits;
}

QStringList ConverterTool::moneyUnits()
{
    return l_moneyUnits;
}

double ConverterTool::lengthUnitValue(QString unit)
{
    int i = l_lengthUnits.indexOf(unit);

    if (i > -1) return l_lengthUnitsValues.at(i);

    else return 0;
}

double ConverterTool::areaUnitValue(QString unit)
{
    int i = l_areaUnits.indexOf(unit);

    if (i > -1) return l_areaUnitsValues.at(i);

    else return 0;
}

double ConverterTool::volumeUnitValue(QString unit)
{
    int i = l_volumeUnits.indexOf(unit);

    if (i > -1) return l_volumeUnitsValues.at(i);

    else return 0;
}

double ConverterTool::weightUnitValue(QString unit)
{
    int i = l_weightUnits.indexOf(unit);

    if (i > -1) return l_weightUnitsValues.at(i);

    else return 0;
}

double ConverterTool::moneyUnitValue(QString unit)
{
    int i = l_moneyUnits.indexOf(unit);

    if (i > -1) return l_moneyUnitsValues.at(i);

    else return 0;
}

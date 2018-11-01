#include "convertertool.h"
#include "src/functions.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

ConverterTool::ConverterTool(QObject *parent) : QObject(parent)
{
    m_unitPaths.append({ ":/units/default.ini", QDir::homePath() + "/.gm-companion/units/custom.ini" });
    addAddonUnits();

    updateUnits();
}

// Find installed addons and if they have a units.ini file, add it to the unit
// paths
void ConverterTool::addAddonUnits()
{
    for (QString path : QStringList({ QDir::homePath() + "/.gm-companion/addons", ":/addons" }))
    {
        for (QString addon : getFolders(path))
        {
            if (sManager.getIsAddonEnabled(addon))
            {
                if (QFile(path + "/" + addon + "/units.ini").exists())
                {
                    m_unitPaths.append(path + "/" + addon + "/units.ini");
                }
            }
        }
    }
}

// Read all the units from the .ini files
void ConverterTool::updateUnits()
{
    qDebug() << "Updating Units ...";

    m_categories.clear();
    m_units.clear();
    m_unitValues.clear();
    bool firstPath = true;

    for (QString path : m_unitPaths)
    {
        QSettings settings(path, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");

        for (int type = 0; type < m_unitTypes.size(); type++)
        {
            int count = settings.beginReadArray(m_unitTypes[type]);

            if (firstPath)
            {
                QStringList tl1;
                QList<QStringList>    tl2;
                QList<QList<double> > tl3;
                m_categories.append(tl1);
                m_units.append(tl2);
                m_unitValues.append(tl3);
            }

            for (int i = 0; i < count; i++)
            {
                settings.setArrayIndex(i);

                QString cat = settings.value("category", tr("General")).toString();

                if (!m_categories[type].contains(cat))
                {
                    m_categories[type].append(cat);

                    QStringList l;
                    m_units[type].append(l);

                    QList<double> d;
                    m_unitValues[type].append(d);
                }

                int categoryIndex = m_categories[type].indexOf(cat);

                m_units[type][categoryIndex].append(settings.value("name", "UNKNOWN UNIT").toString());
                m_unitValues[type][categoryIndex].append(settings.value("refUnits", 0).toDouble());
            }

            settings.endArray();
        }

        firstPath = false;
    }

    emit unitsChanged();
    emit categoriesChanged();
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
            settings.setValue("category", "Custom");

            settings.endArray();

            updateUnits();
        }
    }
}

// Delete a (custom) unit
void ConverterTool::deleteUnit(int index)
{
    QSettings settings(QDir::homePath() + "/.gm-companion/units/custom.ini", QSettings::IniFormat);

    settings.setIniCodec("UTF-8");

    struct Unit
    {
        QString name;
        QString category;
        double  refUnits;
    };

    QList<Unit> tempList;

    // Get all units
    int size = settings.beginReadArray(m_unitTypes[m_typeIndex]);

    for (int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);

        Unit u;
        u.name     = settings.value("name").toString();
        u.category = settings.value("category").toString();
        u.refUnits = settings.value("refUnits").toDouble();

        tempList.push_back(u);
    }

    settings.endArray();

    // Remove selected unit
    if (index < tempList.size()) tempList.removeAt(index);

    // Write all other units back to file
    settings.beginWriteArray(m_unitTypes[m_typeIndex]);

    for (int i = 0; i < tempList.size(); i++)
    {
        settings.setArrayIndex(i);

        Unit u = tempList[i];

        settings.setValue("name",     u.name);
        settings.setValue("refUnits", u.refUnits);
        settings.setValue("category", "Custom");
    }

    settings.endArray();

    updateUnits();
}

// Return the name of the reference unit that belongs to the unit type.
// Example: returns "Meters" for "Length"
QString ConverterTool::refUnitName(QString unit)
{
    QString refUnit;

    if (unit == "Length")
    {
        refUnit = tr("Meters");
    }
    else if (unit == "Area")
    {
        refUnit = tr("Square Meters");
    }
    else if (unit == "Volume")
    {
        refUnit = tr("Cubic Meters");
    }
    else if (unit == "Weight")
    {
        refUnit = tr("Kilograms");
    }
    else if (unit == "Money")
    {
        refUnit = tr("Dollars");
    }
    else
    {
        qDebug() << "Converter Tool: Unknown Unit Type" << unit;
    }

    return refUnit;
}

void ConverterTool::setCurrentType(int index)
{
    m_typeIndex      = index;
    m_categoryIndex  = 0;
    m_categoryIndex1 = 0;
    m_categoryIndex2 = 0;
    m_unitIndex1     = 0;
    m_unitIndex2     = 0;
    convert();
    emit categoriesChanged();
    emit unitsChanged();
    emit unit1Changed();
    emit unit2Changed();
}

void ConverterTool::setCurrentCategory(int categoryIndex)
{
    m_categoryIndex = categoryIndex;
    emit unitsChanged();
}

void ConverterTool::setUnit(int index)
{
    if (m_isUnit1Next)
    {
        m_unitIndex1     = index;
        m_categoryIndex1 = m_categoryIndex;
        emit unit1Changed();
    }
    else
    {
        m_unitIndex2     = index;
        m_categoryIndex2 = m_categoryIndex;
        emit unit2Changed();
    }

    convert();
}

void ConverterTool::setUnit1ValueString(QString value)
{
    m_unit1ValueString = value;
    convert();
}

void ConverterTool::setIsUnit1Next(bool isNext)
{
    m_isUnit1Next = isNext;
    emit isUnit1NextChanged();
}

void ConverterTool::convert()
{
    m_unit2ValueString = QString::number(unitValue(m_unitIndex1, m_categoryIndex1) / unitValue(m_unitIndex2, m_categoryIndex2) * textToNumber(m_unit1ValueString), 'g', 4);

    //    qDebug() << unitValue(m_unitIndex1);
    //    qDebug() << unitValue(m_unitIndex2) << "\n";

    emit unit2ValueStringChanged();
}

double ConverterTool::unitValue(int index, int category)
{
    if (index > -1) return m_unitValues[m_typeIndex][category][index];

    else return 0;
}

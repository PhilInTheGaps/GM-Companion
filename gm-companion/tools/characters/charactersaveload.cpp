#include "charactersaveload.h"

#include <QDebug>

CharacterSaveLoad::CharacterSaveLoad(QObject *parent) : QObject(parent)
{
    sManager = new SettingsManager;
}

void CharacterSaveLoad::initializeSaving()
{
    emit saving();
}

void CharacterSaveLoad::initializeLoading()
{
    emit loading();
}

// Save Character
void CharacterSaveLoad::saveCharacter(QString character_name, QStringList table_names, QList<QVariant>table_values, QStringList text_groups, QStringList text_names, QStringList texts)
{
    QString path = sManager->getSetting(Setting::charactersPath) + "/" + character_name.replace(" ", "_") + ".character";

    QSettings settings(path, QSettings::IniFormat);

    qDebug() << "Saving Character" << character_name;

    qDebug() << "Saving Tables ...";

    // Save Tables
    for (int i = 0; i < table_names.size(); i++)
    {
        settings.beginGroup(table_names.at(i));

        for (int j = 0; j < table_values.at(i).toList().size(); j++)
        {
            settings.setValue(QString::number(j), table_values.at(i).toList().at(j).toList());
        }

        settings.endGroup();
    }

    qDebug() << "Saving Texts ...";

    // Save Texts
    for (int i = 0; i < text_groups.size(); i++)
    {
        if (text_groups.at(i) != "")
        {
            settings.beginGroup(text_groups.at(i));

            settings.setValue(text_names.at(i), texts.at(i));

            settings.endGroup();
        }
    }
}

// Save the content of an EditableTable
void CharacterSaveLoad::saveTable(QString character_name, QString name, QList<QVariant>values)
{
    QString path = sManager->getSetting(Setting::charactersPath) + "/" + character_name.replace(" ", "_") + ".character";

    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup(name);

    for (int i = 0; i < values.size(); i++)
    {
        settings.setValue(QString::number(i), values.at(i).toList());
    }

    settings.endGroup();
}

// Save a text
void CharacterSaveLoad::saveText(QString character_name, QString group, QString name, QString text)
{
    QString path = sManager->getSetting(Setting::charactersPath) + "/" + character_name.replace(" ", "_") + ".character";

    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup(group);

    settings.setValue(name, text);

    settings.endGroup();
}

// Return List of Table Values
QStringList CharacterSaveLoad::loadTable(QString character_name, QString name, int column)
{
    QString path = sManager->getSetting(Setting::charactersPath) + "/" + character_name.replace(" ", "_") + ".character";

    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup(name);

    QStringList list = settings.value(QString::number(column), {}).toStringList();

    settings.endGroup();

    return list;
}

// Return Saved String
QString CharacterSaveLoad::loadText(QString character_name, QString group, QString name)
{
    QString path = sManager->getSetting(Setting::charactersPath) + "/" + character_name.replace(" ", "_") + ".character";

    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup(group);

    QString text = settings.value(name, "").toString();

    settings.endGroup();

    return text;
}

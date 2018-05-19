#include "charactertool.h"

#include <QDebug>
#include <QSettings>
#include <QFile>

CharacterTool::CharacterTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Character Tool ...";

    sManager = new SettingsManager;
}

// Create new Character File
void CharacterTool::addCharacter(QString template_name, QString character_name, QString player_name)
{
    if (!template_name.isNull() && !character_name.isNull() && !player_name.isNull())
    {
        qDebug() << "Creating new Character file for" << character_name << "by" << player_name << "...";

        QString char_path = sManager->getSetting(Setting::charactersPath);
        QString file_name = character_name.replace(" ", "_") + ".character";

        int file_counter = 1;

        while (QFile(char_path + "/" + file_name).exists())
        {
            file_counter++;
            file_name = character_name.replace(" ", "_") + "_(" + QString::number(file_counter) + ").character";
        }

        QSettings settings(char_path + "/" + file_name, QSettings::IniFormat);

        settings.setValue("sheet_template", template_name);
        settings.setValue("player_name",    player_name);
        settings.setValue("character_name", character_name);

        settings.sync();

        emit charactersUpdated();
    }
}

// Delete a Character
void CharacterTool::deleteCharacter(QString character_name)
{
    QFile f(sManager->getSetting(Setting::charactersPath) + "/" + character_name.replace(" ", "_") + ".character");

    if (f.exists())
    {
        f.remove();
    }

    emit charactersUpdated();
}

QString CharacterTool::getSheetTemplate(QString character_name)
{
    QString char_path = sManager->getSetting(Setting::charactersPath);
    QString file_name = character_name.replace(" ", "_") + ".character";

    QSettings settings(char_path + "/" + file_name, QSettings::IniFormat);

    return settings.value("sheet_template", "Default").toString();
}

int CharacterTool::getSheetIndex(QString template_name)
{
    if (template_name == "Default")
    {
        return 1;
    }
    else if (template_name == "DSA5")
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

void CharacterTool::updateCharacterList()
{
    l_activeCharacters.clear();
    l_inactiveCharacters.clear();

    QStringList inactive = sManager->getInactiveCharacters();
    QString     path     = sManager->getSetting(Setting::charactersPath);

    // Active Characters
    for (QString f : getFiles(path))
    {
        if (f.contains(".character"))
        {
            QString character = f.replace(".character", "");
            character = character.replace("_", " ");

            if (inactive.contains(character))
            {
                l_inactiveCharacters.append(character);
            }
            else
            {
                l_activeCharacters.append(character);
            }
        }
    }

    emit charactersUpdated();
}

void CharacterTool::setCharacterActive(QString character_name)
{
    QStringList inactive = sManager->getInactiveCharacters();

    inactive.removeOne(character_name.replace(" ", "_"));

    sManager->setInactiveCharacters(inactive);

    emit charactersUpdated();
}

void CharacterTool::setCharacterInactive(QString character_name)
{
    QStringList inactive = sManager->getInactiveCharacters();

    inactive.append(character_name.replace(" ", "_"));

    sManager->setInactiveCharacters(inactive);

    emit charactersUpdated();
}

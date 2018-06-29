#include "charactertool.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QFile>

CharacterTool::CharacterTool(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Character Tool ...";
    m_settings = new QSettings(m_sManager.getSetting(Setting::charactersPath) + "/settings.ini", QSettings::IniFormat);
}

void CharacterTool::updateCharacterList()
{
    qDebug() << "Updating Character List ...";

    m_activeCharacters.clear();
    m_inactiveCharacters.clear();

    QString basePath         = m_sManager.getSetting(Setting::charactersPath);
    QStringList allInactives = m_settings->value("inactive").toStringList();

    for (QString s : getFolders(basePath))
    {
        if ((s != ".") && (s != ".."))
        {
            if (allInactives.contains(s))
            {
                m_inactiveCharacters.append(s);
            }
            else
            {
                m_activeCharacters.append(s);
            }
        }
    }

    emit charactersUpdated();
}

QStringList CharacterTool::getImages(QString character_name)
{
    QStringList list;

    QString path = m_sManager.getSetting(Setting::charactersPath) + "/" + character_name;

    for (QString s : getFiles(path))
    {
        if (s.endsWith(".jpg") || s.endsWith(".jpeg") || s.endsWith(".png"))
        {
            list.append(path + "/" + s);
        }
    }

    return list;
}

void CharacterTool::setCharacterActive(QString character_name)
{
    QStringList allInactives = m_settings->value("inactive").toStringList();

    allInactives.removeAll(character_name);
    m_inactiveCharacters.removeAll(character_name);
    m_activeCharacters.append(character_name);

    m_settings->setValue("inactive", allInactives);

    emit charactersUpdated();
}

void CharacterTool::setCharacterInactive(QString character_name)
{
    QStringList allInactives = m_settings->value("inactive").toStringList();

    allInactives.append(character_name);
    m_inactiveCharacters.append(character_name);
    m_activeCharacters.removeAll(character_name);

    m_settings->setValue("inactive", allInactives);

    emit charactersUpdated();
}

void CharacterTool::loadButtonTexts(QString character_name)
{
    QStringList list;
    QString     path = m_sManager.getSetting(Setting::charactersPath) + "/" + character_name;

    for (QString s : getFiles(path))
    {
        if (s.endsWith(".jpg") || s.endsWith(".jpeg") || s.endsWith(".png"))
        {
            list.append(s.left(s.indexOf(".")));
        }
    }

    m_buttonTexts = list;
}

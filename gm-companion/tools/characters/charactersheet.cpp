#include "charactersheet.h"

#include <QDebug>

CharacterSheet::CharacterSheet(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Character Sheet ...";
}

QString CharacterSheet::getTemplateName()
{
    return l_character_name;
}

void CharacterSheet::setTemplateName(QString name)
{
    l_template_name = name;
}

QString CharacterSheet::getPlayerName()
{
    return l_player_name;
}

void CharacterSheet::setPlayerName(QString name)
{
    l_player_name = name;
}

QString CharacterSheet::getCharacterName()
{
    return l_character_name;
}

void CharacterSheet::setCharacterName(QString name)
{
    l_character_name = name;
}

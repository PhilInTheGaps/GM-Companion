#include "character.h"
#include <QDebug>

Character::Character(QString name, QObject *parent)
    : QObject(parent), m_name(name)
{
    qDebug() << "Initializing new character:" << name << "...";
}

int Character::type() const
{
    if (m_files.size() < 1) return -1;

    if (m_files[0].name().endsWith("pdf")) return 1;

    if (m_files[0].name().endsWith("json")) return 2;

    return 0;
}

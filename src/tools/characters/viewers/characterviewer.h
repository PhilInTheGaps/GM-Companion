#ifndef CHARACTERVIEWER_H
#define CHARACTERVIEWER_H

#include "tools/characters/character.h"
#include <QObject>

class CharacterViewer : public QObject
{
    Q_OBJECT
public:
    explicit CharacterViewer(QObject *parent) : QObject(parent)
    {
    }

    virtual void setCharacter(Character *character)
    {
        m_currentCharacter = character;
    }
    virtual int pageIndex() const
    {
        return 0;
    }

    QStringList categories() const
    {
        return m_categories;
    }
    virtual void setCurrentCategory(qsizetype index)
    {
        m_categoryIndex = index;
    }

    qsizetype categoryIndex() const
    {
        return m_categoryIndex;
    }

signals:
    void categoryChanged();
    void categoriesChanged();

protected:
    QStringList m_categories;
    qsizetype m_categoryIndex = 0;

    Character *m_currentCharacter = nullptr;
};

#endif // CHARACTERVIEWER_H

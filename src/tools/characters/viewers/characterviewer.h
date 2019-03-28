#ifndef CHARACTERVIEWER_H
#define CHARACTERVIEWER_H

#include <QObject>
#include "src/tools/characters/character.h"

class CharacterViewer : public QObject
{
    Q_OBJECT
public:
    explicit CharacterViewer();

    virtual void setCharacter(Character* character) { m_currentCharacter = character; }
    virtual int pageIndex() const { return 0; }

    QStringList categories() const { return m_categories; }
    virtual void setCurrentCategory(int index) { m_categoryIndex = index; }

    int categoryIndex() const { return m_categoryIndex; }

signals:
    void categoryChanged();
    void categoriesChanged();

protected:
    QStringList m_categories;
    int m_categoryIndex = 0;

    Character* m_currentCharacter = nullptr;
};

#endif // CHARACTERVIEWER_H

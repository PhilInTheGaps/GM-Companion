#pragma once

#include "tools/characters/character.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class CharacterViewer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    using QObject::QObject;

    virtual void setCharacter(Character *character)
    {
        m_currentCharacter = character;
    }

    [[nodiscard]] virtual auto pageIndex() const -> int
    {
        return 0;
    }

    [[nodiscard]] auto categories() const -> QStringList
    {
        return m_categories;
    }

    virtual void setCurrentCategory(qsizetype index)
    {
        m_categoryIndex = index;
    }

    [[nodiscard]] auto categoryIndex() const -> qsizetype
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

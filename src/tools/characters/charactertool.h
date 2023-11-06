#pragma once

#include "character.h"
#include "common/abstracttool.h"
#include "viewers/characterimageviewer.h"
#include <QJSEngine>
#include <QList>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class CharacterTool : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    CharacterTool() = delete;
    explicit CharacterTool(QObject *parent = nullptr);

    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> CharacterTool *;

    Q_PROPERTY(CharacterImageViewer *imageViewer READ imageViewer CONSTANT)
    [[nodiscard]] auto imageViewer() -> CharacterImageViewer *
    {
        return &m_imageViewer;
    }

    Q_PROPERTY(QStringList characters READ characters NOTIFY charactersChanged)
    [[nodiscard]] auto characters() const -> QStringList;

    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    [[nodiscard]] auto categories() const -> QStringList
    {
        return m_currentViewer ? m_currentViewer->categories() : QStringList();
    }

    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY categoryChanged)
    [[nodiscard]] auto categoryIndex() const -> int // needs to be int for qml
    {
        return m_currentViewer ? static_cast<int>(m_currentViewer->categoryIndex()) : 0;
    }

    Q_PROPERTY(int pageIndex READ pageIndex NOTIFY pageIndexChanged)
    [[nodiscard]] auto pageIndex() const -> int
    {
        return m_currentViewer ? m_currentViewer->pageIndex() : 0;
    }

    Q_INVOKABLE void setCurrentCategory(int index);

    Q_PROPERTY(bool active READ active NOTIFY charactersChanged)
    [[nodiscard]] auto active() const -> bool
    {
        return m_active;
    }
    Q_INVOKABLE void toggleCharacterActive(qsizetype index);

    Q_INVOKABLE void setCurrentCharacter(qsizetype index);
    Q_INVOKABLE void displayActiveCharacters(bool active);

public slots:
    void loadData() override;

signals:
    void charactersChanged();
    void currentCharacterChanged();
    void categoryChanged();
    void categoriesChanged();
    void pageIndexChanged();

private:
    CharacterViewer *m_currentViewer = nullptr;
    CharacterImageViewer m_imageViewer;

    QList<Character *> m_characters;
    QStringList m_inactiveCharacters;
    Character *m_currentCharacter = nullptr;

    bool m_active = true;

    void loadCharacters();
    void receivedCharacterFolders(const QStringList &folders);
    void receivedCharacterFiles(const QStringList &files);

    void loadInactiveCharacters(const QByteArray &data);
    void saveInactiveCharacters() const;
    void updateCharacter() const;

    void convertSettingsFile(const QByteArray &data);
};

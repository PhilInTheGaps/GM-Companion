#ifndef CHARACTERTOOL_H
#define CHARACTERTOOL_H

#include "character.h"
#include "common/abstracttool.h"
#include "viewers/characterdsa5viewer.h"
#include "viewers/characterimageviewer.h"
#include <QList>
#include <QQmlApplicationEngine>

class CharacterTool : public AbstractTool
{
    Q_OBJECT
    Q_PROPERTY(QStringList characters READ characters NOTIFY charactersChanged)
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY categoryChanged)
    Q_PROPERTY(bool active READ active NOTIFY charactersChanged)
    Q_PROPERTY(int pageIndex READ pageIndex NOTIFY pageIndexChanged)

public:
    explicit CharacterTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    QStringList characters() const;
    QStringList categories() const
    {
        return m_currentViewer ? m_currentViewer->categories() : QStringList();
    }
    int categoryIndex() const
    {
        return m_currentViewer ? m_currentViewer->categoryIndex() : 0;
    }
    int pageIndex() const
    {
        return m_currentViewer ? m_currentViewer->pageIndex() : 0;
    }

    Q_INVOKABLE void setCurrentCategory(int index);

    bool active() const
    {
        return m_active;
    }
    Q_INVOKABLE void toggleCharacterActive(int index);

    Q_INVOKABLE void setCurrentCharacter(int index);
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
    CharacterImageViewer *m_imageViewer = nullptr;
    CharacterDSA5Viewer *m_dsa5Viewer = nullptr;

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

#endif // CHARACTERTOOL_H

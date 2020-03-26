#ifndef CHARACTERTOOL_H
#define CHARACTERTOOL_H

#include <QObject>
#include <QList>
#include <QQmlApplicationEngine>
#include "character.h"
#include "managers/filemanager.h"
#include "viewers/characterimageviewer.h"
#include "viewers/characterdsa5viewer.h"

class CharacterTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList characters READ characters NOTIFY charactersChanged)
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY categoryChanged)
    Q_PROPERTY(bool active READ active NOTIFY charactersChanged)
    Q_PROPERTY(int pageIndex READ pageIndex NOTIFY pageIndexChanged)

public:
    explicit CharacterTool(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent = nullptr);

    CharacterImageViewer *getImageViewer() const { return m_imageViewer; }
    CharacterDSA5Viewer *getDSA5Viewer() const { return m_dsa5Viewer; }

    QStringList characters() const { return m_characterNames; }
    QStringList categories() const { if (m_currentViewer) return m_currentViewer->categories(); else return {}; }
    int categoryIndex() const { if (m_currentViewer) return m_currentViewer->categoryIndex(); else return 0; }
    int pageIndex() const { if (m_currentViewer) return m_currentViewer->pageIndex(); else return 0; }

    Q_INVOKABLE void setCurrentCategory(int index);

    bool active() const { return m_active; }
    Q_INVOKABLE void toggleCharacterActive(int index);

    Q_INVOKABLE void setCurrentCharacter(int index);
    Q_INVOKABLE void displayActiveCharacters(bool active);

signals:
    void charactersChanged();
    void currentCharacterChanged();
    void categoryChanged();
    void categoriesChanged();
    void pageIndexChanged();

private:
    FileManager *fileManager = nullptr;

    CharacterViewer *m_currentViewer = nullptr;
    CharacterImageViewer *m_imageViewer = nullptr;
    CharacterDSA5Viewer *m_dsa5Viewer = nullptr;

    QStringList m_characterNames, m_inactiveCharacters;
    QList<Character*> m_characters;
    QList<Character*> m_visibleCharacters;
    Character *m_currentCharacter = nullptr;

    bool m_active = true;

    void loadInactiveCharacters();
    void saveInactiveCharacters();
    void updateCharacter();

private slots:
    void receivedCharacters(QList<Character*> characters);
    void receivedFiles(int reqId, QList<CharacterFile> files);
};


#endif // CHARACTERTOOL_H


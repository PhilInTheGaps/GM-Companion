#ifndef CHARACTERTOOL_H
#define CHARACTERTOOL_H

#include <QObject>
#include <QList>
#include <QQmlApplicationEngine>
#include "character.h"
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
    explicit CharacterTool(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    CharacterImageViewer *getImageViewer() const { return m_imageViewer; }
    CharacterDSA5Viewer *getDSA5Viewer() const { return m_dsa5Viewer; }

    QStringList characters() const;
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
    CharacterViewer *m_currentViewer = nullptr;
    CharacterImageViewer *m_imageViewer = nullptr;
    CharacterDSA5Viewer *m_dsa5Viewer = nullptr;

    QList<Character*> m_characters;
    QStringList m_inactiveCharacters;
    Character *m_currentCharacter = nullptr;

    bool m_active = true;
    int m_loadInactiveRequestId = -1;
    int m_loadCharacterFilesRequestId = -1;
    int m_loadCharacterFoldersRequestId = -1;
    int m_convertFileRequestId = -1;

    void loadCharacters();
    void receivedCharacterFolders(QStringList folders);
    void receivedCharacterFiles(QStringList files);

    void loadInactiveCharacters(QByteArray data);
    void saveInactiveCharacters();
    void updateCharacter();

    void convertSettingsFile(QByteArray data);

private slots:
    void receivedFile(int id, QByteArray data);
    void receivedFileList(int id, QStringList files);
};


#endif // CHARACTERTOOL_H


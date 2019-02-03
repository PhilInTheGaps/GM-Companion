#ifndef CHARACTERFILEMANAGER_H
#define CHARACTERFILEMANAGER_H

#include <QObject>
#include <QList>
#include <QDir>
#include "src/cloud/googledrive.h"
#include "src/settings/settingsmanager.h"
#include "src/tools/characters/character.h"

class CharacterFileManager : public QObject
{
    Q_OBJECT
public:
    explicit CharacterFileManager(GoogleDrive *google, QObject *parent = nullptr);

    void findCharacters(int mode);
    void findFiles(Character* character);

signals:
    void receivedCharacters(QList<Character*> characters);
    void receivedFiles(int reqId, QList<CharacterFile> files);

private:
    SettingsManager sManager;
    GoogleDrive *googleDrive = nullptr;

    int m_mode = 0;
    QList<Character*> m_characters;
    QList<int> m_requestIDs;

    void findLocalCharacters();
};

#endif // CHARACTERFILEMANAGER_H

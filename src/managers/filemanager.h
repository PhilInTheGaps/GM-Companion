#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QList>
#include <QUrl>

#include "cloud/googledrive.h"
#include "settings/settingsmanager.h"

#include "tools/audio/audiosaveload.h"
#include "file_managers/mapsfilemanager.h"
#include "file_managers/shopfilemanager.h"
#include "file_managers/characterfilemanager.h"

enum CloudMode
{
    LOCAL,
    GOOGLE_DRIVE,
    GENERIC_CLOUD
};

class FileManager : public QObject
{
    Q_OBJECT

public:
    FileManager();
    ~FileManager();
    void run();

    int getModeInt() const { return static_cast<int>(m_mode); }

    GoogleDrive* getGoogleDrive() const { return google; }

    AudioSaveLoad* getAudioSaveLoad() const { return audioSaveLoad; }
    MapsFileManager* getMapsFileManger() const { return mapsFileManager; }
    ShopFileManager* getShopFileManager() const { return shopFileManager; }
    CharacterFileManager* getCharacterFileManager() const { return characterFileManager; }

private:
    GoogleDrive *google;

    AudioSaveLoad *audioSaveLoad;
    MapsFileManager *mapsFileManager;
    ShopFileManager *shopFileManager;
    CharacterFileManager *characterFileManager;

    CloudMode m_mode;

    QString m_origPath;
    QStringList m_filesToBeDeleted;
    QStringList m_foldersToBeDeleted;

    bool removeDir(const QString & dirName);
    void updateMode();
};

#endif // FILEMANAGER_H

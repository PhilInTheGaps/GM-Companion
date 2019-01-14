#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QList>
#include <QUrl>

#include "src/cloud/googledrive.h"
#include "src/settings/settingsmanager.h"

#include "file_managers/audiofilemanager.h"
#include "file_managers/mapsfilemanager.h"

enum CloudMode
{
    LOCAL,
    GOOGLE_DRIVE
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
    AudioFileManager* getAudioFileManager() const { return audioFileManager; }
    MapsFileManager* getMapsFileManger() const { return mapsFileManager; }

private:
    GoogleDrive *google;
    SettingsManager *sManager;

    AudioFileManager *audioFileManager;
    MapsFileManager *mapsFileManager;

    CloudMode m_mode;

    QString m_origPath;
    QStringList m_filesToBeDeleted;
    QStringList m_foldersToBeDeleted;

    bool removeDir(const QString & dirName);
    void updateMode();
};

#endif // FILEMANAGER_H

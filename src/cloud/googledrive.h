#ifndef GOOGLEDRIVE_H
#define GOOGLEDRIVE_H

#define MAX_FILE_QUERY_COUNT 10

#include <QObject>
#include <QUrl>
#include <QByteArray>
#include <QNetworkAccessManager>
#include "o2google.h"
#include "settings/settingsmanager.h"
#include "foldertree.h"

struct GoogleFile
{
    QString fileName;
    QString id;
    QString parent;
    QString weblink;
};

struct FilePath
{
    QString fileName;
    QString fullPath;
    QString webConentLink;
    QString parentID;
    int internalParentID = -1;
};

class GoogleDrive : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool linked READ linked NOTIFY linkStatusChanged)

public:
    explicit GoogleDrive(QObject *parent = nullptr);

    Q_INVOKABLE void link();
    Q_INVOKABLE void unlink() { drive->unlink(); }
    bool linked() const { return drive->linked(); }

    Q_INVOKABLE void setClientID(QString id) { m_id = id; SettingsManager::setSetting("googleID", id, "Google"); }
    Q_INVOKABLE void setClientSecret(QString secret) { m_secret = secret; SettingsManager::setSetting("googleSecret", secret, "Google"); }

    int getFolderContent(QString id);
    int getFile(QString id);
    int getFileUrls(QString parentId);
    void getFileUrls(QStringList paths, int mode, QStringList args = {});
    int getSubfolderIDs(QString parentId);

private:
    O2Google *drive;
    FolderTree *folderTree;
    QNetworkAccessManager *m_manager;

    QList<Folder*> addPathsToTree(QStringList paths, DefaultFolder root);
    void findMissingFolderIDs(Folder *parent, int mode, QStringList args);
    void getFolderIDs(QStringList folderNames, QString parentID, int mode, QStringList args);
    void setFolderID(QString name, QString id, QString parentID, int mode, QStringList args);

    QString m_id;
    QString m_secret;

    int m_requestCount = 0;

    // Generic Files
    QList<FilePath> m_filePaths;
    QList<Folder*> m_fileFolders;

    // Music
    QList<FilePath> m_songPaths;
    QList<QUrl> m_songGetUrls;
    QList<Folder*> m_songFolders;

    // Sounds
    QList<FilePath> m_soundPaths;
    QList<Folder*> m_soundFolders;

    // Resources
    QList<FilePath> m_iconPaths;
    QList<Folder*> m_iconFolders;

    void fetchWebContentURLs(int index, int mode, QStringList args);

signals:
    void linkStatusChanged(bool linked);
    void openWebsite(QUrl url);
    void receivedFileUrls(QList<QUrl> urls, int mode, QStringList args);
    void receivedFileUrls(QList<FilePath> *files, int mode);
    void receivedFileUrls(int requestID, QList<GoogleFile>);
    void receivedFolderContent(int requestID, QList<GoogleFile>);
    void receivedSubfolderIDs(int requestID, QList<GoogleFile>);
    void receivedFile(int requestID, QByteArray data);
    void receivedFileLink(int requestID, QString link);
    void allFolderIDsFound();
};

#endif // GOOGLEDRIVE_H

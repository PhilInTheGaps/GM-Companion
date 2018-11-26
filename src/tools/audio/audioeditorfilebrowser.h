#ifndef AUDIOEDITORFILEBROWSER_H
#define AUDIOEDITORFILEBROWSER_H

#include <QObject>
#include "src/settings/settingsmanager.h"

class AudioEditorFileBrowser : public QObject
{
    Q_OBJECT
public:
    explicit AudioEditorFileBrowser(QObject *parent = nullptr);

    Q_INVOKABLE void setType(int type);
    Q_INVOKABLE int getType() const { return m_type; }

    Q_INVOKABLE void updateFolders();
    Q_INVOKABLE void updateFiles();

    Q_INVOKABLE void setCurrentFolder(QString folder);
    Q_INVOKABLE QStringList getFolderList() const { return m_folders; }
    Q_INVOKABLE QStringList getFolderPaths() const { return m_folderPaths; }
    Q_INVOKABLE void folderBack();

    Q_INVOKABLE QStringList getFileList() const { return m_files; }
    Q_INVOKABLE QStringList getFilePaths() const { return m_filePaths; }

signals:
    void foldersChanged();
    void filesChanged();

private:
    SettingsManager sManager;

    QString m_basePath;
    QStringList m_relativeFolders;
    QString m_currentPath();

    int m_type = 0;

    QStringList m_folders;
    QStringList m_folderPaths;

    QStringList m_files;
    QStringList m_filePaths;
};

#endif // AUDIOEDITORFILEBROWSER_H

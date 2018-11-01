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
    Q_INVOKABLE int getType();

    Q_INVOKABLE void updateFolders();
    Q_INVOKABLE void updateFiles();

    Q_INVOKABLE void setCurrentFolder(QString folder);
    Q_INVOKABLE QStringList getFolderList();
    Q_INVOKABLE QStringList getFolderPaths();
    Q_INVOKABLE void folderBack();

    Q_INVOKABLE QStringList getFileList();
    Q_INVOKABLE QStringList getFilePaths();

signals:
    void foldersChanged();
    void filesChanged();

private:
    SettingsManager sManager;

    QString l_basePath;
    QStringList l_relativeFolders;
    QString l_currentPath();

    int l_type = 0;

    QStringList l_folders;
    QStringList l_folderPaths;

    QStringList l_files;
    QStringList l_filePaths;
};

#endif // AUDIOEDITORFILEBROWSER_H

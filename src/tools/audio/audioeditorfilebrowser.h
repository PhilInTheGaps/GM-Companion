#ifndef AUDIOEDITORFILEBROWSER_H
#define AUDIOEDITORFILEBROWSER_H

#include <QObject>
#include "src/settings/settingsmanager.h"

class AudioEditorFileBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QStringList folderNames READ folderNames NOTIFY foldersChanged)
    Q_PROPERTY(QStringList fileNames READ fileNames NOTIFY filesChanged)
    Q_PROPERTY(QStringList filePaths READ filePaths NOTIFY filesChanged)

public:
    explicit AudioEditorFileBrowser(QObject *parent = nullptr);

    void setType(int type);
    int type() const { return m_type; }

    Q_INVOKABLE void setCurrentFolder(QString folder);
    Q_INVOKABLE void folderBack();
    Q_INVOKABLE void home();
    Q_INVOKABLE void addAllFiles();

    QStringList folderNames() const { return m_folders; }
    QStringList fileNames() const { return m_files; }
    QStringList filePaths() const { return m_filePaths; }

signals:
    void foldersChanged();
    void filesChanged();
    void typeChanged();
    void addFiles(QStringList files);

private:
    SettingsManager sManager;

    void updateFolders();
    void updateFiles();

    QString m_basePath;
    QStringList m_relativeFolders;
    QString currentPath();

    int m_type = 0;

    QStringList m_folders;

    QStringList m_files;
    QStringList m_filePaths;
};

#endif // AUDIOEDITORFILEBROWSER_H

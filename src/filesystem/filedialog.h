#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QQmlContext>
#include <QObject>
#include "utils/fileutils.h"

#include "thirdparty/propertyhelper/PropertyHelper.h"

class FileObject : public QObject
{
    Q_OBJECT
public:
    FileObject(const QString &name, bool isFolder, QObject *parent = nullptr);

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; emit nameChanged(); }

    Q_PROPERTY(bool isFolder READ isFolder WRITE setIsFolder NOTIFY isFolderChanged)
    bool isFolder() const { return m_isFolder; }
    void setIsFolder(const bool& isFolder) { m_isFolder = isFolder; emit isFolderChanged(); }

signals:
    void nameChanged();
    void isFolderChanged();

private:
    QString m_name;
    bool m_isFolder;
};

class FileDialog : public QObject
{
    Q_OBJECT
public:
    explicit FileDialog(QObject *parent = nullptr);

    AUTO_PROPERTY(QString, test)

    Q_PROPERTY(QString currentDir READ currentDir WRITE setCurrentDir NOTIFY currentDirChanged)
    QString currentDir() const { return FileUtils::dirFromFolders(m_currentDir); }
    void setCurrentDir(const QString& dir);

    Q_PROPERTY(QList<QObject*> files READ files WRITE setFiles NOTIFY filesChanged)
    QList<QObject*> files() const { return m_files; }
    void setFiles(QList<QObject*> files) { m_files = files; emit filesChanged(); }

    Q_PROPERTY(bool canForward READ canForward NOTIFY canForwardChanged)
    bool canForward() const { return m_forwardFolders.length() > 0; }

    Q_PROPERTY(bool folderMode READ folderMode WRITE setFolderMode NOTIFY folderModeChanged)
    bool folderMode() const { return m_folderMode; }
    void setFolderMode(const bool& folderMode) { m_folderMode = folderMode; emit folderModeChanged(); }

    Q_INVOKABLE QString getSelected(int index) const;

public slots:
    void enterFolder(int index);
    void forward();
    void clearForward();
    void back();

signals:
    void currentDirChanged();
    void filesChanged();
    void canForwardChanged();
    void folderModeChanged();

private:
    QStringList m_currentDir;
    QStringList m_forwardFolders;
    QList<QObject*> m_files;

    int m_foldersRequestId = -1;
    int m_filesRequestId = -1;
    bool m_folderMode = false;

    void updateFileList();

private slots:
    void onFileListReceived(const int& requestId, const QStringList& files);

};

#endif // FILEDIALOG_H

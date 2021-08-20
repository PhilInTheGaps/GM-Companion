#pragma once

#include <QObject>
#include <QFuture>
#include "utils/fileutils.h"
#include "file.h"

#include "thirdparty/propertyhelper/PropertyHelper.h"

namespace Files {

class FileDialog : public QObject
{
    Q_OBJECT
public:
    explicit FileDialog(QObject *parent = nullptr);

    Q_PROPERTY(QString currentDir READ currentDir WRITE setCurrentDir NOTIFY currentDirChanged)
    QString currentDir() const { return FileUtils::dirFromFolders(m_currentDir); }
    void setCurrentDir(const QString& dir);

    Q_PROPERTY(bool canGoForward READ canGoForward NOTIFY canGoForwardChanged)
    bool canGoForward() const { return m_forwardFolders.length() > 0; }

    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged)
    bool canGoBack() const { return m_currentDir.length() > 1; }

    AUTO_PROPERTY(QList<QObject*>, entries)
    AUTO_PROPERTY(bool, folderMode)
    AUTO_PROPERTY(bool, isLoading)

    Q_INVOKABLE QString getSelected(int index) const;

public slots:
    void enterFolder(int index);
    void forward();
    void back();

signals:
    void currentDirChanged(const QString& dir);
    void canGoForwardChanged(bool canGo);
    void canGoBackChanged(bool canGo);

private:
    QStringList m_currentDir;
    QStringList m_forwardFolders;

    QFuture<FileListResult*> m_currentFuture;

    void updateFileList();
    void clearFileList();
    void stopCurrentRequest();
    void clearForward();

    void onFileListReceived(FileListResult *result);

private slots:
    void onCurrentDirChanged(const QString& dir);
};

}

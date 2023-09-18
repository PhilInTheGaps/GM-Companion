#pragma once

#include "results/filelistresult.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QFuture>
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Files
{

class FileObject;

class FileDialogBackend : public QObject
{
    Q_OBJECT
    READ_LIST_PROPERTY(Files::FileObject, entries)
    AUTO_PROPERTY_VAL2(bool, folderMode, false)
    AUTO_PROPERTY_VAL2(bool, isLoading, false)
    QML_ELEMENT
public:
    explicit FileDialogBackend(QObject *parent = nullptr);

    Q_PROPERTY(QString currentDir READ currentDir WRITE setCurrentDir NOTIFY currentDirChanged)
    [[nodiscard]] auto currentDir() const -> QString;
    void setCurrentDir(const QString &dir);

    Q_PROPERTY(bool canGoForward READ canGoForward NOTIFY canGoForwardChanged)
    [[nodiscard]] auto canGoForward() const -> bool;

    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged)
    [[nodiscard]] auto canGoBack() const -> bool;

    Q_INVOKABLE QString getSelected(int index) const;

public slots:
    void enterFolder(int index);
    void forward();
    void back();
    void createFolder(const QString &folderName);

signals:
    void currentDirChanged(const QString &dir);
    void canGoForwardChanged(bool canGo);
    void canGoBackChanged(bool canGo);

private:
    QStringList m_currentDir;
    QStringList m_forwardFolders;

    QFuture<FileListResult *> m_currentFuture;

    void updateFileList();
    void clearFileList();
    void stopCurrentRequest();
    void clearForward();

    void onFileListReceived(FileListResult *result);

private slots:
    void onCurrentDirChanged(const QString &dir);
};

} // namespace Files

#pragma once

#include "fileresult.h"
#include <QStringList>
#include "thirdparty/propertyhelper/PropertyHelper.h"

namespace Files {

/// Class that either contains a list of paths to files and/or folders
/// or an error message.
class FileListResult : public FileResult
{
    Q_OBJECT

public:
    explicit FileListResult(const QString& path, const QString& errorMessage, QObject *parent = nullptr)
        : FileResult(errorMessage, parent), a_path(path) {}

    explicit FileListResult(const QString& path, const QStringList& folders, const QStringList& files, QObject *parent = nullptr)
        : FileResult(true, QByteArray(), parent), a_folders(folders), a_files(files), a_path(path) {}

    QStringList filesFull(const QString &wildcard = "") const;
    QStringList foldersFull(const QString &wildcard = "") const;

    READONLY_PROPERTY(QStringList, folders)
    READONLY_PROPERTY(QStringList, files)
    READONLY_PROPERTY(QString, path)

private:
    QStringList getFullyQualified(const QStringList &list) const;
    static QStringList getMatchingEntries(const QStringList &list, const QString &wildcard);
};

}

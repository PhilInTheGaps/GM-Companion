#pragma once

#include "fileresult.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QStringList>

using namespace Qt::Literals::StringLiterals;
namespace Files
{

/// Class that either contains a list of paths to files and/or folders
/// or an error message.
class FileListResult : public FileResult
{
    Q_OBJECT

public:
    explicit FileListResult(const QString &path, const QString &errorMessage, QObject *parent = nullptr)
        : FileResult(errorMessage, parent), a_path(path)
    {
    }

    explicit FileListResult(const QString &path, const QStringList &folders, const QStringList &files,
                            QObject *parent = nullptr)
        : FileResult(true, QByteArray(), parent), a_folders(folders), a_files(files), a_path(path)
    {
    }

    QStringList filesFull(const QString &wildcard = u""_s) const;
    QStringList foldersFull(const QString &wildcard = u""_s) const;

    READONLY_PROPERTY(QStringList, folders)
    READONLY_PROPERTY(QStringList, files)
    READONLY_PROPERTY(QString, path)

private:
    QStringList getFullyQualified(const QStringList &list) const;
    static QStringList getMatchingEntries(const QStringList &list, const QString &wildcard);
};

} // namespace Files

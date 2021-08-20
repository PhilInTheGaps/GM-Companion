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
    explicit FileListResult(const QString& errorMessage, QObject *parent = nullptr)
        : FileResult(errorMessage, parent) {}

    explicit FileListResult(const QStringList& folders, const QStringList& files, QObject *parent = nullptr)
        : FileResult(true, QByteArray(), parent), a_folders(folders), a_files(files) {}

    READONLY_PROPERTY(QStringList, folders)
    READONLY_PROPERTY(QStringList, files)
};

}

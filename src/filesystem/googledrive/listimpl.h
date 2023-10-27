#pragma once

#include <QFuture>
#include <QString>
#include <QStringList>

namespace Files
{

class FileAccessGoogleDrive;
class FileListResult;

namespace GoogleDrive
{

class ListImpl
{
public:
    static auto list(const QString &path, bool files, bool folders, FileAccessGoogleDrive *fa)
        -> QFuture<FileListResult>;

private:
    struct State
    {
        FileAccessGoogleDrive *fa;
        QString path;
        QString query;
        QString pageToken;
        bool includeFiles;
        bool includeFolders;
        QStringList files;
        QStringList folders;
    };

    static auto list(State &&state) -> QFuture<FileListResult>;
    static auto makeListQuery(const QString &id, const State &state) -> QString;
    static auto onFileIdReceived(State &&state) -> QFuture<FileListResult>;
};

} // namespace GoogleDrive
} // namespace Files

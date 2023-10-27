#pragma once

#include <QString>

namespace Files
{

class FileAccessGoogleDrive;
class FileResult;

namespace GoogleDrive
{

/// Delete a file/folder
/// 1. Get file/folder ID
/// 2. Send DELETE request
class DeleteImpl
{
public:
    static auto deleteFile(const QString &path, FileAccessGoogleDrive *fa) -> QFuture<FileResult>;

private:
    struct State
    {
        FileAccessGoogleDrive *fa;
        QString path;
        QString fileId;
    };

    static auto deleteFile(State &&state) -> QFuture<FileResult>;
    static auto onFileIdReceived(State &&state) -> QFuture<FileResult>;
};

} // namespace GoogleDrive
} // namespace Files

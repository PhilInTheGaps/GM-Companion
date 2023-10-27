#pragma once

#include <QFuture>
#include <QString>

namespace Files
{

class FileAccessGoogleDrive;
class FileCheckResult;

namespace GoogleDrive
{

class CheckImpl
{
public:
    static auto check(const QString &path, FileAccessGoogleDrive *fa) -> QFuture<FileCheckResult>;

private:
    struct State
    {
        FileAccessGoogleDrive *fa;
        QString path;
    };

    static auto check(State &&state) -> QFuture<FileCheckResult>;
    //    static auto onFileIdReceived(State &&state) -> QFuture<FileCheckResult>;
};

} // namespace GoogleDrive
} // namespace Files

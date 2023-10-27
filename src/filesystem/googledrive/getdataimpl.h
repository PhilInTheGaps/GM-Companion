#pragma once

#include <QFuture>
#include <QString>

namespace Files
{

class FileAccessGoogleDrive;
class FileDataResult;

namespace GoogleDrive
{

class GetDataImpl
{
public:
    static auto getData(const QString &path, FileAccessGoogleDrive *fa) -> QFuture<FileDataResult>;

private:
    struct State
    {
        FileAccessGoogleDrive *fa;
        QString path;
        QString fileId;
    };

    static auto getData(State &&state) -> QFuture<FileDataResult>;
    static auto onFileIdReceived(State &&state) -> QFuture<FileDataResult>;
};

} // namespace GoogleDrive
} // namespace Files

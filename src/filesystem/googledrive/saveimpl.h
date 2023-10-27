#pragma once

#include <QByteArray>
#include <QFuture>
#include <QString>

namespace Files
{

class FileAccessGoogleDrive;
class FileResult;

namespace GoogleDrive
{

class SaveImpl
{
public:
    static auto save(const QString &path, const QByteArray &data, FileAccessGoogleDrive *fa) -> QFuture<FileResult>;

private:
    struct State
    {
        FileAccessGoogleDrive *fa;
        QString path;
        QByteArray data;
        QString fileId;
        QByteArray uploadUrl;
    };

    static auto save(State &&state) -> QFuture<FileResult>;
    static auto onFileIdReceived(State &&state) -> QFuture<FileResult>;

    static auto onEndpointReceived(State &&state) -> QFuture<FileResult>;

    static auto createNewFile(State &&state) -> QFuture<FileResult>;
};

} // namespace GoogleDrive
} // namespace Files

#pragma once

#include <QPixmap>
#include <QString>
#include <QFuture>

namespace Files {
class FileDataResult;
}

class FileImageLoader
{
public:
    static auto loadImageAsync(const QString &path) -> QFuture<QPixmap>;

private:
    static auto loadFromFileResult(const QString &path, Files::FileDataResult *result) -> QPixmap;
};

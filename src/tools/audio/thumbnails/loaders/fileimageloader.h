#pragma once

#include <QFuture>
#include <QPixmap>
#include <QString>

namespace Files
{
class FileDataResult;
}

class FileImageLoader
{
public:
    static auto loadImageAsync(const QString &path) -> QFuture<QPixmap>;

private:
    static auto loadFromFileResult(const QString &path, std::shared_ptr<Files::FileDataResult> result) -> QPixmap;
};

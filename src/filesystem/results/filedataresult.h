#pragma once

#include "fileresult.h"
#include <QByteArray>

namespace Files
{

class FileDataResult : public FileResult
{
public:
    explicit FileDataResult(const QString &errorMessage) : FileResult(errorMessage)
    {
    }

    explicit FileDataResult(const QByteArray &data) : FileResult(true, QByteArray()), m_data(data)
    {
    }

    static auto fromNetworkReply(RestNetworkReply *reply) -> std::shared_ptr<FileDataResult>;

    [[nodiscard]] auto data() const -> const QByteArray &
    {
        return m_data;
    }

private:
    const QByteArray m_data;
};

} // namespace Files

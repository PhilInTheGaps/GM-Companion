#pragma once

#include <QObject>
#include <memory>

class RestNetworkReply;

namespace Files
{

class FileResult
{
public:
    explicit FileResult(bool success) : FileResult(success, QLatin1String())
    {
    }

    explicit FileResult(bool success, const QString &errorMessage) : m_success(success), m_errorMessage(errorMessage)
    {
    }

    explicit FileResult(const QString &errorMessage) : FileResult(false, errorMessage)
    {
    }

    static auto fromNetworkReply(RestNetworkReply *reply) -> std::shared_ptr<FileResult>;

    [[nodiscard]] auto success() const -> bool
    {
        return m_success;
    }

    [[nodiscard]] auto errorMessage() const -> QString
    {
        return m_errorMessage;
    }

protected:
    const bool m_success;
    const QString m_errorMessage;
};

} // namespace Files

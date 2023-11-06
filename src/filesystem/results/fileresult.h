#pragma once

#include <QObject>
#include <memory>

namespace Services
{
class RestReply;
}

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

    explicit FileResult(QString &&errorMessage) : m_success(false), m_errorMessage(std::move(errorMessage))
    {
    }

    static auto fromRestReply(const Services::RestReply &reply) -> FileResult;

    [[nodiscard]] auto success() const -> bool
    {
        return m_success;
    }

    [[nodiscard]] auto errorMessage() const -> QString
    {
        return m_errorMessage;
    }

protected:
    bool m_success;
    QString m_errorMessage;
};

} // namespace Files

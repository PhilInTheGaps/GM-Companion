#pragma once

#include "fileresult.h"

namespace Services
{
class RestReply;
}

namespace Files
{

class FileCheckResult : public FileResult
{
public:
    explicit FileCheckResult(const QString &path, const QString &errorMessage)
        : FileResult(errorMessage), m_path(path), m_exists(false)
    {
    }

    explicit FileCheckResult(const QString &path, bool exists)
        : FileResult(true, QByteArray()), m_path(path), m_exists(exists)
    {
    }

    static auto fromRestReply(const Services::RestReply &reply, const QString &path) -> FileCheckResult;

    [[nodiscard]] auto path() const -> const QString &
    {
        return m_path;
    }
    [[nodiscard]] auto exists() const -> bool
    {
        return m_exists;
    }

private:
    QString m_path;
    bool m_exists;
};

} // namespace Files

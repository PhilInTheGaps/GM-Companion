#pragma once

#include "fileresult.h"

class RestNetworkReply;

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

    static auto fromNetworkReply(RestNetworkReply *reply, const QString &path) -> std::unique_ptr<FileCheckResult>;

    [[nodiscard]] auto path() const -> const QString &
    {
        return m_path;
    }
    [[nodiscard]] auto exists() const -> bool
    {
        return m_exists;
    }

private:
    const QString m_path;
    const bool m_exists;
};

} // namespace Files

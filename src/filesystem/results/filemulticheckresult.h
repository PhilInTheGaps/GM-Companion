#pragma once

#include "filecheckresult.h"
#include "fileresult.h"
#include <QList>

namespace Files
{

class FileMultiCheckResult : public FileResult
{
public:
    explicit FileMultiCheckResult(std::vector<FileCheckResult> &&results)
        : FileResult(true), m_results(std::move(results))
    {
    }

    explicit FileMultiCheckResult(bool success, const QString &errorMessage) : FileResult(success, errorMessage)
    {
    }

    explicit FileMultiCheckResult(bool success) : FileResult(success)
    {
    }

    void add(FileCheckResult &&result)
    {
        m_results.push_back(result);
    }

    [[nodiscard]] auto results() const -> const std::vector<FileCheckResult> &
    {
        return m_results;
    }

    [[nodiscard]] auto existing() const -> QStringList
    {
        return find(true);
    }

    [[nodiscard]] auto missing() const -> QStringList
    {
        return find(false);
    }

private:
    std::vector<FileCheckResult> m_results;

    [[nodiscard]] auto find(bool existing) const -> QStringList
    {
        QStringList files;
        for (const auto &result : m_results)
        {
            if (result.exists() == existing)
            {
                files.append(result.path());
            }
        }
        return files;
    }
};

} // namespace Files

#pragma once

#include <QObject>
#include <utility>

namespace Files
{

/// Helper class for chaining multiple GET requests in a row.
template <typename ResultType> class MultiGetHelper
{
public:
    explicit MultiGetHelper(const QStringList &paths) : m_paths(paths)
    {
    }

    explicit MultiGetHelper(QStringList &&paths) : m_paths(std::move(paths))
    {
    }

    [[nodiscard]] auto isDone() const -> bool
    {
        return index >= m_paths.length();
    }

    [[nodiscard]] auto getNextPath() -> QString
    {
        return m_paths.at(index++);
    }

    void addResult(ResultType &&result)
    {
        m_results.push_back(result);
    }

    [[nodiscard]] auto getResults() const -> std::vector<ResultType>
    {
        return m_results;
    }

private:
    std::vector<ResultType> m_results;
    QStringList m_paths;
    int index = 0;
};

} // namespace Files

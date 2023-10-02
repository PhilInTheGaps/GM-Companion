#pragma once

#include <QObject>

namespace Files
{

/// Helper class for chaining multiple GET requests in a row.
template <typename ResultType> class MultiGetHelper
{
public:
    explicit MultiGetHelper(const QStringList &paths) : m_paths(paths)
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

    void addResult(std::shared_ptr<ResultType> result)
    {
        m_results.push_back(result);
    }

    [[nodiscard]] auto getResults() const -> std::vector<std::shared_ptr<ResultType>>
    {
        return m_results;
    }

private:
    std::vector<std::shared_ptr<ResultType>> m_results;
    QStringList m_paths;
    int index = 0;
};

} // namespace Files

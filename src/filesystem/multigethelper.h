#pragma once

#include <QObject>

namespace Files
{

/// Helper class for chaining multiple GET requests in a row.
template <typename ResultType> class MultiGetHelper : public QObject
{
public:
    explicit MultiGetHelper(const QStringList &paths, QObject *parent) : QObject(parent), m_paths(paths)
    {
    }

    bool isDone() const
    {
        return index >= m_paths.length();
    }
    QString getNextPath()
    {
        return m_paths[index++];
    }

    void addResult(ResultType *result)
    {
        m_results.push_back(result);
    }
    std::vector<ResultType *> getResults() const
    {
        return m_results;
    }

private:
    std::vector<ResultType *> m_results;
    QStringList m_paths;
    int index = 0;
};

} // namespace Files

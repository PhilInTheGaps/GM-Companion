#pragma once

#include "fileresult.h"
#include "filecheckresult.h"
#include <QVector>

namespace Files {

class FileMultiCheckResult : public FileResult
{
    Q_OBJECT
public:
    FileMultiCheckResult(const QVector<FileCheckResult*> &results, QObject *parent = nullptr)
        : FileResult(true, parent), m_results(results) {}

    FileMultiCheckResult(bool success, const QString& errorMessage, QObject *parent = nullptr)
        : FileResult(success, errorMessage, parent) {}

    FileMultiCheckResult(bool success, QObject *parent = nullptr)
        : FileResult(success, parent) {}

    void add(FileCheckResult *result) { result->setParent(this), m_results.append(result); }
    QVector<FileCheckResult*> results() const { return m_results; }

    QStringList existing() const { return find(true); }
    QStringList missing() const { return find(false); }

private:
    QVector<FileCheckResult*> m_results;

    QStringList find(bool existing) const {
        QStringList files;
        for (const auto *result : m_results)
        {
            if (result->exists() == existing)
            {
                files.append(result->path());
            }
        }
        return files;
    }
};

}

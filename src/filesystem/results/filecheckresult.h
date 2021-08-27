#pragma once

#include "fileresult.h"

namespace Files {

class FileCheckResult : public FileResult
{
    Q_OBJECT

public:
    explicit FileCheckResult(const QString& path, const QString& errorMessage, QObject *parent = nullptr)
        : FileResult(errorMessage, parent), m_path(path), m_exists(false) {}

    explicit FileCheckResult(const QString& path, bool exists, QObject *parent = nullptr)
        : FileResult(true, QByteArray(), parent), m_path(path), m_exists(exists) {}

    QString path() const { return m_path; }
    bool exists() const { return m_exists; }

private:
    const QString m_path;
    const bool m_exists;
};

}

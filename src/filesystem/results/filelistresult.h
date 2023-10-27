#pragma once

#include "fileresult.h"
#include <QStringList>

using namespace Qt::Literals::StringLiterals;
namespace Files
{

/// Class that either contains a list of paths to files and/or folders
/// or an error message.
class FileListResult : public FileResult
{
public:
    explicit FileListResult(const QString &path, const QString &errorMessage) : FileResult(errorMessage), m_path(path)
    {
    }

    explicit FileListResult(const QString &path, QString &&errorMessage)
        : FileResult(std::move(errorMessage)), m_path(path)
    {
    }

    explicit FileListResult(const QString &path, const QStringList &folders, const QStringList &files)
        : FileResult(true, QByteArray()), m_folders(folders), m_files(files), m_path(path)
    {
    }

    [[nodiscard]] auto filesFull(const QString &wildcard = u""_s) const -> QStringList;
    [[nodiscard]] auto foldersFull(const QString &wildcard = u""_s) const -> QStringList;

    [[nodiscard]] auto folders() const -> const QStringList &
    {
        return m_folders;
    }
    [[nodiscard]] auto files() const -> const QStringList &
    {
        return m_files;
    }
    [[nodiscard]] auto path() const -> const QString &
    {
        return m_path;
    }

private:
    [[nodiscard]] auto getFullyQualified(const QStringList &list) const -> QStringList;
    static auto getMatchingEntries(const QStringList &list, const QString &wildcard) -> QStringList;

    QStringList m_folders;
    QStringList m_files;
    QString m_path;
};

} // namespace Files

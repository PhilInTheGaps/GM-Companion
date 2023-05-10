#pragma once

#include <QDateTime>
#include <QString>
#include <vector>

struct MockFile
{
    QString name;
    QByteArray data;
    QDateTime lastModified;
};

class MockDir
{
public:
    explicit MockDir(const QString &name);

    [[nodiscard]] auto name() const -> QString
    {
        return m_name;
    }

    [[nodiscard]] auto lastModified() const -> QDateTime
    {
        return m_lastModified;
    }

    [[nodiscard]] auto dirs() const -> std::vector<MockDir>
    {
        return m_dirs;
    }

    auto mkdir(const QString &path) -> bool;
    void add(const MockDir &dir);

    [[nodiscard]] auto getFiles(const QString &path) -> std::vector<MockFile>;
    [[nodiscard]] auto getDirs(const QString &path) -> std::vector<MockDir>;

    void createOrUpdateFile(const QString &path, const QByteArray &data);
    [[nodiscard]] auto getFile(const QString &path) -> MockFile;
    [[nodiscard]] auto getDir(const QString &path) -> MockDir *;

    auto moveFileOrFolder(const QString &source, const QString &destination) -> bool;
    auto copyFileOrFolder(const QString &source, const QString &destination) -> bool;
    auto deleteFileOrFolder(const QString &path) -> bool;

private:
    QString m_name;
    QDateTime m_lastModified;

    std::vector<MockDir> m_dirs;
    std::vector<MockFile> m_files;
};

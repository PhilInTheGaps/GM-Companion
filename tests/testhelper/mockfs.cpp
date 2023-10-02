#include "mockfs.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(mockfs, "testing.mock.fs")

MockDir::MockDir(const QString &name) : m_name(name), m_lastModified(QDateTime::currentDateTime())
{
}

auto MockDir::mkdir(const QString &path) -> bool
{
    if (path.isEmpty()) return false;

    // dir already exists
    if (getDir(path)) return false;

    auto dirs = FileUtils::foldersFromDir(path);
    auto first = dirs.takeFirst();

    while (first.isEmpty() && !dirs.isEmpty())
    {
        first = dirs.takeFirst();
    }

    if (first.isEmpty()) return false;

    auto *subDir = getDir(first);

    // add subdir if it does not exist
    if (!subDir)
    {
        MockDir dir(first);

        if (!dirs.isEmpty())
        {
            dir.mkdir(FileUtils::dirFromFolders(dirs));
        }

        m_dirs.push_back(dir);
        return true;
    }

    return subDir->mkdir(FileUtils::dirFromFolders(dirs));
}

void MockDir::add(const MockDir &dir)
{
    m_dirs.push_back(dir);
}

auto MockDir::getFiles(const QString &path) -> std::vector<MockFile>
{
    if (path.isEmpty()) return m_files;

    auto *dir = getDir(path);
    if (!dir) return {};

    return dir->getFiles(QLatin1String());
}

auto MockDir::getDirs(const QString &path) -> std::vector<MockDir>
{
    if (path.isEmpty()) return m_dirs;

    auto *dir = getDir(path);
    if (!dir) return {};

    return dir->getDirs(QLatin1String());
}

void MockDir::createOrUpdateFile(const QString &path, const QByteArray &data)
{
    if (path.isEmpty()) return;

    auto dirs = FileUtils::foldersFromDir(path);
    auto first = dirs.takeFirst();

    while (first.isEmpty() && !dirs.isEmpty())
    {
        first = dirs.takeFirst();
    }

    if (first.isEmpty()) return;

    if (dirs.isEmpty())
    {
        // Try to update file
        for (auto &file : m_files)
        {
            if (file.name == first)
            {
                file.data = data;
                file.lastModified = QDateTime::currentDateTime();
                return;
            }
        }

        // Create new file
        m_files.push_back(MockFile{path, data, QDateTime::currentDateTime()});
        return;
    }

    // Try to find matching subdir
    for (auto &d : m_dirs)
    {
        if (d.m_name == first)
        {
            d.createOrUpdateFile(FileUtils::dirFromFolders(dirs), data);
            return;
        }
    }

    // Create subdir if it does not exist yet
    mkdir(first);
    m_dirs.back().createOrUpdateFile(FileUtils::dirFromFolders(dirs), data);
}

auto MockDir::getFile(const QString &path) -> MockFile
{
    if (path.isEmpty()) return {};

    auto fileName = FileUtils::fileName(path);
    auto dirName = FileUtils::dirFromPath(path);

    if (dirName.isEmpty())
    {
        for (const auto &file : m_files)
        {
            if (file.name == fileName) return file;
        }

        return {};
    }

    auto *dir = getDir(dirName);
    if (!dir) return {};

    return dir->getFile(fileName);
}

auto MockDir::moveFileOrFolder(const QString &source, const QString &destination) -> bool
{
    qCDebug(mockfs()) << "Move" << source << "to" << destination;

    if (!copyFileOrFolder(source, destination)) return false;

    return deleteFileOrFolder(source);
}

auto MockDir::copyFileOrFolder(const QString &source, const QString &destination) -> bool
{
    qCDebug(mockfs) << "Copy" << source << "to" << destination;

    if (source.isEmpty() || destination.isEmpty()) return false;

    const auto sourceFile = getFile(source);

    if (!sourceFile.name.isEmpty())
    {
        if (!getFile(destination).name.isEmpty()) return false;

        createOrUpdateFile(destination, sourceFile.data);
        return true;
    }

    const auto *sourceDir = getDir(source);
    if (!sourceDir) return false;

    auto destinationParentPath = FileUtils::dirFromPath(destination);
    mkdir(destinationParentPath);
    auto *destinationDir = getDir(destinationParentPath);
    if (!destinationDir) return false;

    destinationDir->add(*sourceDir);
    return true;
}

auto MockDir::deleteFileOrFolder(const QString &path) -> bool
{
    qCDebug(mockfs) << "Delete" << path;

    if (path.isEmpty()) return false;

    const auto fileName = FileUtils::fileName(path);
    const auto dirName = FileUtils::dirFromPath(path);

    auto isFile = [fileName](const MockFile &file) { return file.name == fileName; };
    auto isDir = [fileName](const MockDir &dir) { return dir.name() == fileName; };

    if (dirName.isEmpty())
    {
        auto oldSize = m_files.size();
        m_files.erase(std::remove_if(m_files.begin(), m_files.end(), isFile), m_files.end());

        if (m_files.size() < oldSize) return true;

        oldSize = m_dirs.size();
        m_dirs.erase(std::remove_if(m_dirs.begin(), m_dirs.end(), isDir), m_dirs.end());

        return m_dirs.size() < oldSize;
    }

    auto *dir = getDir(dirName);
    if (!dir) return false;

    return dir->deleteFileOrFolder(fileName);
}

auto MockDir::getDir(const QString &path) -> MockDir *
{
    if (path.isEmpty()) return this;

    auto dirs = FileUtils::foldersFromDir(path);
    auto first = dirs.takeFirst();

    while (first.isEmpty() && !dirs.isEmpty())
    {
        first = dirs.takeFirst();
    }

    for (auto &dir : m_dirs)
    {
        if (dir.name() == first)
        {
            if (dirs.isEmpty()) return &dir;

            return dir.getDir(FileUtils::dirFromFolders(dirs));
        }
    }

    return nullptr;
}

void MockDir::printTree(int depth) const
{
    for (const auto &file : m_files)
    {
        qCDebug(mockfs).noquote() << calcIndent(depth) << file.name << "(" << file.lastModified << ")";
    }

    for (const auto &dir : m_dirs)
    {
        qCDebug(mockfs).noquote() << calcIndent(depth) << dir.name() << "(" << dir.lastModified() << ")";
        dir.printTree(depth + 1);
    }
}

auto MockDir::calcIndent(int depth) -> QString
{
    static QString s("    ");

    auto ret = QStringLiteral("");

    while (depth-- > 0)
        ret.append(s);

    return ret;
}

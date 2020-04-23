#include "googledrivefile.h"

#include <utility>
#include "services/services.h"
#include "logging.h"

GoogleDriveFile::GoogleDriveFile(QString name, QString id, bool isFolder, GoogleDriveFile *parent) :
    QObject(parent), m_name(std::move(name)), m_id(std::move(id)), m_isFolder(isFolder)
{
}

GoogleDriveFile::GoogleDriveFile(QJsonObject fileObject, GoogleDriveFile *parent) :
    QObject(parent)
{
    m_name     = fileObject["name"].toString();
    m_id       = fileObject["id"].toString();
    m_isFolder = fileObject["mimeType"].toString() == GOOGLE_MIMETYPE_FOLDER;
}

GoogleDriveFile::~GoogleDriveFile()
{
    if (parent())
    {
        auto *parentFile = dynamic_cast<GoogleDriveFile *>(parent());

        if (parentFile)
        {
            parentFile->removeChild(this);
        }
    }

    for (auto *child : m_children)
    {
        delete child;
    }
}

auto GoogleDriveFile::getFile(QString path)->GoogleDriveFileReply
{
    qCDebug(gmFileAccessGoogle()) << "Looking for file or folder" << path;

    if (path.endsWith("/")) path = path.left(path.length() - 1);
    return getFile(path.split("/"));
}

auto GoogleDriveFile::getFile(QStringList path)->GoogleDriveFileReply
{
    if (path.isEmpty())
    {
        qCCritical(gmFileAccessGoogle()) << "Error: file path is empty.";
        return GoogleDriveFileReply(nullptr, OtherError);
    }

    // Safety check if current folder and basefolder names match
    if (path.takeFirst() != m_name)
    {
        qCCritical(gmFileAccessGoogle()) << "Error: folder names do not match.";
        return GoogleDriveFileReply(nullptr, OtherError);
    }

    // The current file is the one we are looking for
    // Is the case if we requested the top level folder
    if (path.isEmpty())
    {
        return GoogleDriveFileReply(this, m_isRefreshing ? Refreshing : Valid);
    }

    // We are currently fetching folder content
    if (m_isRefreshing) return GoogleDriveFileReply(this, Refreshing);

    // Folder content is not valid
    if (m_isFolder && !isUpToDate())
    {
        m_isRefreshing = true;
        return GoogleDriveFileReply(this, Invalid);
    }

    // Find file in children
    QString wanted = path.first();

    for (auto *child : m_children)
    {
        if (!child) continue;

        if (child->name() == wanted)
        {
            // We found the file
            if (path.length() == 1)
            {
                if (child->isFolder())
                {
                    return GoogleDriveFileReply(child, child->isUpToDate() ? Valid : Invalid);
                }

                return GoogleDriveFileReply(child, m_isRefreshing ? Refreshing : Valid);
            }

            // We found a subfolder and need to go deeper
            if (child->isFolder())
            {
                return child->getFile(path);
            }
        }
    }

    // We did not find the file
    return GoogleDriveFileReply(this, NotFound, wanted);
}

auto GoogleDriveFile::containsChild(const QString& name)->bool
{
    for (auto *child : m_children)
    {
        if (!child) continue;

        if (child->name() == name) return true;
    }

    return false;
}

void GoogleDriveFile::addChild(GoogleDriveFile *child)
{
    m_children.append(child);
    setRefreshed();
}

void GoogleDriveFile::receivedIndex()
{
    m_isRefreshing = false;
    setRefreshed();
}

auto GoogleDriveFile::isUpToDate() const->bool
{
    if (!m_refreshTime.isValid()) return false;

    return m_refreshTime.secsTo(QDateTime::currentDateTime()) < GOOGLE_DRIVE_CACHE_TIME;
}

void GoogleDriveFile::writeData(const QByteArray& data)
{
    m_hasData      = true;
    m_isRefreshing = false;

    if (m_file.open())
    {
        m_file.write(data);
        m_file.close();
    }
    else
    {
        qCCritical(gmFileAccessGoogle()) << "Error: Could not open temporary file!";
    }
}

auto GoogleDriveFile::readData()->QByteArray
{
    if (!m_hasData) return "";

    if (m_file.open())
    {
        auto data = m_file.readAll();
        m_file.close();
        return data;
    }

    qCCritical(gmFileAccessGoogle()) << "Error: Could not open temporary file!";
    return "";
}

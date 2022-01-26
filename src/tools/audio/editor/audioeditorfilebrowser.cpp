#include "audioeditorfilebrowser.h"
#include "settings/settingsmanager.h"
#include "filesystem/file.h"
#include "utils/fileutils.h"
#include "logging.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QQmlContext>

using namespace AsyncFuture;

AudioEditorFileBrowser::AudioEditorFileBrowser(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent), m_type(-1)
{
    m_fileModel = new AudioEditorFileModel(this);
    qmlEngine   = engine;
    qmlEngine->rootContext()->setContextProperty("audioEditorFileBrowserModel", m_fileModel);
}

/**
 * @brief Set the type of the current element
 * @param type Music: 0, Sounds: 1, Radio: 2
 */
void AudioEditorFileBrowser::setType(int type)
{
    if (type == m_type) return;

    qCDebug(gmAudioEditorFileBrowser()) << "Changing type:" << type << "...";
    m_type = type;
    emit typeChanged();

    switch (type)
    {
    case 0: // Music
        m_basePath = SettingsManager::getPath("music");
        break;

    case 1: // Sounds
        m_basePath = SettingsManager::getPath("sounds");
        break;

    case 2: // Radio
        m_basePath = SettingsManager::getPath("radio");
        break;

    default:
        m_basePath.clear();
        break;
    }

    clearFiles();
    updateFiles();
}

/**
 * @brief Update the file model
 */
void AudioEditorFileBrowser::updateFiles()
{
    addFiles({}, -1, true);
    addFiles({}, -1, false);
}

void AudioEditorFileBrowser::addFiles(const QStringList& path, int index, bool folders)
{
    qCDebug(gmAudioEditorFileBrowser()) << "Adding files:" << m_basePath << path << index << folders;

    const auto dir = FileUtils::fileInDir(FileUtils::dirFromFolders(path), m_basePath);
    observe(Files::File::listAsync(dir, !folders, folders)).subscribe([this, path, folders, index](Files::FileListResult *result) {
        if (!result) return;

        qCDebug(gmAudioEditorFileBrowser()) << "Received file list:" << result;
        addFilesToModel(folders ? result->folders() : result->files(), path, folders ? 3 : m_type, index);

        result->deleteLater();
    });
}

void AudioEditorFileBrowser::addFilesToModel(const QStringList& files, const QStringList& path, int type, int index)
{
    for (const auto& file : files)
    {
        auto *editorFile = new AudioEditorFile(file, path, type, this);

        if (index > -1)
        {
            m_fileModel->insert(index, editorFile);
        }
        else
        {
            m_fileModel->append(editorFile);
        }

        index++;
    }
}

void AudioEditorFileBrowser::removeElement(const QStringList& path)
{
    for (auto *element : m_fileModel->elements())
    {
        auto *file = qobject_cast<AudioEditorFile*>(element);

        if ((file->path() == path) || FileUtils::dirFromFolders(file->path())
                .startsWith(FileUtils::dirFromFolders(path)))
        {
            m_fileModel->remove(file);
        }
    }
}

void AudioEditorFileBrowser::clearFiles()
{
    m_fileModel->clear();
}

void AudioEditorFileBrowser::openFolder(bool open, const QString& folder, const QStringList& path)
{
    int index = 0;

    auto folderPath = path;
    folderPath.append(folder);

    for (auto *element : m_fileModel->elements())
    {
        auto *file = qobject_cast<AudioEditorFile *>(element);

        if ((file->name() == folder) && (file->path() == path))
        {
            if (open)
            {
                addFiles(folderPath, index + 1, true);
                addFiles(folderPath, index + 1, false);
                break;
            }
            
            removeElement(folderPath);
        }

        index++;
    }
}

auto AudioEditorFileModel::data(const QModelIndex& index, int /*role*/) const -> QVariant
{
    return QVariant::fromValue(m_items.at(index.row()));
}

void AudioEditorFileModel::insert(int index, QObject *item)
{
    beginInsertRows(QModelIndex(), index, index);
    m_items.insert(index, item);
    endInsertRows();

    emit isEmptyChanged();
}

void AudioEditorFileModel::append(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.append(item);
    endInsertRows();

    emit isEmptyChanged();
}

void AudioEditorFileModel::remove(QObject *item)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i) == item) {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.takeAt(i)->deleteLater();
            endRemoveRows();
            break;
        }
    }

    emit isEmptyChanged();
}

auto AudioEditorFileModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void AudioEditorFileModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_items.count() - 1);

    while (!m_items.empty())
    {
        m_items.takeAt(0)->deleteLater();
    }

    endRemoveRows();

    emit isEmptyChanged();
}

void AudioEditorFileModel::setElements(const QList<AudioEditorFile*> &elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        append(elements[i]);
    }

    emit isEmptyChanged();
}

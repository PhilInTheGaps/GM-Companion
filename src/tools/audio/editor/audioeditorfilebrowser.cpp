#include "audioeditorfilebrowser.h"
#include "filesystem/file.h"
#include "filesystem/results/filelistresult.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <QQmlContext>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioEditorFileBrowser, "gm.audio.editor.filebrowser")

AudioEditorFileBrowser::AudioEditorFileBrowser(QObject *parent) : QObject(parent), m_fileModel(this)
{
    onTypeChanged(type());
    connect(this, &AudioEditorFileBrowser::typeChanged, this, &AudioEditorFileBrowser::onTypeChanged);
}

/**
 * @brief Update the file model
 */
void AudioEditorFileBrowser::updateFiles()
{
    addFiles({}, -1, true);
    addFiles({}, -1, false);
}

void AudioEditorFileBrowser::addFiles(const QStringList &path, int index, bool folders)
{
    qCDebug(gmAudioEditorFileBrowser()) << "Adding files:" << m_basePath << path << index << folders;

    const auto dir = FileUtils::fileInDir(FileUtils::dirFromFolders(path), m_basePath);
    Files::File::listAsync(dir, !folders, folders)
        .then([this, path, folders, index](const Files::FileListResult &result) {
            if (!result.success()) return;

            addFilesToModel(folders ? result.folders() : result.files(), path, folders ? 3 : static_cast<int>(type()),
                            index);
        });
}

void AudioEditorFileBrowser::addFilesToModel(const QStringList &files, const QStringList &path, int type, int index)
{
    for (const auto &file : files)
    {
        auto *editorFile = new AudioEditorFile(file, path, type, this);

        if (index > -1)
        {
            m_fileModel.insert(index, editorFile);
        }
        else
        {
            m_fileModel.append(editorFile);
        }

        index++;
    }
}

void AudioEditorFileBrowser::removeElement(const QStringList &path)
{
    foreach (auto *element, m_fileModel.elements())
    {
        auto *file = qobject_cast<AudioEditorFile *>(element);

        if ((file->path() == path) ||
            FileUtils::dirFromFolders(file->path()).startsWith(FileUtils::dirFromFolders(path)))
        {
            m_fileModel.remove(file);
        }
    }
}

void AudioEditorFileBrowser::clearFiles()
{
    m_fileModel.clear();
}

void AudioEditorFileBrowser::onTypeChanged(AudioElement::Type type)
{
    switch (type)
    {
    case AudioElement::Type::Music:
        m_basePath = SettingsManager::getPath(u"music"_s);
        break;

    case AudioElement::Type::Sound:
        m_basePath = SettingsManager::getPath(u"sounds"_s);
        break;

    case AudioElement::Type::Radio:
        m_basePath = SettingsManager::getPath(u"radio"_s);
        break;

    default:
        m_basePath.clear();
        break;
    }

    clearFiles();
    updateFiles();
}

void AudioEditorFileBrowser::openFolder(bool open, const QString &folder, const QStringList &path)
{
    int index = 0;

    auto folderPath = path;
    folderPath.append(folder);

    foreach (auto *element, m_fileModel.elements())
    {
        if (const auto *file = qobject_cast<AudioEditorFile *>(element);
            (file->name() == folder) && (file->path() == path))
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

auto AudioEditorFileBrowserModel::data(const QModelIndex &index, int /*role*/) const -> QVariant
{
    return QVariant::fromValue(m_items.at(index.row()));
}

void AudioEditorFileBrowserModel::insert(int index, QObject *item)
{
    beginInsertRows(QModelIndex(), index, index);
    m_items.insert(index, item);
    endInsertRows();

    emit isEmptyChanged();
}

void AudioEditorFileBrowserModel::append(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.append(item);
    endInsertRows();

    emit isEmptyChanged();
}

void AudioEditorFileBrowserModel::remove(const QObject *item)
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items.at(i) == item)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.takeAt(i)->deleteLater();
            endRemoveRows();
            break;
        }
    }

    emit isEmptyChanged();
}

auto AudioEditorFileBrowserModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void AudioEditorFileBrowserModel::clear()
{
    if (m_items.isEmpty()) return;

    beginRemoveRows(QModelIndex(), 0, static_cast<int>(m_items.count()) - 1);

    while (!m_items.empty())
    {
        m_items.takeFirst()->deleteLater();
    }

    endRemoveRows();

    emit isEmptyChanged();
}

void AudioEditorFileBrowserModel::setElements(const QList<AudioEditorFile *> &elements)
{
    clear();

    for (auto i = elements.size() - 1; i > -1; i--)
    {
        append(elements.at(i));
    }

    emit isEmptyChanged();
}

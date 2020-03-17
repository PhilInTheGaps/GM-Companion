#include "audioeditorfilebrowser.h"
#include "functions.h"

#include <QDebug>
#include <QQmlContext>

AudioEditorFileBrowser::AudioEditorFileBrowser(QQmlApplicationEngine *engine, QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Audio Editor File Browser ...";

    m_type = -1;

    m_fileModel = new AudioEditorFileModel;
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

    qDebug() << "AudioEditorFileBrowser: Changing type:" << type << "...";
    m_type = type;
    emit typeChanged();

    switch (type)
    {
    case 0: // Music
        m_basePath = sManager.getSetting(Setting::musicPath);
        break;

    case 1: // Sounds
        m_basePath = sManager.getSetting(Setting::soundPath);
        break;

    case 2: // Radio
        m_basePath = sManager.getSetting(Setting::radioPath);
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
    QStringList tempPath;

    addFolders(tempPath);
    addFiles(tempPath);
}

int AudioEditorFileBrowser::addFolders(QStringList path, int index)
{
    int count = 0;

    for (QString f : getFolders(m_basePath + "/" + path.join("/")))
    {
        if ((f != ".") && (f != ".."))
        {
            auto file = new AudioEditorFile(f, path, 3);

            if (index > -1)
            {
                m_fileModel->insert(index + count, file);
            }
            else
            {
                m_fileModel->append(file);
            }

            count++;
        }
    }

    return count;
}

int AudioEditorFileBrowser::addFiles(QStringList path, int index)
{
    int count = 0;

    for (QString f : getFiles(m_basePath + "/" + path.join("/")))
    {
        if (!f.endsWith("."))
        {
            auto file = new AudioEditorFile(f, path, m_type);

            if (index > -1)
            {
                m_fileModel->insert(index + count, file);
            }
            else
            {
                m_fileModel->append(file);
            }

            count++;
        }
    }
    return count;
}

void AudioEditorFileBrowser::removeElement(QStringList path)
{
    for (auto e : m_fileModel->elements())
    {
        auto file = static_cast<AudioEditorFile *>(e);

        if ((file->path() == path) || file->path().join("/").startsWith(path.join("/")))
        {
            m_fileModel->remove(file);

            if (file) file->deleteLater();
        }
    }
}

void AudioEditorFileBrowser::clearFiles()
{
    for (auto e : m_fileModel->elements())
    {
        if (e) e->deleteLater();
    }

    m_fileModel->clear();
}

void AudioEditorFileBrowser::openFolder(bool open, QString folder, QStringList path)
{
    int index              = 0;
    QStringList folderPath = path;

    folderPath.append(folder);

    for (auto e : m_fileModel->elements())
    {
        auto file = static_cast<AudioEditorFile *>(e);

        if ((file->name() == folder) && (file->path() == path))
        {
            if (open)
            {
                int count = addFolders(folderPath, index + 1) + 1;
                count += addFiles(folderPath, index + count);
                break;
            }
            else
            {
                removeElement(folderPath);
            }
        }

        index++;
    }
}

/**
 * @brief Add all files from current path to the current element
 */
void AudioEditorFileBrowser::addAllFiles()
{
    //    emit addFiles(m_filePaths);
}

QVariant AudioEditorFileModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
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
            m_items.remove(i);
            endRemoveRows();
            break;
        }
    }

    emit isEmptyChanged();
}

QHash<int, QByteArray>AudioEditorFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void AudioEditorFileModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_items.count() - 1);

    while (m_items.size() > 0)
    {
        m_items[0]->deleteLater();
        m_items.remove(0);
    }

    endRemoveRows();

    emit isEmptyChanged();
}

void AudioEditorFileModel::setElements(QList<AudioEditorFile *>elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        append(elements[i]);
    }

    emit isEmptyChanged();
}

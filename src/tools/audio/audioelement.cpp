#include "audioelement.h"
#include <QBuffer>
#include <QDebug>
#include <QJsonArray>
#include <QTimer>
#include "logging.h"

AudioElement::AudioElement(QString name, int type, QString path) :
    m_name(name), m_type(type)
{
    m_path = path + "/" + QString::number(m_type) + "/" + name;
    m_icon = new AudioIcon(path);
}

AudioElement::~AudioElement()
{
    m_icon->deleteLater();

    for (auto f : m_files)
    {
        if (f) f->deleteLater();
    }
}

AudioElement::AudioElement(QJsonObject object, int type, QString path, QObject *parent) : QObject(parent)
{
    m_name             = object["name"].toString();
    m_relativeIconPath = object["icon"].toString();
    m_mode             = object["mode"].toInt();
    m_path             = path + "/" + QString::number(type) + "/" + m_name;
    m_icon             = new AudioIcon(m_path);
    m_type             = type;

    for (auto file : object.value("files").toArray())
    {
        m_files.append(new AudioFile(file.toObject()));
    }
}

QJsonObject AudioElement::toJson()
{
    QJsonObject object;

    object.insert("name", m_name);
    object.insert("icon", m_relativeIconPath);
    object.insert("mode", m_mode);

    // Files
    QJsonArray files;

    for (auto f : m_files)
    {
        files.append(QJsonObject{
            { "url", f->url() },
            { "source", f->source() }
        });
    }
    object.insert("files", files);

    return object;
}

QVariant AudioElementModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void AudioElementModel::insert(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();

    emit isEmptyChanged();
}

void AudioElementModel::remove(QObject *item)
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

QHash<int, QByteArray>AudioElementModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void AudioElementModel::clear()
{
    while (m_items.size() > 0)
    {
        remove(m_items[0]);
    }

    emit isEmptyChanged();
}

void AudioElementModel::setElements(QList<AudioElement *>elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }

    emit isEmptyChanged();
}

QVariant AudioElementModelModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void AudioElementModelModel::insert(QObject *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.push_front(item);
    endInsertRows();

    emit isEmptyChanged();
}

void AudioElementModelModel::append(QObject *item)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.push_back(item);
    endInsertRows();

    emit isEmptyChanged();
}

void AudioElementModelModel::remove(QObject *item)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i) == item) {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.takeAt(i)->deleteLater();

            //            m_items.remove(i);
            endRemoveRows();
            break;
        }
    }

    emit isEmptyChanged();
}

QHash<int, QByteArray>AudioElementModelModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

/**
 * @brief Remove all items except the first one
 */
void AudioElementModelModel::clear()
{
    while (m_items.size() > 1)
    {
        remove(m_items[1]);
    }

    emit isEmptyChanged();
}

void AudioElementModelModel::setElements(QList<AudioElementModel *>elements)
{
    clear();

    for (int i = elements.size() - 1; i > -1; i--)
    {
        insert(elements[i]);
    }

    emit isEmptyChanged();
}

QVariant AudioFileModel::data(const QModelIndex& index, int /*role*/) const
{
    QObject *item = m_items.at(index.row());

    return QVariant::fromValue(item);
}

void AudioFileModel::insert(int index, QObject *item)
{
    beginInsertRows(QModelIndex(), index, index);
    m_items.insert(index, item);
    endInsertRows();

    emit isEmptyChanged();
}

void AudioFileModel::append(QObject *item)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(item);
    endInsertRows();

    emit isEmptyChanged();
}

bool AudioFileModel::moveRow(const QModelIndex& sourceParent, int sourceRow, const QModelIndex& destinationParent, int destinationChild)
{
    if (beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, (destinationChild > sourceRow) ? destinationChild + 1 : destinationChild))
    {
        m_items.move(sourceRow, destinationChild);
        endMoveRows();
    }

    return true;
}

void AudioFileModel::remove(QObject *item)
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i) == item) {
            beginRemoveRows(QModelIndex(), i, i);
            m_items.remove(i);

            //            if (item) item->deleteLater();
            endRemoveRows();
            break;
        }
    }

    emit isEmptyChanged();
}

QHash<int, QByteArray>AudioFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void AudioFileModel::clear()
{
    while (m_items.size() > 0)
    {
        beginRemoveRows(QModelIndex(), 0, 0);

        //        m_items[0]->deleteLater();
        m_items.remove(0);
        endRemoveRows();
    }


    emit isEmptyChanged();
}

void AudioFileModel::setElements(QList<AudioFile *>elements)
{
    clear();

    for (auto element : elements)
    {
        append(element);
    }

    emit isEmptyChanged();
}

bool AudioIcon::addCollageIcon(const QPair<QString, QPixmap>icon)
{
    // Check if icon already exists
    for (auto entry : m_collageIcons)
    {
        if (entry.first == icon.first) return false;

        if (entry.second.cacheKey() == icon.second.cacheKey()) return false;
    }

    m_collageIcons.append(icon);
    return true;
}

void AudioIcon::setCollageIcon(const QPair<QString, QPixmap>icon, int index)
{
    qCDebug(gmAudioTool) << "Setting collage icon to index" << index;
    qCDebug(gmAudioTool) << "   Icon count:" << m_collageIcons.length();

    if (m_collageIcons.length() < index)
    {
        qCWarning(gmAudioTool) << "Could not set collage icon, index out of range";
        return;
    }

    m_collageIcons.replace(index, icon);
}

AudioFile::AudioFile(QJsonObject object, QObject *parent) : QObject(parent)
{
    m_url    = object["url"].toString();
    m_source = object["source"].toInt();
}

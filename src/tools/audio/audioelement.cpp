#include "audioelement.h"
#include <QDebug>

AudioElement::AudioElement(QString name, QString icon) :
    m_name(name), m_icon(icon) {}

MusicElement::MusicElement(QString name)
{
    m_name = name;
}

SoundElement::SoundElement(QString name)
{
    m_name = name;
}

RadioElement::RadioElement(QString name, bool local)
{
    m_name  = name;
    m_local = local;
}

SpotifyElement::SpotifyElement(QString name, QString id)
{
    m_name = name;
    m_id   = id;
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

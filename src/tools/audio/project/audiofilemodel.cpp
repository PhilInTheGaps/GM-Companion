#include "audiofilemodel.h"

auto AudioFileModel::data(const QModelIndex& index, int /*role*/) const -> QVariant
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

auto AudioFileModel::moveRow(const QModelIndex& sourceParent, int sourceRow, const QModelIndex& destinationParent, int destinationChild) -> bool
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
            endRemoveRows();
            break;
        }
    }

    emit isEmptyChanged();
}

auto AudioFileModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void AudioFileModel::clear()
{
    while (!m_items.empty())
    {
        beginRemoveRows(QModelIndex(), 0, 0);
        m_items.remove(0);
        endRemoveRows();
    }

    emit isEmptyChanged();
}

void AudioFileModel::setElements(const QList<AudioFile *>&elements)
{
    clear();

    for (auto element : elements)
    {
        append(element);
    }

    emit isEmptyChanged();
}

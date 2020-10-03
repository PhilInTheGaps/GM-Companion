#include "audioelementmodel.h"

auto AudioElementModel::data(const QModelIndex& index, int /*role*/) const -> QVariant
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

auto AudioElementModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;

    roles[Qt::UserRole + 1] = "item";
    return roles;
}

void AudioElementModel::clear()
{
    while (!m_items.empty())
    {
        remove(m_items[0]);
    }

    emit isEmptyChanged();
}

void AudioElementModel::setSearchTerm(const QString &term)
{
    if (term.isEmpty())
    {

    }
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

auto AudioElementModelModel::data(const QModelIndex& index, int /*role*/) const -> QVariant
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
            endRemoveRows();
            break;
        }
    }

    emit isEmptyChanged();
}

auto AudioElementModelModel::roleNames() const -> QHash<int, QByteArray>
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "item";
    return roles;
}

/**
 * @brief Remove all items
 */
void AudioElementModelModel::clear()
{
    while (!m_items.empty())
    {
        remove(m_items.first());
    }

    emit isEmptyChanged();
}

void AudioElementModelModel::setSearchTerm(const QString &term)
{
    for (auto *item : m_items)
    {
        auto model = qobject_cast<AudioElementModel*>(item);

        if (model)
        {
            model->setSearchTerm(term);
        }
    }
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

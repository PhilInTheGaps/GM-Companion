#include "customobjectlistmodel.h"
#include "utils/utils.h"

CustomObjectListModel::CustomObjectListModel(bool isOwning, QObject *parent)
    : QAbstractListModel(parent), a_isOwning(isOwning)
{
}

auto CustomObjectListModel::rowCount(const QModelIndex &parent) const -> int
{
    Q_UNUSED(parent)
    return m_objects.size();
}

auto CustomObjectListModel::headerData(int section, Qt::Orientation orientation, int role) const -> QVariant
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return {};
}

void CustomObjectListModel::clear()
{
    beginResetModel();

    if (isOwning())
    {
        QObject *object = nullptr;
        while (!m_objects.isEmpty())
        {
            object = m_objects.takeLast();
            if (object) object->deleteLater();
        }
    }
    else
    {
        m_objects.clear();
    }

    endResetModel();
}

void CustomObjectListModel::replaceAll(const QList<QObject *> &objects)
{
    clear();

    takeOwnershipIfRequired(objects);

    beginResetModel();
    m_objects = objects;
    endResetModel();
}

auto CustomObjectListModel::removeRows(int row, int count, const QModelIndex &parent) -> bool
{
    const auto firstIndex = row;
    auto lastIndex = row + count - 1;

    if (Utils::isInBounds(m_objects, firstIndex) && Utils::isInBounds(m_objects, lastIndex))
    {
        beginRemoveRows(parent, firstIndex, lastIndex);

        if (isOwning())
        {
            QObject *object = nullptr;
            for (int i = firstIndex; i <= lastIndex; lastIndex--)
            {
                object = m_objects.takeAt(i);
                if (object) object->deleteLater();
            }
        }
        else
        {
            m_objects.remove(firstIndex, count);
        }

        endRemoveRows();

        return true;
    }

    return false;
}

auto CustomObjectListModel::insert(int row, QObject *object, const QModelIndex &parent) -> bool
{
    if (!Utils::isInBounds(m_objects, row) && row != rowCount()) return false;

    takeOwnershipIfRequired(object);

    beginInsertRows(parent, row, row);
    m_objects.insert(row, object);
    endInsertRows();

    return true;
}

void CustomObjectListModel::append(QObject *object, const QModelIndex &parent)
{
    takeOwnershipIfRequired(object);

    const auto lastIndex = m_objects.size();
    beginInsertRows(parent, lastIndex, lastIndex);
    m_objects.append(object);
    endInsertRows();
}

void CustomObjectListModel::prepend(QObject *object, const QModelIndex &parent)
{
    takeOwnershipIfRequired(object);

    beginInsertRows(parent, 0, 0);
    m_objects.prepend(object);
    endInsertRows();
}

auto CustomObjectListModel::get(int row) const -> QObject *
{
    if (!Utils::isInBounds(m_objects, row)) return nullptr;

    return m_objects.at(row);
}

auto CustomObjectListModel::getAll() const -> QList<QObject *>
{
    return m_objects;
}

void CustomObjectListModel::takeOwnershipIfRequired(QObject *object)
{
    if (!object || !isOwning()) return;

    object->setParent(this);
}

void CustomObjectListModel::takeOwnershipIfRequired(const QList<QObject *> &objects)
{
    if (!isOwning()) return;

    foreach (auto *object, objects)
    {
        if (object) object->setParent(this);
    }
}

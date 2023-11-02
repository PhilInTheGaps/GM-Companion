#pragma once

#include "utils/utils.h"
#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>
#include <memory>
#include <vector>

namespace Common
{

template <class T> class CustomSharedPtrListModel : public QAbstractListModel
{
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    using QAbstractListModel::QAbstractListModel;

    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override = 0;

    [[nodiscard]] auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override
    {
        Q_UNUSED(parent)
        return m_objects.size();
    }

    [[nodiscard]] auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
        -> QVariant override
    {
        Q_UNUSED(section)
        Q_UNUSED(orientation)
        Q_UNUSED(role)
        return {};
    }

    void clear()
    {
        beginResetModel();
        m_objects.clear();
        endResetModel();
    }

    void replaceAll(const std::vector<std::shared_ptr<T>> &objects)
    {
        beginResetModel();
        m_objects = objects;
        endResetModel();
    }

    auto removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) -> bool override
    {
        const auto firstIndex = row;
        auto lastIndex = row + count - 1;

        if (Utils::isInBounds(m_objects, firstIndex) && Utils::isInBounds(m_objects, lastIndex))
        {
            beginRemoveRows(parent, firstIndex, lastIndex);
            m_objects.erase(m_objects.begin() + firstIndex, m_objects.begin() + firstIndex + count);
            endRemoveRows();
            return true;
        }

        return false;
    }

    auto insert(int row, std::shared_ptr<T> object, const QModelIndex &parent = QModelIndex()) -> bool
    {
        if (!Utils::isInBounds(m_objects, row) && row != rowCount()) return false;

        beginInsertRows(parent, row, row);
        m_objects.insert(m_objects.begin() + row, object);
        endInsertRows();

        return true;
    }

    void append(std::shared_ptr<T> object, const QModelIndex &parent = QModelIndex())
    {
        const auto lastIndex = m_objects.size();
        beginInsertRows(parent, lastIndex, lastIndex);
        m_objects.push_back(object);
        endInsertRows();
    }

    void prepend(std::shared_ptr<T> object, const QModelIndex &parent = QModelIndex())
    {
        beginInsertRows(parent, 0, 0);
        m_objects.insert(m_objects.begin(), object);
        endInsertRows();
    }

    [[nodiscard]] auto get(int row) const -> std::shared_ptr<T>
    {
        if (!Utils::isInBounds(m_objects, row)) return {};

        return m_objects.at(row);
    }

    [[nodiscard]] auto getAll() const -> std::vector<std::shared_ptr<T>>
    {
        return m_objects;
    }

private:
    std::vector<std::shared_ptr<T>> m_objects;
};

} // namespace Common

#pragma once

#include <QList>
#include <QObject>
#include <vector>

class Utils
{
public:
    static auto isInHierarchy(const QObject &object, const QObject *root) -> bool;

    template <typename T> static auto toQObjectList(const T &from) -> QList<QObject *>
    {
        return toList<QObject *>(from);
    }

    template <typename T, typename L> static auto toList(const L &from) -> QList<T>
    {
        QList<T> list;
        list.reserve(from.size());
        foreach (auto entry, from)
        {
            auto *object = qobject_cast<T>(entry);
            if (object) list.push_back(object);
        }
        return list;
    }

    template <typename T> static auto isInBounds(const T &list, qsizetype index) -> bool
    {
        return index > -1 && index < std::size(list);
    }

    template <typename T> static auto copyList(const QList<T *> &original) -> QList<T *>
    {
        QList<T *> list;
        list.reserve(original.size());
        foreach (auto *item, original)
        {
            if (item) list << new T(*item);
        }
        return list;
    }
};

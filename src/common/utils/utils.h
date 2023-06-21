#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils
{
public:
    static auto isInHierarchy(const QObject &object, const QObject *root) -> bool;

    template <typename T> static auto toQObjectList(const T &from) -> QList<QObject *>
    {
        return toList<QObject *>(from);
    }

    template <typename T> static auto toQObjectVector(const T &from) -> QVector<QObject *>
    {
        return toVector<QObject *>(from);
    }

    template <typename T, typename L> static auto toList(const L &from) -> QList<T>
    {
        QList<T> list;
        list.reserve(from.count());
        for (auto entry : from)
        {
            auto *object = qobject_cast<T>(entry);
            if (object) list.push_back(object);
        }
        return list;
    }

    template <typename T, typename L> static auto toVector(const L &from) -> QVector<T>
    {
        QVector<T> list;
        list.reserve(from.count());
        for (auto entry : from)
        {
            auto *object = qobject_cast<T>(entry);
            if (object) list.push_back(object);
        }
        return list;
    }

    template <typename T> static auto isInBounds(const QList<T> &list, int index) -> bool
    {
        return index > -1 && index < list.length();
    }

    template <typename T> static auto isInBounds(const QVector<T> &list, int index) -> bool
    {
        return index > -1 && index < list.length();
    }

    template <typename T> static auto copyList(const QList<T *> &original) -> QList<T *>
    {
        QList<T *> list;
        list.reserve(original.count());
        for (auto *item : original)
        {
            if (item) list << new T(*item);
        }
        return list;
    }
};

#endif // UTILS_H

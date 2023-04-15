#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils
{
public:
    static auto isInHierarchy(const QObject &object, const QObject *root) -> bool;

    template <typename T> static auto toQObjectList(const T &from) -> QList<QObject *>
    {
        QList<QObject *> list;
        list.reserve(from.count());
        for (auto entry : from)
        {
            auto *object = qobject_cast<QObject *>(entry);
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

#ifndef UTILS_H
#define UTILS_H

#include <QPixmap>

class Utils
{
public:
    static auto stringFromImage(const QPixmap &image) -> QString;
    static auto rot13(const QString &input) -> QString;
    static auto hasWildcardMatch(const QString &string, const QString &wildcard) -> bool;
    static auto isInHierarchy(const QObject &object, const QObject *root) -> bool;

    template<typename T>
    static auto toQObjectList(const T &from) -> QList<QObject*>
    {
        QList<QObject*> list;
        list.reserve(from.count());
        for (auto entry : from)
        {
            auto *object = qobject_cast<QObject*>(entry);
            if (object) list.push_back(object);
        }
        return list;
    }

    template<typename T>
    static auto isInBounds(const QList<T> &list, int index) -> bool
    {
        return index > -1 && index < list.length();
    }

    template<typename T>
    static auto isInBounds(const QVector<T> &list, int index) -> bool
    {
        return index > -1 && index < list.length();
    }
};

#endif // UTILS_H

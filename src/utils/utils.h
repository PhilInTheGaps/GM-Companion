#ifndef UTILS_H
#define UTILS_H

#include <QPixmap>

class Utils
{
public:
    static QString stringFromImage(const QPixmap &image);
    static QString rot13(const QString &input);
    static bool hasWildcardMatch(const QString &string, const QString &wildcard);

    template<typename T> static QList<QObject*> toQObjectList(QList<T> from)
    {
        QList<QObject*> list;
        for (auto entry : from)
        {
            auto *object = qobject_cast<QObject*>(entry);
            if (object) list.push_back(object);
        }
        return list;
    }
};

#endif // UTILS_H

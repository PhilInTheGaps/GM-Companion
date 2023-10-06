#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class AudioMetaData : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    AUTO_PROPERTY2(QString, title, QStringLiteral("-"))
    AUTO_PROPERTY2(QStringList, artist, {QStringLiteral("-")})
    AUTO_PROPERTY2(QString, album, QStringLiteral("-"))
    AUTO_PROPERTY(QString, cover)
    AUTO_PROPERTY_VAL2(qint64, length, 0)

public:
    using QObject::QObject;

    void apply(const AudioMetaData &other)
    {
        title(other.title());
        artist(other.artist());
        album(other.album());
        cover(other.cover());
        length(other.length());
    }

    void clear()
    {
        title(QStringLiteral("-"));
        artist({QStringLiteral("-")});
        album(QStringLiteral("-"));
        cover(QStringLiteral());
        length(0);
    }
};

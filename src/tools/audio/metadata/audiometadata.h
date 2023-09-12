#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class AudioMetaData : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    using QObject::QObject;

    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(QString, artist)
    AUTO_PROPERTY(QString, album)
    AUTO_PROPERTY(QString, cover)
    AUTO_PROPERTY(QString, type)
    AUTO_PROPERTY_VAL2(qint64, length, 0)
};

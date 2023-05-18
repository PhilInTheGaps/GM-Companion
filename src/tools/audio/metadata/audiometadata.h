#ifndef AUDIOMETADATA_H
#define AUDIOMETADATA_H

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>

class AudioMetaData : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

    AUTO_PROPERTY(QString, title)
    AUTO_PROPERTY(QString, artist)
    AUTO_PROPERTY(QString, album)
    AUTO_PROPERTY(QString, cover)
    AUTO_PROPERTY(QString, type)
    AUTO_PROPERTY_VAL2(qint64, length, 0)
};

#endif // AUDIOMETADATA_H

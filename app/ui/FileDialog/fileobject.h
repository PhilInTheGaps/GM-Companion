#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Files
{

class FileObject : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    using QObject::QObject;

    explicit FileObject(const QString &name, bool isFolder, QObject *parent)
        : QObject(parent), a_name(name), a_isFolder(isFolder)
    {
    }

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY_VAL(bool, isFolder)
};

} // namespace Files

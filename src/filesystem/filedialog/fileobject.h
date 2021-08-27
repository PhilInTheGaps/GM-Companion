#pragma once

#include <QObject>
#include "thirdparty/propertyhelper/PropertyHelper.h"

namespace Files {

class FileObject : public QObject
{
    Q_OBJECT
public:
    FileObject(const QString &name, bool isFolder, QObject *parent)
        : QObject(parent), a_name(name), a_isFolder(isFolder) {}

    AUTO_PROPERTY(QString, name)
    AUTO_PROPERTY(bool, isFolder)
};

}

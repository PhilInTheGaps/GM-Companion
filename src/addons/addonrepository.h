#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <utility>

class AddonRepository : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    READONLY_PROPERTY(QString, url)
    READONLY_PROPERTY(bool, isDefault)

public:
    explicit AddonRepository(QObject *parent, QString url, bool isDefault)
        : QObject{parent}, a_url(std::move(url)), a_isDefault(isDefault)
    {
    }
};

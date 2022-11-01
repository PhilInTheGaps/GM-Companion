#pragma once

#include <QObject>
#include <utility>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AddonRepository : public QObject
{
    Q_OBJECT

    READONLY_PROPERTY(QString, url)
    READONLY_PROPERTY(bool, isDefault)

public:
    explicit AddonRepository(QObject *parent, QString  url, bool isDefault)
      : QObject{parent}, a_url(std::move(url)), a_isDefault(isDefault) {}
};

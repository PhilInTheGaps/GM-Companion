#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>

class ServiceStatus : public QObject
{
    Q_OBJECT
public:
    ServiceStatus(QObject *parent) : QObject(parent)
    {
    }

    enum class Type
    {
        Info = 0,
        Success = 1,
        Warning = 2,
        Error = 3
    };

    Q_ENUM(Type)

    AUTO_PROPERTY_VAL2(Type, type, Type::Info)
    AUTO_PROPERTY(QString, message)
};

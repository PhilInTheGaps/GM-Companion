#pragma once

#include <QObject>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class ServiceStatus : public QObject
{
    Q_OBJECT
public:
    ServiceStatus(QObject *parent) : QObject(parent), a_type(Type::Info) {}
    ~ServiceStatus() override = default;

    enum Type {
        Info = 0,
        Success = 1,
        Warning = 2,
        Error = 3
    };

    AUTO_PROPERTY(int, type)
    AUTO_PROPERTY(QString, message)
};

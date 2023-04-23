#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>

class ConverterUnit : public QObject
{
    Q_OBJECT
public:
    explicit ConverterUnit(const QString &name, double value, QObject *parent);
    explicit ConverterUnit(const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    AUTO_PROPERTY(QString, name);
    AUTO_PROPERTY(double, value);
};

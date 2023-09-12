#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class ConverterUnit : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit ConverterUnit(const QString &name, double value, QObject *parent);
    explicit ConverterUnit(const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    AUTO_PROPERTY(QString, name);
    AUTO_PROPERTY_VAL(double, value);
};

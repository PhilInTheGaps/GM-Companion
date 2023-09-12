#pragma once

#include "converterunit.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class ConverterCategory : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    AUTO_PROPERTY(QString, name);
    READ_LIST_PROPERTY(ConverterUnit, units);

public:
    explicit ConverterCategory(const QString &name, QObject *parent);
    explicit ConverterCategory(const QString &name, const QList<ConverterUnit *> &units, QObject *parent);
    explicit ConverterCategory(const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    void setUnits(const QList<ConverterUnit *> &units);
};

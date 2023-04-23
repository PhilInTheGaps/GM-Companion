#pragma once

#include "converterunit.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonObject>
#include <QObject>

class ConverterCategory : public QObject
{
    Q_OBJECT
public:
    explicit ConverterCategory(const QString &name, QObject *parent);
    explicit ConverterCategory(const QString &name, const QList<ConverterUnit *> &units, QObject *parent);
    explicit ConverterCategory(const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;

    AUTO_PROPERTY(QString, name);
    AUTO_PROPERTY(QList<ConverterUnit *>, units);
};

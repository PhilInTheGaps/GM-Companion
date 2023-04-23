#pragma once

#include "convertercategory.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

class ConverterProject : public QObject
{
    Q_OBJECT
public:
    explicit ConverterProject(const QString &name, QObject *parent);
    explicit ConverterProject(const QString &name, const QList<ConverterCategory *> &categories, QObject *parent);
    explicit ConverterProject(const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonDocument;

    AUTO_PROPERTY(QString, name);
    AUTO_PROPERTY(QList<ConverterCategory *>, categories);
    READONLY_PROPERTY(int, version);
};

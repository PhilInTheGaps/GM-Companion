#pragma once

#include "convertercategory.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QtQml/qqmlregistration.h>

class ConverterProject : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    AUTO_PROPERTY(QString, name);
    READONLY_PROPERTY2(int, version, 2);
    READ_LIST_PROPERTY(ConverterCategory, categories);

public:
    explicit ConverterProject(const QString &name, QObject *parent);
    explicit ConverterProject(const QString &name, const QList<ConverterCategory *> &categories, QObject *parent);
    explicit ConverterProject(const QJsonObject &json, QObject *parent);

    [[nodiscard]] auto toJson() const -> QJsonDocument;

    void setCategories(const QList<ConverterCategory *> &categories);
};

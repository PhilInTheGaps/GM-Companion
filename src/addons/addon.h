#pragma once

#include <QObject>
#include <QJsonDocument>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class Addon : public QObject
{
    Q_OBJECT

    READONLY_PROPERTY(QString, name)
    READONLY_PROPERTY(QString, shortName)
    READONLY_PROPERTY(QString, version)
    READONLY_PROPERTY(QString, author)
    READONLY_PROPERTY(QString, description)
    READONLY_PROPERTY(QString, path)
    AUTO_PROPERTY(bool, enabled)

public:
    explicit Addon(QObject *parent, QString name, QString shortName, QString version, QString author, QString description, QString path);

    static auto fromJson(QObject *parent, const QJsonDocument& json, QString path) -> Addon*;

};


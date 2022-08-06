#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "abstractnamegenerator.h"

class NameGeneratorFactory
{
public:
    static auto buildFromJson(QObject *parent, const QJsonDocument& json) -> QList<AbstractNameGenerator*>;

private:
    static auto buildFromJson(QObject *parent, const QJsonObject& json) -> AbstractNameGenerator*;
};


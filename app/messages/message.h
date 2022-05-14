#pragma once

#include <QObject>
#include <QDateTime>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class Message : public QObject
{
    Q_OBJECT
    READONLY_PROPERTY(QDateTime, timestamp)
    READONLY_PROPERTY(int, type)
    READONLY_PROPERTY(QString, category)
    READONLY_PROPERTY(QString, body)

public:
    explicit Message(QObject *parent, const QDateTime &timestamp, int type, const QString &category, const QString &body)
      : QObject(parent), a_timestamp(timestamp), a_type(type), a_category(category), a_body(body) {}
    ~Message() override = default;
};

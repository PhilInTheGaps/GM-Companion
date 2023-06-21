#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>

class BaseProjectItem : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, name)
    READ_PROPERTY(QString, originalName)
    AUTO_PROPERTY_VAL2(bool, wasRenamed, false)

public:
    explicit BaseProjectItem(const QString &name, QObject *parent = nullptr);

signals:
    void wasEdited();

private slots:
    void onNameChanged(const QString &name);
    void onWasRenamedChanged(bool wasRenamed);
};

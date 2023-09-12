#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class BaseProjectItem : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

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

Q_DECLARE_METATYPE(BaseProjectItem *)

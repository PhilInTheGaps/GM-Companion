#pragma once

#include <QObject>
#include <QStringList>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AbstractNameGenerator : public QObject
{
    Q_OBJECT
public:
    explicit AbstractNameGenerator(QObject *parent, QString name, QStringList categories, QStringList prefixes, QStringList suffixes);

    Q_INVOKABLE virtual bool generate(int count) = 0;
    Q_INVOKABLE bool setCategoryEnabled(int index, bool value);

    READONLY_PROPERTY(QString, name)
    READONLY_PROPERTY(QStringList, categories)
    READONLY_PROPERTY(QStringList, prefixes)
    READONLY_PROPERTY(QStringList, suffixes)

    AUTO_PROPERTY(QList<QStringList>, generatedNames)
    AUTO_PROPERTY(QList<bool>, enabledCategories)
    AUTO_PROPERTY(int, activePrefix)
    AUTO_PROPERTY(int, activeSuffix)

private:
    auto buildEmptyNameList() -> QList<QStringList>;
    auto buildInitialEnabledCategoryList() -> QList<bool>;
};


#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QStringList>

class AbstractNameGenerator : public QObject
{
    Q_OBJECT
public:
    explicit AbstractNameGenerator(QObject *parent, QString name, QStringList categories, QStringList prefixes,
                                   QStringList suffixes);

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
    [[nodiscard]] auto buildEmptyNameList() const -> QList<QStringList>;
    [[nodiscard]] auto buildInitialEnabledCategoryList() const -> QList<bool>;
};

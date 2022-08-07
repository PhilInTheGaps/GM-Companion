#pragma once

#include <QObject>

// I don't exactly know what this include does but if it is not there,
// the application won't compile on some systems.
#include <QQmlContext>

#include "addon.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AddonManager : public QObject
{
    Q_OBJECT
    READ_PROPERTY(QList<Addon*>, addons)

public:
    static auto instance() -> AddonManager*;

    Q_INVOKABLE bool setAddonEnabled(int index, bool value);

private:
    explicit AddonManager(QObject *parent = nullptr);

private:
    void load();
    void loadFromDir(const QString& dir);

    inline static AddonManager* s_instance = nullptr;
};

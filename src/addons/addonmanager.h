#pragma once

#include <QObject>
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

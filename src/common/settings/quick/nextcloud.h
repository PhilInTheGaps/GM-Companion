#pragma once

#include "../settingsmanager.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Common::Settings::Quick
{

class NextCloud : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    using QObject::QObject;

    Q_INVOKABLE static QString getServerUrl()
    {
        return SettingsManager::getServerUrl(GROUP, false);
    }

    Q_INVOKABLE static void setServerUrl(const QString &url)
    {
        SettingsManager::setServerUrl(url, GROUP);
    }

private:
    inline static constexpr auto GROUP = "NextCloud";
};

} // namespace Common::Settings::Quick

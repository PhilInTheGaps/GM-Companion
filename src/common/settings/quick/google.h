#pragma once

#include "../settingsmanager.h"
#include "macros.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Common::Settings::Quick
{

class Google : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    SETTINGS_PROPERTY2(QString, clientId, QLatin1String(), GROUP)
    SETTINGS_PROPERTY2(QString, clientSecret, QLatin1String(), GROUP)

public:
    using QObject::QObject;

private:
    static constexpr auto GROUP = "Google";
};

} // namespace Common::Settings::Quick

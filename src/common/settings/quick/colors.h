#pragma once

#include "../settingsmanager.h"
#include "macros.h"
#include <QColor>
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Common::Settings::Quick
{

class Colors : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    SETTINGS_PROPERTY2(QColor, info, QColor::fromString(INFO_DEFAULT), GROUP)
    SETTINGS_PROPERTY2(QColor, success, QColor::fromString(SUCCESS_DEFAULT), GROUP)
    SETTINGS_PROPERTY2(QColor, warning, QColor::fromString(WARNING_DEFAULT), GROUP)
    SETTINGS_PROPERTY2(QColor, error, QColor::fromString(ERROR_DEFAULT), GROUP)

    Q_INVOKABLE void resetInfo()
    {
        info(INFO_DEFAULT);
    }

    Q_INVOKABLE void resetSuccess()
    {
        success(SUCCESS_DEFAULT);
    }

    Q_INVOKABLE void resetWarning()
    {
        warning(WARNING_DEFAULT);
    }

    Q_INVOKABLE void resetError()
    {
        error(ERROR_DEFAULT);
    }

public:
    using QObject::QObject;

private:
    inline static constexpr auto GROUP = "Colors";
    inline static constexpr auto INFO_DEFAULT = "#00bfff";
    inline static constexpr auto SUCCESS_DEFAULT = "#48b749";
    inline static constexpr auto WARNING_DEFAULT = "#fce208";
    inline static constexpr auto ERROR_DEFAULT = "#ef2426";
};

} // namespace Common::Settings::Quick

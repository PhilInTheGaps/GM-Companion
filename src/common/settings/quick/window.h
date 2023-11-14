#pragma once

#include "../settingsmanager.h"
#include "macros.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Common::Settings::Quick
{

class Window : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    SETTINGS_PROPERTY_VAL(int, width, WIDTH_DEFAULT)
    SETTINGS_PROPERTY_VAL(int, height, HEIGHT_DEFAULT)

    SETTINGS_PROPERTY_VAL(bool, showToolNames, false)

public:
    using QObject::QObject;

private:
    static constexpr int WIDTH_DEFAULT = 1280;
    static constexpr int HEIGHT_DEFAULT = 720;
};

} // namespace Common::Settings::Quick

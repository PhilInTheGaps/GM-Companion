#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

class AudioPlayer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    using QObject::QObject;

    enum class State
    {
        Initialized,
        Playing,
        Paused,
        Stopped,
        Loading
    };
    Q_ENUM(State)

    AUTO_PROPERTY_VAL2(State, state, State::Initialized)

public slots:
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setVolume(int linear, int logarithmic) = 0;
    virtual void again() = 0;
    virtual void next(bool withError) = 0;

protected:
    static auto normalizeVolume(int volume) -> float;
};

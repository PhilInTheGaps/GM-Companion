#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

class AudioPlayer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    using QObject::QObject;

public slots:
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setVolume(int linear, int logarithmic) = 0;
    virtual void again() = 0;
    virtual void next() = 0;

protected:
    static auto normalizeVolume(int volume) -> float;
};

#pragma once

#include <QObject>

class AudioPlayer : public QObject
{
    Q_OBJECT
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

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer(QObject *parent = nullptr) : QObject(parent) {}
    AudioPlayer() = default;

public slots:
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setVolume(int linear, int logarithmic) = 0;
    virtual void again() = 0;
    virtual void next() = 0;

};

#endif // AUDIOPLAYER_H

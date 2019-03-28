#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include "src/managers/filemanager.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer(FileManager *fManager, QObject *parent = nullptr) {}
    AudioPlayer() {}
    virtual ~AudioPlayer() {}

    virtual void play() {}
    virtual void pause() {}
    virtual void stop() {}
    virtual void setVolume(int volume) {}
    virtual void setVolume(float volume) {}
    virtual void again() {}
    virtual void next() {}
    virtual void setIndex(int index) {}

protected:
    FileManager *fileManager;
};

#endif // AUDIOPLAYER_H

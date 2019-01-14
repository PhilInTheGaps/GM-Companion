#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "../audioelement.h"
#include "audioplayer.h"

class SoundPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    explicit SoundPlayer(FileManager *fManager);

    void play(SoundElement* element);
    void stop(QString element);
    void setVolume(float volume);

private:
    QList<QMediaPlayer*> players;
    QList<SoundElement*> elements;

    float m_volume;
    bool isSoundPlaying(SoundElement* element);
    void removeElement(QString element);

signals:
    void soundsChanged(QList<SoundElement*>);

public slots:
    void onSoundPathsChanged(QList<QUrl> urls, QStringList args);
};

#endif // SOUNDPLAYER_H

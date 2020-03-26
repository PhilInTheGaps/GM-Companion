#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "../audioelement.h"
#include "audioplayer.h"
#include "youtube.h"

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
    YouTube youtube;

    QMap<int, QMediaPlayer*> m_ytRequestMap;

    float m_volume;
    bool isSoundPlaying(SoundElement* element);
    void removeElement(QString element);

private slots:
    void onYtReceivedVideoMediaStreamInfos(MediaStreamInfoSet *infos, int requestId);

signals:
    void soundsChanged(QList<SoundElement*>);
};

#endif // SOUNDPLAYER_H

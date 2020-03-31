#ifndef RADIOPLAYER_H
#define RADIOPLAYER_H

#include <QBuffer>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "../audioelement.h"
#include "audioplayer.h"

class RadioPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    RadioPlayer(QObject *parent = nullptr);
    ~RadioPlayer();

    void play(AudioElement *element);
    void play();
    void pause();
    void stop();
    void setLogarithmicVolume(int volume) { player->setVolume(volume); }
    void setLinearVolume(int volume) { }
    void again() { }
    void next() { }
    void setIndex(int index) {}

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    AudioElement *currentElement;
    int m_fileRequestId = 0;

    QByteArray m_mediaData;
    QBuffer *m_mediaBuffer = nullptr;

signals:
    void startedPlaying();
    void metaDataChanged(QMediaPlayer *mediaPlayer, QPixmap elementIcon);

private slots:
    void onMetaDataChanged();
    void onFileReceived(int id, QByteArray data);
};

#endif // RADIOPLAYER_H

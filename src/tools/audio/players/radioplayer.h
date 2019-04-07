#ifndef RADIOPLAYER_H
#define RADIOPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "../audioelement.h"
#include "audioplayer.h"

class RadioPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    explicit RadioPlayer(FileManager *fManager);

    void play(RadioElement *element);
    void play();
    void pause();
    void stop();
    void setVolume(float volume) { player->setVolume(volume); }

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    RadioElement *currentElement;

signals:
    void startedPlaying();
    void metaDataChanged(QMediaPlayer *mediaPlayer, QString elementIcon);

public slots:
    void onUrlChanged(QUrl url);

private slots:
    void onMetaDataChanged();
};

#endif // RADIOPLAYER_H

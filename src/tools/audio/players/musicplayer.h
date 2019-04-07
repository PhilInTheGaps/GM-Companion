#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "../audioelement.h"
#include "audioplayer.h"

class MusicPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    explicit MusicPlayer(FileManager *fManager);
    ~MusicPlayer();

    void play(MusicElement *element);
    void play();
    void pause();
    void stop();
    void setVolume(float volume) { player->setVolume(volume); }
    void next() { playlist->next(); }
    void again() { player->setPosition(0); }
    void setIndex(int index) { playlist->setCurrentIndex(index); player->play(); }

    QStringList songNames() const { return m_songNames; }
    int index() const { return playlist->currentIndex(); }

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    MusicElement *currentElement;

    QStringList m_songNames;
    bool m_waitingForUrls;

signals:
    void startedPlaying();
    void metaDataChanged(QMediaPlayer *mediaPlayer, QString elementIcon);
    void songNamesChanged();

public slots:
    void onSongPathsChanged(QList<QUrl> urls);

private slots:
    void onMetaDataChanged();
};

#endif // MUSICPLAYER_H

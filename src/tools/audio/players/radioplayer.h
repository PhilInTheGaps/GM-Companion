#ifndef RADIOPLAYER_H
#define RADIOPLAYER_H

#include <QBuffer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTemporaryDir>

#include "../metadata/metadatareader.h"
#include "../project/audioelement.h"
#include "audioplayer.h"
#include "discordplayer.h"

class RadioPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    RadioPlayer(MetaDataReader *metaDataReader, DiscordPlayer *m_discordPlayer,
                QObject *parent = nullptr);

    void play(AudioElement *element);
    void play();
    void pause();
    void stop();
    void setLogarithmicVolume(int volume) { m_mediaPlayer->setVolume(volume); }
    void setLinearVolume(int volume) { }
    void again() { }
    void next() { }
    void setIndex(int index) {}

private:
    QMediaPlayer *m_mediaPlayer = nullptr;
    QMediaPlaylist *m_playlist = nullptr;
    DiscordPlayer *m_discordPlayer = nullptr;

    AudioElement *m_currentElement = nullptr;
    int m_fileRequestId = -1;

    QBuffer m_mediaBuffer;
    QTemporaryDir m_tempDir;
    QString m_fileName;

signals:
    void startedPlaying();
    void metaDataChanged(QMediaPlayer *mediaPlayer);

private slots:
    void onMetaDataChanged();
    void onFileReceived(int id, const QByteArray& data);
};

#endif // RADIOPLAYER_H

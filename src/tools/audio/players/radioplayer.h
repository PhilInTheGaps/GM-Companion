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
#include "filesystem/file.h"

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
    QObject *m_fileRequestContext = nullptr;

    AudioElement *m_currentElement = nullptr;

    QBuffer m_mediaBuffer;
    QTemporaryDir m_tempDir;
    QString m_fileName;

signals:
    void startedPlaying();
    void metaDataChanged(QMediaPlayer *mediaPlayer);

private slots:
    void onMetaDataChanged();
    void onFileReceived(Files::FileDataResult *result);
};

#endif // RADIOPLAYER_H

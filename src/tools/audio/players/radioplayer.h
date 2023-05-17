#ifndef RADIOPLAYER_H
#define RADIOPLAYER_H

#include <QBuffer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTemporaryDir>

#include "../metadata/metadatareader.h"
#include "../project/audioelement.h"
#include "audioplayer.h"
#include "filesystem/file.h"

class RadioPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    RadioPlayer(MetaDataReader *metaDataReader, QObject *parent = nullptr);

public slots:
    void play(AudioElement *element);
    void play() override;
    void pause() override;
    void stop() override;
    void setVolume(int linear, int logarithmic) override;
    void again() override
    {
    }
    void next() override
    {
    }

private:
    QMediaPlayer *m_mediaPlayer = nullptr;
    QMediaPlaylist *m_playlist = nullptr;
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

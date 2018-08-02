#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QtNetworkAuth>
#include <QQueue>

#include "gm-companion/settings/settingsmanager.h"

struct Playlist
{
    QString id;
    int index;
};

class Spotify : public QObject
{
    Q_OBJECT
public:
    explicit Spotify(QObject *parent = nullptr);
    ~Spotify() {}

    void grant() { m_spotify.grant(); }
    bool isGranted() const { return m_isGranted; }

    void play(QString id);
    void play();
    void stop();
    void pausePlay();

    void setIndex(int index);
    void next();
    void again();

    void setVolume(int volume);
    bool isPlaying() const { return m_isPlaying; }

    void fetchIcon(QString id, int index);

private:
    QOAuth2AuthorizationCodeFlow m_spotify;
    SettingsManager m_sManager;

    QQueue<Playlist> iconFetchQueue;

    bool m_isGranted = false;
    QString m_tempId;
    bool m_isPlaying = false;
    int m_volume;
    int m_afterGranted = 0;

    void fetchQueuedIcons();

signals:
    void iconChanged(int index, QString url);

private slots:
    void authStatusChanged(QAbstractOAuth::Status status);
    void granted();
};



#endif // SPOTIFY_H

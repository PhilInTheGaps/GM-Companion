#ifndef DISCORDPLAYER_H
#define DISCORDPLAYER_H

#include <QObject>
#include "settings/settingsmanager.h"
#include "services/discord/discord.h"

class DiscordPlayer : public QObject
{
    Q_OBJECT
public:
    explicit DiscordPlayer(QObject *parent = nullptr);

    void play();
    void pause();

    void setMusicVolume(int volume);
    void setSoundsVolume(int volume);

    void playMusic(const QByteArray& data);
    void playMusic(const QString& url);
    void playRadioPlaylist(const QByteArray& data);

    void playSound(const QString& id, const QByteArray& data);
    void playSound(const QString& url);

    void stopSound(const QString& id);

private:
    static float percentVolume(int volume) { return volume / 100.0f; }

    void sendData(const QString& url, const QByteArray& data);
    void sendUrl(const QString& url);

};

#endif // DISCORDPLAYER_H

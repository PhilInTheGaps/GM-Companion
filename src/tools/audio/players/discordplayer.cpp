#include "discordplayer.h"
#include "logging.h"

#include <QUrl>

DiscordPlayer::DiscordPlayer(QObject *parent) : QObject(parent)
{

}

void DiscordPlayer::play()
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Playing";
    auto url = "/%1/play";
    sendUrl(url);
}

void DiscordPlayer::pause()
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Pausing";
    auto url = "/%1/pause";
    sendUrl(url);
}

void DiscordPlayer::setMusicVolume(int volume)
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Setting music volume:" << volume;
    auto url = "/%1/volume/?music=" + QUrl::toPercentEncoding(QString::number(DiscordPlayer::percentVolume(volume)));
    sendUrl(url);
}

void DiscordPlayer::setSoundsVolume(int volume)
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Setting sounds volume:" << volume;
    auto url = "/%1/volume/?sounds=" + QUrl::toPercentEncoding(QString::number(DiscordPlayer::percentVolume(volume)));
    sendUrl(url);
}

void DiscordPlayer::playMusic(const QByteArray &data)
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Playing music from data ...";
    auto url = "/%1/play/music/data";
    sendData(url, data);
}

void DiscordPlayer::playMusic(const QString &url)
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Playing music from url:" << url;
    auto finalUrl = "/%1/play/music/url?url=" + QUrl::toPercentEncoding(url);
    sendUrl(finalUrl);
}

void DiscordPlayer::playRadioPlaylist(const QByteArray &data)
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Playing radio from playlist ...";
    auto url = "/%1/play/music/playlist";
    sendData(url, data);
}

void DiscordPlayer::playSound(const QString &id, const QByteArray &data)
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Playing sound from data ...";
    auto url = "/%1/play/sound/data?id=" + QUrl::toPercentEncoding(id);
    sendData(url, data);
}

void DiscordPlayer::playSound(const QString &url)
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Playing sound from url:" << url;
    auto finalUrl = "/%1/play/sound/url?url=" + QUrl::toPercentEncoding(url);
    sendUrl(finalUrl);
}

void DiscordPlayer::stopSound(const QString &id)
{
    if (!Discord::getInstance()->enabled()) return;

    qCDebug(gmAudioDiscord()) << "Stopping sound:" << id;
    auto url = "/%1/stop/sound?id=" + QUrl::toPercentEncoding(id);
    sendUrl(url);
}

void DiscordPlayer::sendData(const QString &url, const QByteArray &data)
{
    auto channelId = Discord::getInstance()->channel();

    if (channelId.isEmpty())
    {
        qCWarning(gmAudioDiscord()) << "Error: Discord channel ID is empty.";
        return;
    }

    if (url.isEmpty())
    {
        qCWarning(gmAudioDiscord()) << "Error: Url is empty.";
        return;
    }

    auto *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart dataPart;
    dataPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\"; filename=\"audio\""));
    dataPart.setBody(data);
    multiPart->append(dataPart);

    Discord::getInstance()->post(url.arg(channelId), multiPart);
}

void DiscordPlayer::sendUrl(const QString &url)
{
    auto channelId = Discord::getInstance()->channel();

    if (channelId.isEmpty())
    {
        qCWarning(gmAudioDiscord()) << "Error: Discord channel ID is empty.";
        return;
    }

    if (url.isEmpty())
    {
        qCWarning(gmAudioDiscord()) << "Error: Url is empty.";
        return;
    }

    Discord::getInstance()->post(url.arg(channelId), "");
}





#ifndef NO_DBUS
#include "mprismanager.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMetaType>
#include <QDBusObjectPath>
#include <QStringList>
#include <utility>

using namespace Qt::Literals::StringLiterals;

MprisManager::MprisManager(QObject *parent)
    : QObject(parent), m_mprisAdaptor(new MprisAdaptor(this)), m_mprisPlayerAdaptor(new MprisPlayerAdaptor(this))
{
    connect(m_mprisPlayerAdaptor, &MprisPlayerAdaptor::next, this, &MprisManager::next);
    connect(m_mprisPlayerAdaptor, &MprisPlayerAdaptor::changeVolume, this, &MprisManager::changeVolume);
    connect(m_mprisPlayerAdaptor, &MprisPlayerAdaptor::pause, this, &MprisManager::pause);
    connect(m_mprisPlayerAdaptor, &MprisPlayerAdaptor::previous, this, &MprisManager::previous);
    connect(m_mprisPlayerAdaptor, &MprisPlayerAdaptor::playPause, this, &MprisManager::playPause);
    connect(m_mprisPlayerAdaptor, &MprisPlayerAdaptor::stop, this, &MprisManager::stop);
    connect(m_mprisPlayerAdaptor, &MprisPlayerAdaptor::play, this, &MprisManager::play);

    QDBusConnection::sessionBus().registerObject(u"/org/mpris/MediaPlayer2"_s, this);
    QDBusConnection::sessionBus().registerService(u"org.mpris.MediaPlayer2.gm_companion"_s);

    m_metaData[u"mpris:trackid"_s] =
        QVariant::fromValue(QDBusObjectPath(u"/lol/rophil/gm_companion/audio/current_track"_s));
    sendUpdatedMetadata();
}

void MprisManager::setPlaybackStatus(AudioPlayer::State status)
{
    m_mprisPlayerAdaptor->setPlaybackStatus(status);
    sendMprisUpdateSignal(u"PlaybackStatus"_s, m_mprisPlayerAdaptor->playbackStatus());
}

void MprisManager::setVolume(double volume) const
{
    m_mprisPlayerAdaptor->setVolume(volume);
}

void MprisManager::setMetaDataReader(MetaDataReader *reader)
{
    connect(reader->metaData(), &AudioMetaData::titleChanged, this, [this](const QString &title) {
        m_metaData[u"xesam:title"_s] = title;
        sendUpdatedMetadata();
    });

    connect(reader->metaData(), &AudioMetaData::artistChanged, this, [this](const QStringList &artist) {
        m_metaData[u"xesam:artist"_s] = artist;
        m_metaData[u"xesam:albumArtist"_s] = artist;
        sendUpdatedMetadata();
    });

    connect(reader->metaData(), &AudioMetaData::albumChanged, this, [this](const QString &album) {
        m_metaData[u"xesam:album"_s] = album;
        sendUpdatedMetadata();
    });

    connect(reader->metaData(), &AudioMetaData::coverChanged, this, [this](const QString &cover) {
        m_metaData[u"mpris:artUrl"_s] = cover;
        sendUpdatedMetadata();
    });

    connect(reader->metaData(), &AudioMetaData::lengthChanged, this, [this](qint64 length) {
        m_metaData[u"mpris:length"_s] = length;
        sendUpdatedMetadata();
    });
}

/**
 * @brief Send a PropertiesChanged notification via dbus
 * @param property Name of the property
 * @param value Value of the property
 */
void MprisManager::sendMprisUpdateSignal(const QString &property, const QVariant &value) const
{
    QDBusMessage signal = QDBusMessage::createSignal(u"/org/mpris/MediaPlayer2"_s, u"org.freedesktop.DBus.Properties"_s,
                                                     u"PropertiesChanged"_s);

    signal << u"org.mpris.MediaPlayer2.Player"_s;

    QVariantMap changedProps;
    changedProps.insert(property, value);
    signal << changedProps;

    QStringList invalidatedProps;
    signal << invalidatedProps;

    QDBusConnection::sessionBus().send(signal);
}

void MprisManager::sendUpdatedMetadata()
{
    m_mprisPlayerAdaptor->setMetadata(m_metaData);
    sendMprisUpdateSignal(u"Metadata"_s, m_metaData);
}

#endif

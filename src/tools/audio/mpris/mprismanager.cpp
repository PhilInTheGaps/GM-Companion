#include "mprismanager.h"

#ifndef NO_DBUS
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMetaType>
#include <QDBusObjectPath>
#include <QStringList>
#include <utility>
#endif

using namespace Qt::Literals::StringLiterals;

MprisManager::MprisManager(QObject *parent) : QObject(parent)
{
#ifndef NO_DBUS
    mprisAdaptor = new MprisAdaptor(this);
    mprisPlayerAdaptor = new MprisPlayerAdaptor(this);

    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::next, this, &MprisManager::next);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::changeVolume, this, &MprisManager::changeVolume);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::pause, this, &MprisManager::pause);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::previous, this, &MprisManager::previous);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::playPause, this, &MprisManager::playPause);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::stop, this, &MprisManager::stop);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::play, this, &MprisManager::play);

    QDBusConnection::sessionBus().registerObject(u"/org/mpris/MediaPlayer2"_s, this);
    QDBusConnection::sessionBus().registerService(u"org.mpris.MediaPlayer2.gm_companion"_s);
#endif
}

void MprisManager::setPlaybackStatus(int status)
{
#ifndef NO_DBUS
    mprisPlayerAdaptor->setPlaybackStatus(status);
    sendMprisUpdateSignal(u"PlaybackStatus"_s, mprisPlayerAdaptor->playbackStatus());
#endif
}

void MprisManager::setVolume(double volume)
{
#ifndef NO_DBUS
    mprisPlayerAdaptor->setVolume(volume);
#endif
}

void MprisManager::updateMetaData(AudioMetaData *metaData)
{
#ifndef NO_DBUS
    QMap<QString, QVariant> map;
    map.insert(u"mpris:trackid"_s,
               QVariant::fromValue(QDBusObjectPath(u"/lol/rophil/gm_companion/audio/current_track"_s)));
    map.insert(u"mpris:length"_s, metaData->length());
    map.insert(u"mpris:artUrl"_s, metaData->cover());
    map.insert(u"xesam:album"_s, metaData->album().isEmpty() ? tr("Unknown Album") : metaData->album());
    map.insert(u"xesam:albumArtist"_s,
               metaData->artist().isEmpty() ? QStringList({tr("Unknown Artist")}) : QStringList({metaData->artist()}));
    map.insert(u"xesam:artist"_s,
               metaData->artist().isEmpty() ? QStringList({tr("Unknown Artist")}) : QStringList({metaData->artist()}));
    map.insert(u"xesam:title"_s, metaData->title().isEmpty() ? tr("Unknown Title") : metaData->title());

    mprisPlayerAdaptor->setMetadata(map);
    sendMprisUpdateSignal(u"Metadata"_s, map);
#endif
}

/**
 * @brief Send a PropertiesChanged notification via dbus
 * @param property Name of the property
 * @param value Value of the property
 */
void MprisManager::sendMprisUpdateSignal(const QString &property, const QVariant &value) const
{
#ifndef NO_DBUS
    QDBusMessage signal = QDBusMessage::createSignal(u"/org/mpris/MediaPlayer2"_s, u"org.freedesktop.DBus.Properties"_s,
                                                     u"PropertiesChanged"_s);

    signal << u"org.mpris.MediaPlayer2.Player"_s;

    QVariantMap changedProps;
    changedProps.insert(property, value);
    signal << changedProps;

    QStringList invalidatedProps;
    signal << invalidatedProps;

    QDBusConnection::sessionBus().send(signal);
#endif
}

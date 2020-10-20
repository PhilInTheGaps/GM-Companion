#include "mprismanager.h"

#ifndef NO_DBUS
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QDBusMetaType>
#include <QStringList>
#include <utility>
#endif

MprisManager::MprisManager(QObject *parent) : QObject(parent)
{
#ifndef NO_DBUS
    mprisAdaptor       = new MprisAdaptor(this);
    mprisPlayerAdaptor = new MprisPlayerAdaptor(this);

    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::next,         this, &MprisManager::next);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::changeVolume, this, &MprisManager::changeVolume);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::pause,        this, &MprisManager::pause);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::previous,     this, &MprisManager::previous);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::playPause,    this, &MprisManager::playPause);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::stop,         this, &MprisManager::stop);
    connect(mprisPlayerAdaptor, &MprisPlayerAdaptor::play,         this, &MprisManager::play);

    QDBusConnection::sessionBus().registerObject("/org/mpris/MediaPlayer2", this);
    QDBusConnection::sessionBus().registerService("org.mpris.MediaPlayer2.gm_companion");
#endif
}

void MprisManager::setPlaybackStatus(int status)
{
#ifndef NO_DBUS
    mprisPlayerAdaptor->setPlaybackStatus(status);
    sendMprisUpdateSignal("PlaybackStatus", mprisPlayerAdaptor->playbackStatus());
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
    map.insert("mpris:trackid",     QVariant::fromValue(QDBusObjectPath("/lol/rophil/gm_companion/audio/current_track")));
    map.insert("mpris:length",      metaData->length());
    map.insert("mpris:artUrl",      metaData->cover());
    map.insert("xesam:album",       metaData->album().isEmpty() ? tr("Unknown Album") : metaData->album());
    map.insert("xesam:albumArtist", metaData->artist().isEmpty() ? QStringList({ tr("Unknown Artist") }) : QStringList({ metaData->artist() }));
    map.insert("xesam:artist",      metaData->artist().isEmpty() ? QStringList({ tr("Unknown Artist") }) : QStringList({ metaData->artist() }));
    map.insert("xesam:title",       metaData->title().isEmpty() ? tr("Unknown Title") : metaData->title());

    mprisPlayerAdaptor->setMetadata(map);
    sendMprisUpdateSignal("Metadata", map);
#endif
}

/**
 * @brief Send a PropertiesChanged notification via dbus
 * @param property Name of the property
 * @param value Value of the property
 */
void MprisManager::sendMprisUpdateSignal(const QString &property, const QVariant &value)
{
#ifndef NO_DBUS
    QDBusMessage signal = QDBusMessage::createSignal(
        "/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged");

    signal << "org.mpris.MediaPlayer2.Player";

    QVariantMap changedProps;
    changedProps.insert(property, value);
    signal << changedProps;

    QStringList invalidatedProps;
    signal << invalidatedProps;

    QDBusConnection::sessionBus().send(signal);
#endif
}

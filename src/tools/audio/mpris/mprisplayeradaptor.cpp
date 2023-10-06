#ifndef NO_DBUS
#include "mprisplayeradaptor.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmMprisPlayer, "gm.mpris.player")

void MprisPlayerAdaptor::setPlaybackStatus(AudioPlayer::State status)
{
    switch (status)
    {
    case AudioPlayer::State::Playing:
        m_PlaybackStatus = u"Playing"_s;
        break;

    case AudioPlayer::State::Paused:
        m_PlaybackStatus = u"Paused"_s;
        break;

    default:
        m_PlaybackStatus = u"Stopped"_s;
        break;
    }

    emit playbackStatusChanged(m_PlaybackStatus);
}

void MprisPlayerAdaptor::setMetadata(const QHash<QString, QVariant> &data)
{
    qCDebug(gmMprisPlayer()) << "Updating mpris metadata ...";

    m_Metadata = data;
    emit metadataChanged(m_Metadata);
}

void MprisPlayerAdaptor::Seek(qlonglong /*Offset*/) const
{
    // Not implemented
}

void MprisPlayerAdaptor::SetPosition(const QDBusObjectPath & /*TrackId*/, qlonglong /*Position*/) const
{
    // Not implemented
}

void MprisPlayerAdaptor::OpenUri(const QString & /*Uri*/) const
{
    // Not implemented
}

#endif

#include "mprisplayeradaptor.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmMprisPlayer, "gm.mpris.player")

void MprisPlayerAdaptor::setPlaybackStatus(int status)
{
    switch (status)
    {
    case 0:
        m_PlaybackStatus = QStringLiteral("Stopped");
        break;

    case 1:
        m_PlaybackStatus = QStringLiteral("Playing");
        break;

    case 2:
        m_PlaybackStatus = QStringLiteral("Paused");
        break;
    }

    emit playbackStatusChanged(m_PlaybackStatus);
}

void MprisPlayerAdaptor::setMetadata(const QMap<QString, QVariant> &data)
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

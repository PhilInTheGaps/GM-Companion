#include "mprisplayeradaptor.h"
#include <QDebug>
#include <QImage>
#include <QUrl>

void MprisPlayerAdaptor::setPlaybackStatus(int status)
{
    switch (status)
    {
    case 0: m_PlaybackStatus = "Stopped";
        break;

    case 1: m_PlaybackStatus = "Playing";
        break;

    case 2: m_PlaybackStatus = "Paused";
        break;
    }

    emit playbackStatusChanged(m_PlaybackStatus);
}

void MprisPlayerAdaptor::setMetadata(const QMap<QString, QVariant>& data)
{
    qDebug() << "Updating mpris metadata ...";

    m_Metadata = data;
    emit metadataChanged(m_Metadata);
}

void MprisPlayerAdaptor::Seek(qlonglong /*Offset*/)
{
    // Not implemented
}

void MprisPlayerAdaptor::SetPosition(const QDBusObjectPath& /*TrackId*/, const qlonglong& /*Position*/)
{
    // Not implemented
}

void MprisPlayerAdaptor::OpenUri(const QString& /*Uri*/)
{
    // Not implemented
}

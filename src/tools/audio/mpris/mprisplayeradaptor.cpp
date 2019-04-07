#include "mprisplayeradaptor.h"

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

void MprisPlayerAdaptor::Seek(qlonglong Offset)
{
    // Not implemented
}

void MprisPlayerAdaptor::SetPosition(QDBusObjectPath TrackId, qlonglong Position)
{
    // Not implemented
}

void MprisPlayerAdaptor::OpenUri(QString Uri)
{
    // Not implemented
}

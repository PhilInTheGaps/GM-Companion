#pragma once

#if WITH_GOOGLE_DRIVE
#include "google.h"
#endif

#include "nextcloud.h"
#include "spotify.h"
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace Common::Settings::Quick
{

class Services : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(Common::Settings::Quick::Spotify *spotify READ spotify CONSTANT FINAL)
    Q_PROPERTY(Common::Settings::Quick::NextCloud *nextcloud READ nextcloud CONSTANT FINAL)

#if WITH_GOOGLE_DRIVE
    Q_PROPERTY(Common::Settings::Quick::Google *google READ google CONSTANT FINAL)
#endif

public:
    using QObject::QObject;

    auto spotify() -> Spotify *
    {
        return &m_spotify;
    }

#if WITH_GOOGLE_DRIVE
    auto google() -> Google *
    {
        return &m_google;
    }
#endif

    auto nextcloud() -> NextCloud *
    {
        return &m_nextcloud;
    }

private:
    Spotify m_spotify;
    NextCloud m_nextcloud;

#if WITH_GOOGLE_DRIVE
    Google m_google;
#endif
};

} // namespace Common::Settings::Quick

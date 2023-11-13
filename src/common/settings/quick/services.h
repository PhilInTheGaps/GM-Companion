#pragma once

#include "google.h"
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
    Q_PROPERTY(Common::Settings::Quick::Google *google READ google CONSTANT FINAL)
    Q_PROPERTY(Common::Settings::Quick::NextCloud *nextcloud READ nextcloud CONSTANT FINAL)

public:
    using QObject::QObject;

    auto spotify() -> Spotify *
    {
        return &m_spotify;
    }

    auto google() -> Google *
    {
        return &m_google;
    }

    auto nextcloud() -> NextCloud *
    {
        return &m_nextcloud;
    }

private:
    Spotify m_spotify;
    Google m_google;
    NextCloud m_nextcloud;
};

} // namespace Common::Settings::Quick

#pragma once

#include "o2spotify.h"
#include "rest/restserviceconnectorlocal.h"

class SpotifyConnectorLocal : public RESTServiceConnectorLocal
{
    Q_OBJECT
public:
    SpotifyConnectorLocal(QNetworkAccessManager &networkManager, O2Spotify *o2Spotify, QObject *parent = nullptr);
};

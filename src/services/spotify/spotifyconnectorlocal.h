#ifndef SPOTIFYCONNECTORLOCAL_H
#define SPOTIFYCONNECTORLOCAL_H

#include "../rest/restserviceconnectorlocal.h"
#include "o2spotify.h"

class SpotifyConnectorLocal : public RESTServiceConnectorLocal
{
    Q_OBJECT
public:
    SpotifyConnectorLocal
    (QNetworkAccessManager* networkManager, O2Spotify *o2Spotify, QObject *parent = nullptr);
};

#endif // SPOTIFYCONNECTORLOCAL_H

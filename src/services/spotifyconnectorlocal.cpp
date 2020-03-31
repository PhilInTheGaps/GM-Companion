#include "spotifyconnectorlocal.h"
#include "logging.h"
#include "services.h"

SpotifyConnectorLocal::SpotifyConnectorLocal
    (QNetworkAccessManager *networkManager, O2Spotify *o2Spotify, QObject *parent) :
    RESTServiceConnectorLocal(networkManager, o2Spotify, gmSpotifyLocal(), parent)
{
    RESTServiceLocalConfig config;

    config.scope                 = SPOTIFY_SCOPE;
    config.port                  = SPOTIFY_LOCAL_PORT;
    config.maxConcurrentRequests = SPOTIFY_MAX_REQUESTS;
    config.idRequest             = SettingRequest("spotifyID", "", "Spotify");
    config.secretRequest         = SettingRequest("spotifySecret", "", "Spotify");

    setConfig(config);
}

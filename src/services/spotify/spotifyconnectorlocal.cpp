#include "spotifyconnectorlocal.h"
#include "config.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmSpotifyLocal, "gm.service.spotify.local")

SpotifyConnectorLocal::SpotifyConnectorLocal
    (QNetworkAccessManager *networkManager, O2Spotify *o2Spotify, QObject *parent) :
    RESTServiceConnectorLocal(networkManager, o2Spotify, gmSpotifyLocal(), parent)
{
    RESTServiceLocalConfig config;

    config.scope                 = SCOPE;
    config.port                  = LOCAL_PORT;
    config.maxConcurrentRequests = MAX_REQUESTS;
    config.idRequest             = SettingRequest<QString>(QStringLiteral("spotifyID"), QLatin1String(), QStringLiteral("Spotify"));
    config.secretRequest         = SettingRequest<QString>(QStringLiteral("spotifySecret"), QLatin1String(), QStringLiteral("Spotify"));

    setConfig(config);
}

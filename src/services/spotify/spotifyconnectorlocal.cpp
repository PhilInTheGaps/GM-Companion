#include "spotifyconnectorlocal.h"
#include "config.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

Q_LOGGING_CATEGORY(gmSpotifyLocal, "gm.service.spotify.local")

SpotifyConnectorLocal::SpotifyConnectorLocal(QNetworkAccessManager &networkManager, O2Spotify *o2Spotify,
                                             QObject *parent)
    : RESTServiceConnectorLocal(networkManager, o2Spotify, gmSpotifyLocal(), parent)
{
    RESTServiceLocalConfig config;

    config.scope = SCOPE;
    config.port = LOCAL_PORT;
    config.maxConcurrentRequests = MAX_REQUESTS;
    config.idRequest = SettingRequest<QString>(u"spotifyID"_s, u""_s, u"Spotify"_s);
    config.secretRequest = SettingRequest<QString>(u"spotifySecret"_s, u""_s, u"Spotify"_s);

    setConfig(config);
}

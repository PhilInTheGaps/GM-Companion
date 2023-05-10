#include "googledriveconnectorlocal.h"
#include "logging.h"
#include "settings/settingsmanager.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>

constexpr auto LOCAL_PORT = 59993;
constexpr auto MAX_CONCURRENT_REQUESTS = 5;
constexpr auto SCOPE = "https://www.googleapis.com/auth/drive";

GoogleDriveConnectorLocal::GoogleDriveConnectorLocal(const QString &serviceName, QNetworkAccessManager &networkManager,
                                                     O2 *o2, QObject *parent)
    : RESTServiceConnectorLocal(networkManager, o2, gmGoogleDriveLocal(), parent)
{
    RESTServiceLocalConfig config;

    config.scope = SCOPE;
    config.port = LOCAL_PORT;
    config.maxConcurrentRequests = MAX_CONCURRENT_REQUESTS;
    config.authHeaderFormat = QStringLiteral("Bearer %1");
    config.idRequest = SettingRequest<QString>(QStringLiteral("googleID"), QLatin1String(), serviceName);
    config.secretRequest = SettingRequest<QString>(QStringLiteral("googleSecret"), QLatin1String(), serviceName);

    setConfig(config);

    o2->setExtraRequestParams({{"access_type", "offline"}});
}

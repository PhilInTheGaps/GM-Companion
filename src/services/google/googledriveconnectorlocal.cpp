#include "googledriveconnectorlocal.h"
#include "logging.h"
#include "settings/settingsmanager.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>

GoogleDriveConnectorLocal::GoogleDriveConnectorLocal(
    QNetworkAccessManager *networkManager, O2Google *o2Google, QObject *parent) :
    RESTServiceConnectorLocal(networkManager, o2Google, gmGoogleDriveLocal(), parent)
{
    RESTServiceLocalConfig config;

    config.scope                 = SCOPE;
    config.port                  = LOCAL_PORT;
    config.maxConcurrentRequests = MAX_CONCURRENT_REQUESTS;
    config.authHeaderFormat      = QStringLiteral("Bearer %1");
    config.idRequest             = SettingRequest(QStringLiteral("googleID"), QLatin1String(), QStringLiteral("Google"));
    config.secretRequest         = SettingRequest(QStringLiteral("googleSecret"), QLatin1String(), QStringLiteral("Google"));

    setConfig(config);

    o2Google->setExtraRequestParams({ { "access_type", "offline" } });
}

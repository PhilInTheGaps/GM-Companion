#include "googledriveconnectorlocal.h"
#include "../services.h"
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

    config.scope                 = GOOGLE_SCOPE;
    config.port                  = GOOGLE_LOCAL_PORT;
    config.maxConcurrentRequests = GOOGLE_MAX_REQUESTS;
    config.authHeaderFormat      = "Bearer %1";
    config.idRequest             = SettingRequest("googleID", "", "Google");
    config.secretRequest         = SettingRequest("googleSecret", "", "Google");

    setConfig(config);

    o2Google->setExtraRequestParams({ { "access_type", "offline" } });
}

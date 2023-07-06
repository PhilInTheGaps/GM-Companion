#include "googledriveconnectorlocal.h"
#include "settings/settingsmanager.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

constexpr auto LOCAL_PORT = 59993;
constexpr auto MAX_CONCURRENT_REQUESTS = 5;
constexpr auto SCOPE = "https://www.googleapis.com/auth/drive";

Q_LOGGING_CATEGORY(gmGoogleDriveLocal, "gm.service.google.drive.local")

GoogleDriveConnectorLocal::GoogleDriveConnectorLocal(const QString &serviceName, QNetworkAccessManager &networkManager,
                                                     O2 *o2, QObject *parent)
    : RESTServiceConnectorLocal(networkManager, o2, gmGoogleDriveLocal(), parent)
{
    RESTServiceLocalConfig config;

    config.scope = SCOPE;
    config.port = LOCAL_PORT;
    config.maxConcurrentRequests = MAX_CONCURRENT_REQUESTS;
    config.authHeaderFormat = u"Bearer %1"_s;
    config.idRequest = SettingRequest<QString>(u"googleID"_s, u""_s, serviceName);
    config.secretRequest = SettingRequest<QString>(u"googleSecret"_s, u""_s, serviceName);

    setConfig(config);

    o2->setExtraRequestParams({{"access_type", "offline"}});
}

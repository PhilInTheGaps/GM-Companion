#include "googledriveconnectorlocal.h"
#include "settings/abstractsettingsmanager.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;
using namespace Services;
using namespace Common::Settings;

constexpr auto LOCAL_PORT = 59993;
constexpr auto MAX_CONCURRENT_REQUESTS = 5;
constexpr auto SCOPE = "https://www.googleapis.com/auth/drive.file"; //"https://www.googleapis.com/auth/drive.appdata "

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
    config.idRequest = Request<QString>(u"clientId"_s, u""_s, serviceName);
    config.secretRequest = Request<QString>(u"clientSecret"_s, u""_s, serviceName);

    setConfig(config);

    QVariantMap extraRequestParams = {{"access_type", "offline"}, {"prompt", "consent"}};
    o2->setExtraRequestParams(extraRequestParams);
}

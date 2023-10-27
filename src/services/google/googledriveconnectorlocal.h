#pragma once

#include "o2.h"
#include "rest/restserviceconnectorlocal.h"

namespace Services
{

class GoogleDriveConnectorLocal : public RESTServiceConnectorLocal
{
    Q_OBJECT
public:
    explicit GoogleDriveConnectorLocal(const QString &serviceName, QNetworkAccessManager &networkManager, O2 *o2,
                                       QObject *parent = nullptr);
};

} // namespace Services

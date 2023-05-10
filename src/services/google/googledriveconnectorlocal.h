#pragma once

#include "o2.h"
#include "rest/restserviceconnectorlocal.h"

class GoogleDriveConnectorLocal : public RESTServiceConnectorLocal
{
    Q_OBJECT
public:
    GoogleDriveConnectorLocal(const QString &serviceName, QNetworkAccessManager &networkManager, O2 *o2,
                              QObject *parent = nullptr);
};

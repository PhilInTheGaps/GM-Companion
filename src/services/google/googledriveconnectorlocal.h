#pragma once

#include "rest/restserviceconnectorlocal.h"
#include "o2google.h"

class GoogleDriveConnectorLocal : public RESTServiceConnectorLocal
{
    Q_OBJECT
public:
    GoogleDriveConnectorLocal
    (QNetworkAccessManager *networkManager, O2Google *o2Google, QObject *parent = nullptr);
};

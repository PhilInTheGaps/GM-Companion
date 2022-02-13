#pragma once

#include "rest/restserviceconnectorlocal.h"
#include "o2google.h"

class GoogleDriveConnectorLocal : public RESTServiceConnectorLocal
{
    Q_OBJECT
public:
    GoogleDriveConnectorLocal
    (QNetworkAccessManager *networkManager, O2Google *o2Google, QObject *parent = nullptr);

private:
    static constexpr int LOCAL_PORT = 59993;
    static constexpr int MAX_CONCURRENT_REQUESTS = 5;
    static constexpr const char* SCOPE = "https://www.googleapis.com/auth/drive";
};

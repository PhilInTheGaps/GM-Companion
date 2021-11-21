#include "fileresult.h"
#include "rest/restnetworkreply.h"

using namespace Files;

auto FileResult::fromNetworkReply(RestNetworkReply *reply, QObject *parent) -> FileResult*
{
    bool success = true;
    QString errorMessage;

    if (!reply || reply->error() != QNetworkReply::NoError)
    {
        success = false;
        errorMessage = reply->errorText();
    }

    if (reply) reply->deleteLater();

    return new FileResult(success, errorMessage, parent);
}

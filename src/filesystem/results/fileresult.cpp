#include "fileresult.h"
#include "rest/restnetworkreply.h"

using namespace Files;

auto FileResult::fromNetworkReply(RestNetworkReply *reply, QObject *parent) -> FileResult*
{
    bool success = true;
    QString errorMessage;

    if (!reply)
    {
        success = false;
        errorMessage = "RestNetworkReply is null! (this is probably caused by a prior error)";
    }

    if (reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            success = false;
            errorMessage = reply->errorText();
        }

        reply->deleteLater();
    }

    return new FileResult(success, errorMessage, parent);
}

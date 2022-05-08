#include "filecheckresult.h"
#include "rest/restnetworkreply.h"

using namespace Files;

auto FileCheckResult::fromNetworkReply(RestNetworkReply *reply, const QString &path, QObject *parent) -> FileCheckResult*
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

    if (success)
    {
        return new FileCheckResult(path, success, parent);
    }

    return new FileCheckResult(path, errorMessage, parent);
}

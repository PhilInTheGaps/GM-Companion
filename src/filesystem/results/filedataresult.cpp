#include "filedataresult.h"
#include "rest/restnetworkreply.h"

using namespace Files;

auto FileDataResult::fromNetworkReply(RestNetworkReply *reply, QObject *parent) -> FileDataResult *
{
    bool success = true;
    QString errorMessage;
    const auto data = reply->data();

    if (!reply || reply->error() != QNetworkReply::NoError)
    {
        success = false;
        errorMessage = reply->errorText();
    }

    if (reply) reply->deleteLater();

    if (success)
    {
        return new FileDataResult(data, parent);
    }

    return new FileDataResult(errorMessage, parent);
}

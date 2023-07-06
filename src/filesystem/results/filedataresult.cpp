#include "filedataresult.h"
#include "rest/restnetworkreply.h"

using namespace Qt::Literals::StringLiterals;
using namespace Files;

auto FileDataResult::fromNetworkReply(RestNetworkReply *reply, QObject *parent) -> FileDataResult *
{
    bool success = true;
    QString errorMessage;

    if (!reply)
    {
        success = false;
        errorMessage = u"RestNetworkReply is null! (this is probably caused by a prior error)"_s;
        return new FileDataResult(errorMessage, parent);
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        success = false;
        errorMessage = reply->errorText();
    }

    const auto data = reply->data();
    reply->deleteLater();

    if (success)
    {
        return new FileDataResult(data, parent);
    }

    return new FileDataResult(errorMessage, parent);
}

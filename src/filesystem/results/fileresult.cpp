#include "fileresult.h"
#include "rest/restnetworkreply.h"

using namespace Qt::Literals::StringLiterals;
using namespace Files;

auto FileResult::fromNetworkReply(RestNetworkReply *reply, QObject *parent) -> FileResult *
{
    bool success = true;
    QString errorMessage;

    if (!reply)
    {
        success = false;
        errorMessage = u"RestNetworkReply is null! (this is probably caused by a prior error)"_s;
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

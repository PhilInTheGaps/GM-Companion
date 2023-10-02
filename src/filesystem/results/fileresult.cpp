#include "fileresult.h"
#include "rest/restnetworkreply.h"
#include <QScopedPointer>

using namespace Qt::Literals::StringLiterals;
using namespace Files;

auto FileResult::fromNetworkReply(RestNetworkReply *_reply) -> std::shared_ptr<FileResult>
{
    bool success = true;
    QString errorMessage;
    QScopedPointer reply(_reply);

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
    }

    return std::make_shared<FileResult>(success, errorMessage);
}

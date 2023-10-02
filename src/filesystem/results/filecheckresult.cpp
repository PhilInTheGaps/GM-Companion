#include "filecheckresult.h"
#include "rest/restnetworkreply.h"
#include <QScopedPointer>

using namespace Qt::Literals::StringLiterals;
using namespace Files;

auto FileCheckResult::fromNetworkReply(RestNetworkReply *_reply, const QString &path)
    -> std::unique_ptr<FileCheckResult>
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

    if (success)
    {
        return std::make_unique<FileCheckResult>(path, success);
    }

    return std::make_unique<FileCheckResult>(path, errorMessage);
}

#include "filecheckresult.h"
#include "rest/restreply.h"

using namespace Qt::Literals::StringLiterals;
using namespace Files;
using namespace Services;

auto FileCheckResult::fromRestReply(RestReply &&reply, const QString &path) -> FileCheckResult
{
    bool success = true;
    QString errorMessage;

    if (reply.error() != QNetworkReply::NoError)
    {
        success = false;
        errorMessage = reply.errorText();
    }

    if (success)
    {
        return FileCheckResult(path, success);
    }

    return FileCheckResult(path, errorMessage);
}

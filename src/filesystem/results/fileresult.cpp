#include "fileresult.h"
#include "rest/restreply.h"

using namespace Qt::Literals::StringLiterals;
using namespace Files;
using namespace Services;

auto FileResult::fromRestReply(const RestReply &reply) -> FileResult
{
    bool success = true;
    QString errorMessage;

    if (reply.error() != QNetworkReply::NoError)
    {
        success = false;
        errorMessage = reply.errorText();
    }

    return FileResult(success, errorMessage);
}

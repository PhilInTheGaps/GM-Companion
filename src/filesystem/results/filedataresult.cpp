#include "filedataresult.h"
#include "rest/restreply.h"

using namespace Qt::Literals::StringLiterals;
using namespace Files;
using namespace Services;

auto FileDataResult::fromRestReply(const RestReply &reply) -> FileDataResult
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
        return FileDataResult(reply.data());
    }

    return FileDataResult(errorMessage);
}

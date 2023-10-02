#include "filedataresult.h"
#include "rest/restnetworkreply.h"
#include <QScopedPointer>

using namespace Qt::Literals::StringLiterals;
using namespace Files;

auto FileDataResult::fromNetworkReply(RestNetworkReply *_reply) -> std::shared_ptr<FileDataResult>
{
    bool success = true;
    QString errorMessage;
    QScopedPointer reply(_reply);

    if (!reply)
    {
        success = false;
        errorMessage = u"RestNetworkReply is null! (this is probably caused by a prior error)"_s;
        return std::make_shared<FileDataResult>(errorMessage);
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        success = false;
        errorMessage = reply->errorText();
    }

    const auto data = reply->data();

    if (success)
    {
        return std::make_shared<FileDataResult>(data);
    }

    return std::make_shared<FileDataResult>(errorMessage);
}

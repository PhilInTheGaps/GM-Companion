#include "fileaccess.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

using namespace Files;

auto FileAccess::multiGetDataAsync(MultiGetHelper<FileDataResult> *helper, bool allowCache)
    -> QFuture<QVector<FileDataResult *>>
{
    if (helper->isDone())
    {
        auto results = helper->getResults();
        helper->deleteLater();
        return AsyncFuture::completed(results);
    }

    auto next = getDataAsync(helper->getNextPath(), allowCache);

    return AsyncFuture::observe(next)
        .subscribe([this, helper, allowCache](FileDataResult *result) {
            helper->addResult(result);
            return multiGetDataAsync(helper, allowCache);
        })
        .future();
}

auto FileAccess::multiCheckAsync(MultiGetHelper<FileCheckResult> *helper, bool allowCache)
    -> QFuture<FileMultiCheckResult *>
{
    if (helper->isDone())
    {
        auto results = helper->getResults();
        helper->deleteLater();
        return AsyncFuture::completed(new FileMultiCheckResult(results, this));
    }

    auto next = checkAsync(helper->getNextPath(), allowCache);

    return AsyncFuture::observe(next)
        .subscribe([this, helper, allowCache](FileCheckResult *result) {
            helper->addResult(result);
            return multiCheckAsync(helper, allowCache);
        })
        .future();
}

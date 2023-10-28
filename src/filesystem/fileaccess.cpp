#include "fileaccess.h"

using namespace Files;

auto FileAccess::multiGetDataAsync(MultiGetHelper<FileDataResult> &&helper, bool allowCache)
    -> QFuture<std::vector<FileDataResult>>
{
    if (helper.isDone())
    {
        return QtFuture::makeReadyFuture(helper.getResults());
    }

    auto next = getDataAsync(helper.getNextPath(), allowCache);

    return next
        .then([this, helper = std::move(helper), allowCache](FileDataResult &&result) mutable {
            helper.addResult(std::move(result));
            return multiGetDataAsync(std::move(helper), allowCache);
        })
        .unwrap();
}

auto FileAccess::multiCheckAsync(MultiGetHelper<FileCheckResult> &&helper, bool allowCache)
    -> QFuture<FileMultiCheckResult>
{
    if (helper.isDone())
    {
        return QtFuture::makeReadyFuture(FileMultiCheckResult(helper.getResults()));
    }

    auto next = checkAsync(helper.getNextPath(), allowCache);

    return next
        .then([this, helper = std::move(helper), allowCache](FileCheckResult &&result) mutable {
            helper.addResult(std::move(result));
            return multiCheckAsync(std::move(helper), allowCache);
        })
        .unwrap();
}

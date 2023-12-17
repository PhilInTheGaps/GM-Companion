#include "fileaccess.h"

using namespace Files;

auto FileAccess::multiGetDataAsync(MultiGetHelper<FileDataResult> &&helper, Options options)
    -> QFuture<std::vector<FileDataResult>>
{
    if (helper.isDone())
    {
        return QtFuture::makeReadyFuture(helper.getResults());
    }

    auto next = getDataAsync(helper.getNextPath(), options);

    return next
        .then([this, helper = std::move(helper), options](FileDataResult &&result) mutable {
            helper.addResult(std::move(result));
            return multiGetDataAsync(std::move(helper), options);
        })
        .unwrap();
}

auto FileAccess::multiCheckAsync(MultiGetHelper<FileCheckResult> &&helper, Options options)
    -> QFuture<FileMultiCheckResult>
{
    if (helper.isDone())
    {
        return QtFuture::makeReadyFuture(FileMultiCheckResult(helper.getResults()));
    }

    auto next = checkAsync(helper.getNextPath(), options);

    return next
        .then([this, helper = std::move(helper), options](FileCheckResult &&result) mutable {
            helper.addResult(std::move(result));
            return multiCheckAsync(std::move(helper), options);
        })
        .unwrap();
}

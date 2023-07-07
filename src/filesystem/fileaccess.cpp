#include "fileaccess.h"

using namespace Files;

auto FileAccess::multiGetDataAsync(MultiGetHelper<FileDataResult> *helper, bool allowCache)
    -> QFuture<std::vector<FileDataResult *>>
{
    if (helper->isDone())
    {
        auto results = helper->getResults();
        helper->deleteLater();
        return QtFuture::makeReadyFuture(results);
    }

    auto next = getDataAsync(helper->getNextPath(), allowCache);

    return next
        .then(this,
              [this, helper, allowCache](FileDataResult *result) {
                  helper->addResult(result);
                  return multiGetDataAsync(helper, allowCache);
              })
        .unwrap();
}

auto FileAccess::multiCheckAsync(MultiGetHelper<FileCheckResult> *helper, bool allowCache)
    -> QFuture<FileMultiCheckResult *>
{
    if (helper->isDone())
    {
        auto results = helper->getResults();
        helper->deleteLater();
        return QtFuture::makeReadyFuture(new FileMultiCheckResult(results, this));
    }

    auto next = checkAsync(helper->getNextPath(), allowCache);

    return next
        .then(this,
              [this, helper, allowCache](FileCheckResult *result) {
                  helper->addResult(result);
                  return multiCheckAsync(helper, allowCache);
              })
        .unwrap();
}

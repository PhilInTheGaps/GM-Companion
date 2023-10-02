#include "fileaccess.h"

using namespace Files;

auto FileAccess::multiGetDataAsync(std::shared_ptr<MultiGetHelper<FileDataResult>> helper, bool allowCache)
    -> QFuture<std::vector<std::shared_ptr<FileDataResult>>>
{
    if (helper->isDone())
    {
        return QtFuture::makeReadyFuture(helper->getResults());
    }

    auto next = getDataAsync(helper->getNextPath(), allowCache);

    return next
        .then(context(),
              [this, helper, allowCache](std::shared_ptr<FileDataResult> result) {
                  helper->addResult(result);
                  return multiGetDataAsync(helper, allowCache);
              })
        .unwrap();
}

auto FileAccess::multiCheckAsync(std::shared_ptr<MultiGetHelper<FileCheckResult>> helper, bool allowCache)
    -> QFuture<std::shared_ptr<FileMultiCheckResult>>
{
    if (helper->isDone())
    {
        auto results = helper->getResults();
        return QtFuture::makeReadyFuture(std::make_shared<FileMultiCheckResult>(results));
    }

    auto next = checkAsync(helper->getNextPath(), allowCache);

    return next
        .then(context(),
              [this, helper, allowCache](std::shared_ptr<FileCheckResult> result) {
                  helper->addResult(std::move(result));
                  return multiCheckAsync(helper, allowCache);
              })
        .unwrap();
}

auto FileAccess::context() -> QObject *
{
    return &m_context;
}

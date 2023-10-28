#include "checkimpl.h"
#include "../fileaccessgoogledrive.h"
#include "endpoints.h"
#include "rest/restreply.h"
#include "utils/fileutils.h"
#include "utils/networkutils.h"

using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;

auto CheckImpl::check(const QString &path, FileAccessGoogleDrive *fa) -> QFuture<FileCheckResult>
{
    State state;
    state.fa = fa;
    state.path = path;
    return check(std::move(state));
}

auto CheckImpl::check(State &&state) -> QFuture<FileCheckResult>
{
    return state.fa->getFileIdAsync(state.path)
        .then([state = std::move(state)](const QString &id) mutable {
            return FileCheckResult(state.path, !id.isEmpty());
        })
        .onCanceled([state = std::move(state)]() { return FileCheckResult(state.path, false); });
}

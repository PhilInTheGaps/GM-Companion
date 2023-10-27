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
        .then(state.fa->context(),
              [state = std::move(state)](const QString &id) mutable {
                  //                  const auto url = QUrl(u"%1/%2"_s.arg(FILES_ENDPOINT, folderId));

                  return FileCheckResult(state.path, !id.isEmpty());

                  //                  m_gd.get(url)
                  //                      .then(context(),
                  //                            [this, path, promise](RestReply reply) mutable {
                  //                                const auto json = QJsonDocument::fromJson(reply.data()).object();
                  //                                const auto id = json["id"_L1].toString();
                  //                                m_idCache.createOrUpdateEntry(path, id.toUtf8());

                  //                                promise->addResult(FileCheckResult::fromRestReply(reply, path));
                  //                                promise->finish();
                  //                            })
                  //                      .onCanceled(context(), [path, promise]() mutable {
                  //                          promise->addResult(FileCheckResult(path, false));
                  //                          promise->finish();
                  //                      });
              })
        .onCanceled(state.fa->context(), [state = std::move(state)]() { return FileCheckResult(state.path, false); });
}

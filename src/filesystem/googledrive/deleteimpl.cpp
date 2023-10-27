#include "deleteimpl.h"
#include "../fileaccessgoogledrive.h"
#include "endpoints.h"
#include "rest/restreply.h"

using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;

auto DeleteImpl::deleteFile(const QString &path, FileAccessGoogleDrive *fa) -> QFuture<FileResult>
{
    State state;
    state.fa = fa;
    state.path = path;

    return deleteFile(std::move(state));
}

auto DeleteImpl::deleteFile(State &&state) -> QFuture<FileResult>
{
    return state.fa->getFileIdAsync(state.path)
        .then(state.fa->context(),
              [state = std::move(state)](const QString &id) mutable {
                  if (id.isEmpty()) return QtFuture::makeReadyFuture(FileResult(u"Could not get file id"));

                  state.fileId = id;
                  return onFileIdReceived(std::move(state));
              })
        .unwrap()
        .onCanceled(state.fa->context(), []() { return FileResult(u"Could not get file id"); });
}

auto DeleteImpl::onFileIdReceived(State &&state) -> QFuture<FileResult>
{
    const auto url = QUrl(u"%1/%2"_s.arg(FILES_ENDPOINT, state.fileId));
    const auto request = QNetworkRequest(url);

    return state.fa->m_gd.customRequest(request, "DELETE", "")
        .then(state.fa->context(),
              [state = std::move(state)](RestReply reply) mutable {
                  if (!reply.hasError())
                  {
                      // Save the folder ID
                      state.fa->m_idCache.removeEntry(state.path);
                      state.fa->m_fileCache.removeEntry(state.path);
                  }

                  return FileResult::fromRestReply(std::move(reply));
              })
        .onCanceled(state.fa->context(), []() { return FileResult(u"Could not delete file"); });
}

#include "getdataimpl.h"
#include "../fileaccessgoogledrive.h"
#include "endpoints.h"
#include "rest/restreply.h"
#include "results/filedataresult.h"

using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;

auto GetDataImpl::getData(const QString &path, FileAccessGoogleDrive *fa) -> QFuture<FileDataResult>
{
    State state;
    state.fa = fa;
    state.path = path;

    return getData(std::move(state));
}

auto GetDataImpl::getData(State &&state) -> QFuture<FileDataResult>
{
    return state.fa->getFileIdAsync(state.path)
        .then(state.fa->context(),
              [state = std::move(state)](const QString &fileId) mutable {
                  if (fileId.isEmpty()) return QtFuture::makeReadyFuture(FileDataResult(u"Could not get file ID"_s));

                  state.fileId = fileId;
                  return onFileIdReceived(std::move(state));
              })
        .unwrap()
        .onCanceled([]() { return FileDataResult(u"Could not get file ID"_s); });
}

auto GetDataImpl::onFileIdReceived(State &&state) -> QFuture<FileDataResult>
{
    const auto url = QUrl(u"%1/%2?alt=media"_s.arg(FILES_ENDPOINT, state.fileId));

    return state.fa->m_gd.get(url)
        .then(state.fa->context(),
              [state = std::move(state)](RestReply reply) {
                  if (reply.error() == QNetworkReply::NoError)
                  {
                      state.fa->m_fileCache.createOrUpdateEntry(state.path, reply.data());
                  }

                  return FileDataResult::fromRestReply(std::move(reply));
              })
        .onCanceled([]() { return FileDataResult(u"Could not get file"_s); });
}

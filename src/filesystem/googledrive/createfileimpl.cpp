#include "createfileimpl.h"
#include "../fileaccessgoogledrive.h"
#include "endpoints.h"
#include "rest/restreply.h"
#include "results/filecheckresult.h"
#include "utils/fileutils.h"
#include "utils/networkutils.h"
#include <QUrl>

using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;

auto CreateFileImpl::create(const QString &path, const QByteArray &mimeType, FileAccessGoogleDrive *fa)
    -> QFuture<FileResult>
{
    State state;
    state.fa = fa;
    state.path = path;
    state.mimeType = mimeType;
    return create(std::move(state));
}

auto CreateFileImpl::create(State &&state) -> QFuture<FileResult>
{
    return state.fa->checkAsync(state.path, true)
        .then(state.fa->context(),
              [state = std::move(state)](FileCheckResult &&result) mutable {
                  if (result.exists())
                  {
                      return QtFuture::makeReadyFuture(FileResult(u"File already exists!"_s));
                  }

                  return onFileCheckComplete(std::move(state));
              })
        .unwrap()
        .onCanceled(state.fa->context(), []() { return FileResult(u"Could not check if file already exists!"_s); });
}

auto CreateFileImpl::onFileCheckComplete(State &&state) -> QFuture<FileResult>
{
    return state.fa->getParentIdAsync(state.path)
        .then(state.fa->context(),
              [state = std::move(state)](const std::pair<QString, QString> &parent) mutable {
                  state.parentId = parent.second;
                  return onParentIdReceived(std::move(state));
              })
        .onCanceled(state.fa->context(),
                    [state = std::move(state)]() mutable { return createParentDirAndTryAgain(std::move(state)); })
        .unwrap();
}

auto CreateFileImpl::onParentIdReceived(State &&state) -> QFuture<FileResult>
{
    // Build file meta data
    const auto metaData = state.fa->makeMetaData(FileUtils::fileName(state.path), state.parentId, state.mimeType);
    const auto request = NetworkUtils::makeJsonRequest(QUrl(FILES_ENDPOINT));

    // Send "create" request
    return state.fa->m_gd.post(request, metaData)
        .then(state.fa->context(),
              [state = std::move(state)](RestReply reply) mutable {
                  if (!reply.hasError())
                  {
                      // Save the folder ID
                      const auto json = QJsonDocument::fromJson(reply.data()).object();
                      state.fa->m_idCache.createOrUpdateEntry(state.path, json["id"_L1].toString().toUtf8());
                  }

                  return FileResult::fromRestReply(std::move(reply));
              })
        .onCanceled(state.fa->context(), []() { return FileResult(u"Could not send POST request!"_s); });
}

auto CreateFileImpl::createParentDirAndTryAgain(State &&state) -> QFuture<FileResult>
{
    const auto parentPath = FileUtils::dirFromPath(state.path);
    return state.fa->createDirAsync(parentPath)
        .then(state.fa->context(),
              [state = std::move(state)](FileResult &&result) mutable {
                  if (!result.success())
                  {
                      return QtFuture::makeReadyFuture(result);
                  }
                  return create(std::move(state));
              })
        .unwrap()
        .onCanceled(state.fa->context(), []() { return FileResult(u"Could not create parent dir!"_s); });
}

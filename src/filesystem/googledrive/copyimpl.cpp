#include "copyimpl.h"
#include "../fileaccessgoogledrive.h"
#include "endpoints.h"
#include "rest/restreply.h"
#include "utils/fileutils.h"
#include "utils/networkutils.h"

using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;

auto CopyImpl::copy(const QString &path, const QString &copy, FileAccessGoogleDrive *fa) -> QFuture<FileResult>
{
    State state;
    state.fa = fa;
    state.path = path;
    state.copyPath = copy;
    return CopyImpl::copy(std::move(state));
}

auto CopyImpl::copy(State &&state) -> QFuture<FileResult>
{
    return state.fa->checkAsync(state.copyPath, true)
        .then(state.fa->context(),
              [state = std::move(state)](FileCheckResult &&result) mutable {
                  if (result.exists())
                  {
                      return QtFuture::makeReadyFuture(FileResult(u"Destination already exists!"_s));
                  }

                  return onCheckSucceeded(std::move(state));
              })
        .unwrap()
        .onCanceled(state.fa->context(),
                    []() { return FileResult(u"Could not check if destination already exists!"_s); });
}

auto CopyImpl::onCheckSucceeded(State &&state) -> QFuture<FileResult>
{
    return state.fa->getFileIdAsync(state.path)
        .then(state.fa->context(),
              [state = std::move(state)](const QString &id) mutable {
                  if (id.isEmpty()) return QtFuture::makeReadyFuture(FileResult(u"Could not get file ID"_s));

                  state.fileId = id;
                  return onFileIdReceived(std::move(state));
              })
        .unwrap()
        .onCanceled(state.fa->context(), []() { return FileResult(u"Could not get file ID"_s); });
}

auto CopyImpl::onFileIdReceived(State &&state) -> QFuture<FileResult>
{
    return state.fa->getParentIdAsync(state.copyPath)
        .then(state.fa->context(),
              [state = std::move(state)](const std::pair<QString, QString> &parent) mutable {
                  state.newParentId = parent.second;
                  return onNewParentIdReceived(std::move(state));
              })
        .onCanceled(state.fa->context(),
                    [state = std::move(state)]() mutable { return createNewParentDirAndTryAgain(std::move(state)); })
        .unwrap();
}

auto CopyImpl::onNewParentIdReceived(State &&state) -> QFuture<FileResult>
{
    const auto filename = FileUtils::fileName(state.copyPath);
    const auto metadata = state.fa->makeMetaData(filename, state.newParentId);

    const auto url = QUrl(u"%1/%2/copy"_s.arg(FILES_ENDPOINT, state.newParentId));
    const auto request = NetworkUtils::makeJsonRequest(url);

    return state.fa->m_gd.post(request, metadata)
        .then(state.fa->context(),
              [state = std::move(state)](RestReply &&reply) mutable {
                  if (!reply.hasError())
                  {
                      // Save the folder ID
                      const auto json = QJsonDocument::fromJson(reply.data()).object();
                      state.fa->m_idCache.createOrUpdateEntry(state.copyPath, json["id"_L1].toString().toUtf8());
                      state.fa->m_fileCache.copyEntry(state.path, state.copyPath);
                  }

                  return FileResult::fromRestReply(std::move(reply));
              })
        .onCanceled(state.fa->context(), []() { return FileResult(u"Could not send copy request"_s); });
}

auto CopyImpl::createNewParentDirAndTryAgain(State &&state) -> QFuture<FileResult>
{
    const auto parentPath = FileUtils::dirFromPath(state.copyPath);

    return state.fa->createDirAsync(parentPath)
        .then(state.fa->context(),
              [state = std::move(state)](FileResult &&reply) mutable {
                  if (!reply.success()) return QtFuture::makeReadyFuture(reply);
                  return copy(std::move(state));
              })
        .unwrap()
        .onCanceled(state.fa->context(), []() mutable { return FileResult(u"Could not create new parent folder"_s); });
}

#include "moveimpl.h"
#include "../fileaccessgoogledrive.h"
#include "endpoints.h"
#include "rest/restreply.h"
#include "utils/fileutils.h"
#include "utils/networkutils.h"

using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;
using namespace Qt::Literals::StringLiterals;

auto MoveImpl::move(const QString &oldPath, const QString &newPath, FileAccessGoogleDrive *fa) -> QFuture<FileResult>
{
    State state;
    state.fa = fa;
    state.oldPath = oldPath;
    state.newPath = newPath;
    return move(std::move(state));
}

auto MoveImpl::move(State &&state) -> QFuture<FileResult>
{
    return state.fa->getFileIdAsync(state.oldPath)
        .then([state = std::move(state)](const QString &fileId) mutable {
            if (fileId.isEmpty()) return QtFuture::makeReadyFuture(FileResult(u"Could not find file/folder ID"_s));

            state.fileId = fileId;
            return onFileIdReceived(std::move(state));
        })
        .unwrap()
        .onCanceled([]() { return FileResult(u"Could not find file/folder ID"_s); });
}

auto MoveImpl::onFileIdReceived(State &&state) -> QFuture<FileResult>
{
    return state.fa->getParentIdAsync(state.oldPath)
        .then([state = std::move(state)](const std::pair<QString, QString> &oldParent) mutable {
            state.oldParentId = oldParent.second;
            return onParentIdReceived(std::move(state));
        })
        .unwrap()
        .onCanceled([]() { return FileResult(u"Could not find ID of old parent folder"_s); });
}

auto MoveImpl::onParentIdReceived(State &&state) -> QFuture<FileResult>
{
    return state.fa->getParentIdAsync(state.newPath)
        .then([state = std::move(state)](const std::pair<QString, QString> &newParent) mutable {
            state.newParentId = newParent.second;
            return onNewParentIdReceived(std::move(state));
        })
        .onCanceled([state = std::move(state)]() mutable { return createNewParentFolderAndTryAgain(std::move(state)); })
        .unwrap();
}

auto MoveImpl::onNewParentIdReceived(State &&state) -> QFuture<FileResult>
{
    const auto filename = FileUtils::fileName(state.newPath);
    const auto metadata = state.fa->makeMetaData(filename);
    const auto url = QUrl(u"%1/%2?addParents=%3&removeParents=%4"_s.arg(FILES_ENDPOINT, state.fileId, state.newParentId,
                                                                        state.oldParentId));
    const auto request = NetworkUtils::makeJsonRequest(url);

    return state.fa->m_gd.customRequest(request, "PATCH", metadata)
        .then([state = std::move(state)](RestReply &&reply) {
            state.fa->m_fileCache.moveEntry(state.oldPath, state.newPath);
            state.fa->m_idCache.moveEntry(state.oldPath, state.newPath);
            return FileResult::fromRestReply(std::move(reply));
        })
        .onCanceled([]() { return FileResult(u"Could not send PATCH request"_s); });
}

auto MoveImpl::createNewParentFolderAndTryAgain(State &&state) -> QFuture<FileResult>
{
    const auto parentPath = FileUtils::dirFromPath(state.newPath);

    return state.fa->createDirAsync(parentPath)
        .then([state = std::move(state)](FileResult &&result) mutable {
            if (!result.success()) return QtFuture::makeReadyFuture(result);
            return move(std::move(state));
        })
        .unwrap()
        .onCanceled([]() { return FileResult(u"Could not create new parent folder"_s); });
}

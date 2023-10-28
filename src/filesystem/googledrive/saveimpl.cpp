#include "saveimpl.h"
#include "../fileaccessgoogledrive.h"
#include "endpoints.h"
#include "rest/restreply.h"
#include "utils/networkutils.h"

using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;

auto SaveImpl::save(const QString &path, const QByteArray &data, FileAccessGoogleDrive *fa) -> QFuture<FileResult>
{
    State state;
    state.fa = fa;
    state.path = path;
    state.data = data;
    return save(std::move(state));
}

auto SaveImpl::save(State &&state) -> QFuture<FileResult>
{
    auto callback = [state = std::move(state)](const QString &fileId = u""_s) mutable {
        qDebug() << "save() onGetFileId callback:" << fileId;

        return QFuture<FileResult>();

        if (fileId.isEmpty()) return createNewFile(std::move(state));

        state.fileId = fileId;
        return onFileIdReceived(std::move(state));
    };

    return state.fa->getFileIdAsync(state.path).then(callback).onCanceled(callback).unwrap();
}

auto SaveImpl::onFileIdReceived(State &&state) -> QFuture<FileResult>
{
    const auto url = QUrl(u"%1/%2?uploadType=resumable"_s.arg(UPLOAD_ENDPOINT, state.fileId));
    const auto request = NetworkUtils::makeJsonRequest(url);

    // Tell GoogleDrive that we want to upload a file
    return state.fa->m_gd.customRequest(request, "PATCH", "")
        .then([state = std::move(state)](RestReply reply) mutable {
            if (reply.hasError())
            {
                return QtFuture::makeReadyFuture(FileResult::fromRestReply(std::move(reply)));
            }

            state.uploadUrl = reply.getHeader("location");
            if (state.uploadUrl.isEmpty()) state.uploadUrl = reply.getHeader("Location");
            if (state.uploadUrl.isEmpty())
            {
                return QtFuture::makeReadyFuture(FileResult(u"Did not receive an upload URL"_s));
            }

            return onEndpointReceived(std::move(state));
        })
        .unwrap()
        .onCanceled([]() { return FileResult(u"Did not receive an upload URL"_s); });
}

auto SaveImpl::onEndpointReceived(State &&state) -> QFuture<FileResult>
{
    auto request = QNetworkRequest(QUrl(QString::fromUtf8(state.uploadUrl)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setHeader(QNetworkRequest::ContentLengthHeader, state.data.size());

    return state.fa->m_gd.put(request, state.data)
        .then([state = std::move(state)](RestReply reply) {
            if (reply.error() == QNetworkReply::NoError)
            {
                state.fa->m_fileCache.createOrUpdateEntry(state.path, state.data);
            }

            return FileResult::fromRestReply(std::move(reply));
        })
        .onCanceled([]() { return FileResult(u"Could not upload file to endpoint"_s); });
}

auto SaveImpl::createNewFile(State &&state) -> QFuture<FileResult>
{
    // Create file first
    return state.fa->createFileAsync(state.path, QByteArray())
        .then([state = std::move(state)](FileResult &&result) mutable {
            if (!result.success())
            {
                return QtFuture::makeReadyFuture(result);
            }

            // try again, since the file should exist now
            return save(std::move(state));
        })
        .unwrap()
        .onCanceled([]() { return FileResult(u"Could not create file"_s); });
}

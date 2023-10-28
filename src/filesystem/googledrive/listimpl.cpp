#include "listimpl.h"
#include "../fileaccessgoogledrive.h"
#include "endpoints.h"
#include "mimetype.h"
#include "rest/restreply.h"
#include "results/filelistresult.h"
#include "utils/fileutils.h"
#include "utils/networkutils.h"
#include <QUrl>
#include <QUrlQuery>

using namespace Files;
using namespace Files::GoogleDrive;
using namespace Services;
using namespace Qt::Literals::StringLiterals;

auto ListImpl::list(const QString &path, bool files, bool folders, FileAccessGoogleDrive *fa) -> QFuture<FileListResult>
{
    State state;
    state.fa = fa;
    state.path = path;
    state.includeFiles = files;
    state.includeFolders = folders;
    return list(std::move(state));
}

auto ListImpl::list(State &&state) -> QFuture<FileListResult>
{
    auto callback = [state = std::move(state)](const QString &id = u""_s) mutable {
        if (id.isEmpty())
        {
            return QtFuture::makeReadyFuture(FileListResult(state.path, u"Could not get folder id"_s));
        }

        state.query = makeListQuery(id, state);
        return onFileIdReceived(std::move(state));
    };

    return state.fa->getFileIdAsync(state.path).then(callback).onCanceled(callback).unwrap();
}

auto ListImpl::makeListQuery(const QString &id, const State &state) -> QString
{
    auto q = u"'%1' in parents and trashed = false"_s.arg(id);

    if (state.includeFolders && !state.includeFiles)
    {
        q.append(" and mimeType = '%1'"_L1.arg(MimeType::FOLDER));
    }
    else if (state.includeFiles && !state.includeFolders)
    {
        q.append(" and mimeType != '%1'"_L1.arg(MimeType::FOLDER));
    }

    return q;
}

auto ListImpl::onFileIdReceived(State &&state) -> QFuture<FileListResult>
{
    QUrl url(FILES_ENDPOINT);
    auto query = QUrlQuery({{"q", QUrl::toPercentEncoding(state.query)}});

    if (!state.pageToken.isEmpty())
    {
        query.addQueryItem(u"pageToken"_s, state.pageToken);
    }

    url.setQuery(query);

    return state.fa->m_gd.get(NetworkUtils::makeJsonRequest(url))
        .then([state = std::move(state)](RestReply reply) mutable {
            if (reply.hasError())
            {
                return QtFuture::makeReadyFuture(FileListResult(state.path, reply.errorText()));
            }

            const auto json = QJsonDocument::fromJson(reply.data()).object();

            foreach (const auto &file, json["files"_L1].toArray())
            {
                const auto fileName = file["name"_L1].toString();
                const auto filePath = FileUtils::fileInDir(fileName, state.path);
                const auto id = file["id"_L1].toString().toUtf8();
                const auto mimeType = file["mimeType"_L1].toString();

                if (mimeType == MimeType::FOLDER)
                {
                    state.folders << fileName;
                }
                else
                {
                    state.files << fileName;
                }

                state.fa->m_idCache.createOrUpdateEntry(filePath, id);
            }

            // Get the next page
            if (json.contains("nextPageToken"_L1))
            {
                state.pageToken = json["nextPageToken"_L1].toString();
                return list(std::move(state));
            }

            return QtFuture::makeReadyFuture(FileListResult(state.path, state.folders, state.files));
        })
        .unwrap();
}

#include "mocknextcloud.h"
#include "mocknetworkreply.h"
#include "oputils.h"
#include "utils/fileutils.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class MockData
{
public:
    static auto loginV2(const QString &baseUrl, QObject *parent) -> QNetworkReply *;
    static auto loginToken() -> QString;
    static auto loginPoll(const QString &baseUrl, QObject *parent) -> QNetworkReply *;
};

MockNextCloud::MockNextCloud(const QString &host, QObject *parent)
    : AbstractMockNetworkManager({host}, parent), m_mockDir(QStringLiteral("/"))
{
}

auto MockNextCloud::sendMockReply(Operation op, const QNetworkRequest &req, const QByteArray &data) -> QNetworkReply *
{
    const auto url = req.url();

    qDebug() << OpUtils::qtOpToVerb(op) << url.scheme() << url.host() << url.path();

    if (op == Operation::PostOperation && url.path() == QStringLiteral("/index.php/login/v2"))
    {
        return MockData::loginV2(baseUrl(url), this);
    }

    if (op == Operation::PostOperation && url.path() == QStringLiteral("/index.php/login/v2/poll"))
    {
        if (data == QStringLiteral("token=%1").arg(MockData::loginToken()))
        {
            if (m_shouldAuthPollReturnSuccess)
            {
                m_shouldAuthPollReturnSuccess = false;
                return MockData::loginPoll(baseUrl(url), this);
            }

            m_shouldAuthPollReturnSuccess = true;
        }

        return MockNetworkReply::notFound(this);
    }

    if (url.path().startsWith(QStringLiteral("/remote.php/dav/files")))
    {
        auto filePath = url.path();
        filePath = filePath.replace(QStringLiteral("/remote.php/dav/files"), QLatin1String());
        return sendDavReply(op, req, filePath, data);
    }

    return MockNetworkReply::notFound(this);
}

auto MockNextCloud::sendDavReply(Operation op, const QNetworkRequest &req, const QString &path, const QByteArray &data)
    -> QNetworkReply *
{
    const auto davOp = OpUtils::qtOpToDavOp(op, req);
    qDebug() << "DAV:" << OpUtils::davOpToVerb(davOp) << path << data;

    switch (davOp)
    {
    case OpUtils::DavOperation::Get: {
        const auto file = m_mockDir.getFile(path);
        if (file.name.isEmpty()) return MockNetworkReply::notFound(this);
        return MockNetworkReply::successGeneric(file.data, this);
    }
    case OpUtils::DavOperation::Put:
        m_mockDir.createOrUpdateFile(path, data);
        return MockNetworkReply::successEmpty(this);
    case OpUtils::DavOperation::Mkcol:
        return m_mockDir.mkdir(path) ? MockNetworkReply::successEmpty(this) : MockNetworkReply::badRequest(this);
    case OpUtils::DavOperation::Delete:
        return m_mockDir.deleteFileOrFolder(path) ? MockNetworkReply::successEmpty(this)
                                                  : MockNetworkReply::notFound(this);
    case OpUtils::DavOperation::Move:
        return m_mockDir.moveFileOrFolder(path, destinationFromHeader(req)) ? MockNetworkReply::successEmpty(this)
                                                                            : MockNetworkReply::notFound(this);
    case OpUtils::DavOperation::Copy:
        return m_mockDir.copyFileOrFolder(path, destinationFromHeader(req)) ? MockNetworkReply::successEmpty(this)
                                                                            : MockNetworkReply::notFound(this);
    case OpUtils::DavOperation::PropFind:
        return sendPropFindReply(path);
    default:
        break;
    }

    qWarning() << "Unhandled DAV request:" << OpUtils::davOpToVerb(davOp) << path << data;
    return MockNetworkReply::notFound(this);
}

auto MockNextCloud::sendPropFindReply(const QString &path) -> QNetworkReply *
{
    const auto file = m_mockDir.getFile(path);
    const auto *dir = m_mockDir.getDir(path);

    if (file.name.isEmpty() && !dir) return MockNetworkReply::notFound(this);

    const auto files = mockFilesToDResponses(path, m_mockDir.getFiles(path));
    const auto dirs = mockDirsToDResponses(path, m_mockDir.getDirs(path));
    const auto parentDirName = FileUtils::dirFromPath(path);

    const auto res = QStringLiteral(R"(<?xml version="1.0"?><d:multistatus xmlns:d="DAV:"
                                        xmlns:s="http://sabredav.org/ns"
                                        xmlns:oc="http://owncloud.org/ns"
                                        xmlns:nc="http://nextcloud.org/ns">
                                        %1
                                        %2
                                        %3
                                       </d:multistatus>)")
                         .arg(file.name.isEmpty() ? mockDirToDResponse(parentDirName, *dir)
                                                  : mockFileToDResponse(parentDirName, file),
                              files, dirs);

    return MockNetworkReply::successXml(res.toUtf8(), this);
}

auto MockNextCloud::destinationFromHeader(const QNetworkRequest &req) -> QString
{
    return QString::fromUtf8(req.rawHeader("Destination"))
        .replace(QStringLiteral("%1://%2/remote.php/dav/files").arg(req.url().scheme(), req.url().host()),
                 QLatin1String());
}

auto MockNextCloud::mockFilesToDResponses(const QString &path, const std::vector<MockFile> &files) -> QString
{
    QString res;
    for (const auto &file : files)
    {
        res += mockFileToDResponse(path, file);
    }
    return res;
}

auto MockNextCloud::mockFileToDResponse(const QString &path, const MockFile &file) -> QString
{
    return QStringLiteral(R"(
        <d:response>
            <d:href>/remote.php/dav/files%1/%2</d:href>
            <d:propstat>
                <d:prop>
                    <d:getlastmodified>%3</d:getlastmodified>
                    <d:getcontentlength>%4</d:getcontentlength>
                    <d:getcontenttype>application/octet-stream</d:getcontenttype>
                </d:prop>
                <d:status>HTTP/1.1 200 OK</d:status>
            </d:propstat>
        </d:response>)")
        .arg(path, QUrl::toPercentEncoding(file.name),
             file.lastModified.toString(QStringLiteral("ddd, dd MMMM yyyy hh:mm:ss")),
             QString::number(file.data.length()));
}

auto MockNextCloud::mockDirsToDResponses(const QString &path, const std::vector<MockDir> &dirs) -> QString
{
    QString res;
    for (const auto &dir : dirs)
    {
        res += mockDirToDResponse(path, dir);
    }
    return res;
}

auto MockNextCloud::mockDirToDResponse(const QString &path, const MockDir &dir) -> QString
{
    return QStringLiteral(R"(
        <d:response>
            <d:href>/remote.php/dav/files%1/%2</d:href>
            <d:propstat>
                <d:prop>
                    <d:getlastmodified>%3</d:getlastmodified>
                    <d:resourcetype>
                        <d:collection/>
                    </d:resourcetype>
                </d:prop>
                <d:status>HTTP/1.1 200 OK</d:status>
            </d:propstat>
        </d:response>)")
        .arg(path, QUrl::toPercentEncoding(dir.name()),
             dir.lastModified().toString(QStringLiteral("ddd, dd MMMM yyyy hh:mm:ss")));
}

auto MockData::loginV2(const QString &baseUrl, QObject *parent) -> QNetworkReply *
{
    QJsonObject poll(
        {{"token", loginToken()}, {"endpoint", QStringLiteral("%1/index.php/login/v2/poll").arg(baseUrl)}});

    const QJsonDocument doc(
        {{"poll", poll},
         {"login", QStringLiteral("%1/index.php/login/v2/flow/"
                                  "guyjGtcKPTKCi4epIRIupIexgJ8wNInMFSfHabACRPZUkmEaWZSM54bFkFuzWksbps7jmTFQj"
                                  "eskLpyJXyhpHlgK8sZBn9HXLXjohIx5iXgJKdOkkZTYCzUWHlsg3YFg")
                       .arg(baseUrl)}});

    return MockNetworkReply::successJson(doc.toJson(QJsonDocument::Compact), parent);
}

auto MockData::loginToken() -> QString
{
    return QStringLiteral(
        "mQUYQdffOSAMJYtm8pVpkOsVqXt5hglnuSpO5EMbgJMNEPFGaiDe8OUjvrJ2WcYcBSLgqynu9jaPFvZHMl83ybMvp6aDIDARjTFIBpRWod6"
        "p32fL9LIpIStvc6k8Wrs1");
}

auto MockData::loginPoll(const QString &baseUrl, QObject *parent) -> QNetworkReply *
{
    const QJsonObject res(
        {{"server", baseUrl},
         {"loginName", "username"},
         {"appPassword", "yKTVA4zgxjfivy52WqD8kW3M2pKGQr6srmUXMipRdunxjPFripJn0GMfmtNOqOolYSuJ6sCN"}});

    return MockNetworkReply::successJson(QJsonDocument(res).toJson(), parent);
}

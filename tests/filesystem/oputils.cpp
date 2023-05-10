#include "oputils.h"

auto OpUtils::qtOpToDavOp(QNetworkAccessManager::Operation op, const QNetworkRequest &req) -> DavOperation
{
    switch (op)
    {
    case QNetworkAccessManager::HeadOperation:
        return DavOperation::Head;
    case QNetworkAccessManager::GetOperation:
        return DavOperation::Get;
    case QNetworkAccessManager::PutOperation:
        return DavOperation::Put;
    case QNetworkAccessManager::PostOperation:
        return DavOperation::Post;
    case QNetworkAccessManager::DeleteOperation:
        return DavOperation::Delete;
    case QNetworkAccessManager::CustomOperation:
        return davVerbToOp(req.attribute(QNetworkRequest::CustomVerbAttribute).toByteArray().constData());
    default:
        return DavOperation::Unknown;
    }
}

auto OpUtils::qtOpToVerb(QNetworkAccessManager::Operation op) -> QString
{
    switch (op)
    {
    case QNetworkAccessManager::HeadOperation:
        return QStringLiteral("HEAD");
    case QNetworkAccessManager::GetOperation:
        return QStringLiteral("GET");
    case QNetworkAccessManager::PutOperation:
        return QStringLiteral("PUT");
    case QNetworkAccessManager::PostOperation:
        return QStringLiteral("POST");
    case QNetworkAccessManager::DeleteOperation:
        return QStringLiteral("DELETE");
    case QNetworkAccessManager::CustomOperation:
        return QStringLiteral("CUSTOM");
    default:
        return QStringLiteral("Unknown");
    }
}

auto OpUtils::davVerbToOp(const QString &verb) -> DavOperation
{
    const auto v = verb.toLower();

    if (v == QStringLiteral("head")) return DavOperation::Head;
    if (v == QStringLiteral("get")) return DavOperation::Get;
    if (v == QStringLiteral("put")) return DavOperation::Put;
    if (v == QStringLiteral("post")) return DavOperation::Post;
    if (v == QStringLiteral("delete")) return DavOperation::Delete;
    if (v == QStringLiteral("mkcol")) return DavOperation::Mkcol;
    if (v == QStringLiteral("move")) return DavOperation::Move;
    if (v == QStringLiteral("copy")) return DavOperation::Copy;
    if (v == QStringLiteral("proppatch")) return DavOperation::PropPatch;
    if (v == QStringLiteral("propfind")) return DavOperation::PropFind;
    if (v == QStringLiteral("report")) return DavOperation::Report;

    return DavOperation::Unknown;
}

auto OpUtils::davOpToVerb(DavOperation op) -> QString
{
    switch (op)
    {
    case DavOperation::Head:
        return QStringLiteral("HEAD");
    case DavOperation::Get:
        return QStringLiteral("GET");
    case DavOperation::Put:
        return QStringLiteral("PUT");
    case DavOperation::Post:
        return QStringLiteral("POST");
    case DavOperation::Delete:
        return QStringLiteral("DELETE");
    case DavOperation::Mkcol:
        return QStringLiteral("MKCOL");
    case DavOperation::Move:
        return QStringLiteral("MOVE");
    case DavOperation::Copy:
        return QStringLiteral("COPY");
    case DavOperation::PropPatch:
        return QStringLiteral("PROPPATCH");
    case DavOperation::PropFind:
        return QStringLiteral("PROPFIND");
    case DavOperation::Report:
        return QStringLiteral("REPORT");
    default:
        return QStringLiteral("Unknown");
    }
}

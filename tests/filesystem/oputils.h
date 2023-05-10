#pragma once

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QString>

class OpUtils
{
public:
    enum class DavOperation
    {
        Unknown,
        Head,
        Get,
        Put,
        Post,
        Delete,
        Mkcol,
        Move,
        Copy,
        PropPatch,
        PropFind,
        Report
    };

    static auto qtOpToDavOp(QNetworkAccessManager::Operation op, const QNetworkRequest &req) -> DavOperation;
    static auto qtOpToVerb(QNetworkAccessManager::Operation op) -> QString;

    static auto davVerbToOp(const QString &verb) -> DavOperation;
    static auto davOpToVerb(DavOperation op) -> QString;
};

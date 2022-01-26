#pragma once

#include <QObject>
#include <QNetworkRequest>
#include "thirdparty/propertyhelper/PropertyHelper.h"

enum RequestType
{
    GET,
    PUT,
    POST,
    CUSTOM
};

class RequestContainer : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(int, id)
    READONLY_PROPERTY(QNetworkRequest, request)
    READONLY_PROPERTY(RequestType, requestType)
    READONLY_PROPERTY(QByteArray, data)
    READONLY_PROPERTY(QByteArray, verb)

public:
    explicit RequestContainer(
            const QNetworkRequest &request,
            const RequestType &requestType,
            const QByteArray &data,
            QObject *parent)
      : RequestContainer(-1, request, requestType, data, "", parent) {}

    explicit RequestContainer(
            const QNetworkRequest &request,
            const RequestType &requestType,
            const QByteArray &data,
            const QByteArray &verb,
            QObject *parent)
      : RequestContainer(-1, request, requestType, data, verb, parent) {}

    explicit RequestContainer(
            int id,
            const QNetworkRequest &request,
            const RequestType &requestType,
            const QByteArray &data,
            const QByteArray &verb,
            QObject *parent)
      : QObject(parent), a_id(id), a_request(request), a_requestType(requestType),
          a_data(data), a_verb(verb) {}
};

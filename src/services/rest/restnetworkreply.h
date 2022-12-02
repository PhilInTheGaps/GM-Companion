#pragma once

#include <QNetworkReply>
#include <QObject>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class RestNetworkReply : public QObject
{
    Q_OBJECT
public:
    RestNetworkReply(QNetworkReply::NetworkError error,
                     const QString &errorText,
                     const QByteArray& data,
                     const QList<QNetworkReply::RawHeaderPair> &headers,
                     QObject *parent)
        : QObject(parent),
          a_error(error),
          a_errorText(errorText),
          a_data(data),
          a_headers(headers) {}

    [[nodiscard]] auto getHeader(const QByteArray &name) const -> QByteArray;

    [[nodiscard]] auto hasError() const -> bool;

    READONLY_PROPERTY(QNetworkReply::NetworkError, error)
    READONLY_PROPERTY(QString, errorText)
    READONLY_PROPERTY(QByteArray, data)
    READONLY_PROPERTY(QList<QNetworkReply::RawHeaderPair>, headers)
};

#pragma once

#include <QObject>

class RestNetworkReply;

namespace Files {

class FileResult : public QObject
{
    Q_OBJECT

public:
    explicit FileResult(bool success, QObject *parent = nullptr)
        : QObject(parent), m_success(success), m_errorMessage(QLatin1String()) {}

    explicit FileResult(bool success, const QString& errorMessage, QObject *parent = nullptr)
        : QObject(parent), m_success(success), m_errorMessage(errorMessage) {}

    explicit FileResult(const QString& errorMessage, QObject *parent = nullptr)
        : FileResult(false, errorMessage, parent) {}

    static FileResult* fromNetworkReply(RestNetworkReply *reply, QObject *parent);

    bool success() const { return m_success; }
    QString errorMessage() const { return m_errorMessage; }

protected:
    const bool m_success;
    const QString m_errorMessage;
};

}

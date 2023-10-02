#pragma once

#include <QNetworkReply>

/// NetworkReply that can be used to mock API data.
/// Based on https://github.com/richmoore/qt-examples/blob/master/qcustomnetworkreply/qcustomnetworkreply.h
class MockNetworkReply : public QNetworkReply
{
    Q_OBJECT

public:
    explicit MockNetworkReply(QObject *parent = nullptr);
    explicit MockNetworkReply(int status, const QByteArray &contentType, const QByteArray &content,
                              QObject *parent = nullptr);

    void setHttpStatusCode(int code, const QByteArray &statusText = QByteArray());
    void setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value);
    void setContentType(const QByteArray &contentType);

    void setContent(const QString &content);
    void setContent(const QByteArray &content);

    void setError(QNetworkReply::NetworkError error, const QString &errorString);

    void abort() override;
    [[nodiscard]] auto bytesAvailable() const -> qint64 override;
    [[nodiscard]] auto isSequential() const -> bool override;

    [[nodiscard]] static auto successEmpty(QObject *parent) -> MockNetworkReply *;
    [[nodiscard]] static auto successGeneric(const QByteArray &data, QObject *parent) -> MockNetworkReply *;
    [[nodiscard]] static auto successJson(const QByteArray &data, QObject *parent) -> MockNetworkReply *;
    [[nodiscard]] static auto successXml(const QByteArray &data, QObject *parent) -> MockNetworkReply *;
    [[nodiscard]] static auto notFound(QObject *parent) -> MockNetworkReply *;
    [[nodiscard]] static auto badRequest(QObject *parent) -> MockNetworkReply *;
    [[nodiscard]] static auto conflictError(const QByteArray &data, QObject *parent) -> MockNetworkReply *;

protected:
    auto readData(char *data, qint64 maxSize) -> qint64 override;

private:
    void setErrorBasedOnStatusCode(int code);

    QByteArray m_content;
    qint64 m_offset = 0;
};

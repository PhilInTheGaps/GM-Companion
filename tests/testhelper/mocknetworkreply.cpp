#include "mocknetworkreply.h"
#include "thirdparty/http-status-codes/HttpStatusCodes_Qt.h"
#include <QTimer>

using namespace Qt::Literals::StringLiterals;

MockNetworkReply::MockNetworkReply(QObject *parent) : QNetworkReply(parent)
{
}

MockNetworkReply::MockNetworkReply(int status, const QByteArray &contentType, const QByteArray &content,
                                   QObject *parent)
    : QNetworkReply(parent)
{
    setHttpStatusCode(status);
    setContentType(contentType);
    setContent(content);
}

void MockNetworkReply::setHttpStatusCode(int code, const QByteArray &statusText)
{
    setAttribute(QNetworkRequest::HttpStatusCodeAttribute, code);
    setErrorBasedOnStatusCode(code);

    if (statusText.isNull()) return;

    setAttribute(QNetworkRequest::HttpReasonPhraseAttribute, statusText);
}

void MockNetworkReply::setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value)
{
    QNetworkReply::setHeader(header, value);
}

void MockNetworkReply::setContentType(const QByteArray &contentType)
{
    setHeader(QNetworkRequest::ContentTypeHeader, contentType);
}

void MockNetworkReply::setContent(const QString &content)
{
    setContent(content.toUtf8());
}

void MockNetworkReply::setContent(const QByteArray &content)
{
    m_content = content;
    m_offset = 0;

    open(ReadOnly | Unbuffered);
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));

    using namespace std::chrono;

    QTimer::singleShot(1ms, this, &QIODevice::readyRead);
    QTimer::singleShot(1ms, this, &QNetworkReply::finished);
}

void MockNetworkReply::setError(NetworkError error, const QString &errorString)
{
    QNetworkReply::setError(error, errorString);
}

void MockNetworkReply::abort()
{
}

auto MockNetworkReply::bytesAvailable() const -> qint64
{
    return m_content.size() - m_offset + QIODevice::bytesAvailable();
}

auto MockNetworkReply::isSequential() const -> bool
{
    return true;
}

auto MockNetworkReply::successEmpty(QObject *parent) -> MockNetworkReply *
{
    return new MockNetworkReply(HttpStatus::Code::NoContent, {}, {}, parent);
}

auto MockNetworkReply::successGeneric(const QByteArray &data, QObject *parent) -> MockNetworkReply *
{
    return new MockNetworkReply(HttpStatus::Code::OK, "application/octet-stream", data, parent);
}

auto MockNetworkReply::successJson(const QByteArray &data, QObject *parent) -> MockNetworkReply *
{
    return new MockNetworkReply(HttpStatus::Code::OK, "application/json", data, parent);
}

auto MockNetworkReply::successXml(const QByteArray &data, QObject *parent) -> MockNetworkReply *
{
    return new MockNetworkReply(HttpStatus::Code::OK, "application/xml", data, parent);
}

auto MockNetworkReply::notFound(QObject *parent) -> MockNetworkReply *
{
    return new MockNetworkReply(HttpStatus::Code::NotFound, "", {}, parent);
}

auto MockNetworkReply::badRequest(QObject *parent) -> MockNetworkReply *
{
    return new MockNetworkReply(HttpStatus::Code::BadRequest, "", {}, parent);
}

auto MockNetworkReply::readData(char *data, qint64 maxSize) -> qint64
{
    if (m_offset >= m_content.size()) return -1;

    const qint64 number = qMin(maxSize, m_content.size() - m_offset);
    memcpy(data, m_content.constData() + m_offset, number);
    m_offset += number;

    return number;
}

void MockNetworkReply::setErrorBasedOnStatusCode(int code)
{
    switch (code)
    {
    case HttpStatus::Code::NotFound:
        setError(NetworkError::ContentNotFoundError, u"Not found"_s);
        break;
    case HttpStatus::Code::BadRequest:
        setError(NetworkError::ProtocolInvalidOperationError, u"Bad Request"_s);
        break;
    default:
        setError(NetworkError::NoError, u""_s);
        break;
    }
}

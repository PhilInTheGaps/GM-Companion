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

auto MockNetworkReply::conflictError(const QByteArray &data, QObject *parent) -> MockNetworkReply *
{
    return new MockNetworkReply(HttpStatus::Code::Conflict, "application/octet-stream", data, parent);
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
    case HttpStatus::Code::BadRequest:
        setError(NetworkError::ProtocolInvalidOperationError, u"Bad Request"_s);
        break;
    case HttpStatus::Code::Unauthorized:
        setError(NetworkError::UnknownContentError, u"Unauthorized"_s);
        break;
    case HttpStatus::Code::PaymentRequired:
        setError(NetworkError::UnknownContentError, u"Payment Required"_s);
        break;
    case HttpStatus::Code::Forbidden:
        setError(NetworkError::ContentAccessDenied, u"Forbidden"_s);
        break;
    case HttpStatus::Code::NotFound:
        setError(NetworkError::ContentNotFoundError, u"Not Found"_s);
        break;
    case HttpStatus::Code::MethodNotAllowed:
        setError(NetworkError::ContentOperationNotPermittedError, u"Method Not Allowed"_s);
        break;
    case HttpStatus::Code::NotAcceptable:
        setError(NetworkError::UnknownContentError, u"Not Acceptable"_s);
        break;
    case HttpStatus::Code::ProxyAuthenticationRequired:
        setError(NetworkError::ProxyAuthenticationRequiredError, u"Proxy Authentication Required"_s);
        break;
    case HttpStatus::Code::RequestTimeout:
        setError(NetworkError::TimeoutError, u"Request Timeout"_s);
        break;
    case HttpStatus::Code::Conflict:
        setError(NetworkError::ContentConflictError, u"Conflict"_s);
        break;
    case HttpStatus::Code::Gone:
        setError(NetworkError::ContentGoneError, u"Gone"_s);
        break;
    case HttpStatus::Code::LengthRequired:
        setError(NetworkError::UnknownContentError, u"Length Required"_s);
        break;
    case HttpStatus::Code::PreconditionFailed:
        setError(NetworkError::UnknownContentError, u"Precondition Failed"_s);
        break;
    case HttpStatus::Code::ContentTooLarge:
        setError(NetworkError::UnknownContentError, u"Content Too Large"_s);
        break;
    case HttpStatus::Code::URITooLong:
        setError(NetworkError::UnknownContentError, u"URI Too Long"_s);
        break;
    case HttpStatus::Code::UnsupportedMediaType:
        setError(NetworkError::UnknownContentError, u"Unsupported Media Type"_s);
        break;
    case HttpStatus::Code::RangeNotSatisfiable:
        setError(NetworkError::UnknownContentError, u"Range Not Satisfiable"_s);
        break;
    case HttpStatus::Code::ExpectationFailed:
        setError(NetworkError::UnknownContentError, u"Expectation Failed"_s);
        break;
    case HttpStatus::Code::ImATeapot:
        setError(NetworkError::UnknownContentError, u"I'm A Teapot"_s);
        break;
    case HttpStatus::Code::MisdirectedRequest:
        setError(NetworkError::UnknownContentError, u"Misdirected Request"_s);
        break;
    case HttpStatus::Code::UnprocessableContent:
        setError(NetworkError::UnknownContentError, u"Unprocessable Content"_s);
        break;
    case HttpStatus::Code::Locked:
        setError(NetworkError::UnknownContentError, u"Locked"_s);
        break;
    case HttpStatus::Code::FailedDependency:
        setError(NetworkError::UnknownContentError, u"Failed Dependency"_s);
        break;
    case HttpStatus::Code::TooEarly:
        setError(NetworkError::UnknownContentError, u"Too Early"_s);
        break;
    case HttpStatus::Code::UpgradeRequired:
        setError(NetworkError::UnknownContentError, u"Upgrade Required"_s);
        break;
    case HttpStatus::Code::PreconditionRequired:
        setError(NetworkError::UnknownContentError, u"Precondition Required"_s);
        break;
    case HttpStatus::Code::TooManyRequests:
        setError(NetworkError::UnknownContentError, u"Too Many Requests"_s);
        break;
    case HttpStatus::Code::RequestHeaderFieldsTooLarge:
        setError(NetworkError::UnknownContentError, u"Request Header Fields Too Large"_s);
        break;
    case HttpStatus::Code::UnavailableForLegalReasons:
        setError(NetworkError::UnknownContentError, u"Unavailable For Legal Reasons"_s);
        break;
    case HttpStatus::Code::InternalServerError:
        setError(NetworkError::InternalServerError, u"Internal Server Error"_s);
        break;
    case HttpStatus::Code::ServiceUnavailable:
        setError(NetworkError::ServiceUnavailableError, u"Service Unavailable"_s);
        break;
    case HttpStatus::Code::NetworkAuthenticationRequired:
        setError(NetworkError::AuthenticationRequiredError, u"Network Authentication Required"_s);
        break;
    default:
        setError(NetworkError::NoError, u""_s);
        break;
    }
}

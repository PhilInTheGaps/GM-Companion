#pragma once

#include <QByteArray>
#include <QNetworkRequest>

namespace Services
{

class RestRequest
{
    Q_GADGET

public:
    enum class Type
    {
        GET,
        PUT,
        POST,
        CUSTOM
    };
    Q_ENUM(Type)

    explicit RestRequest(const QNetworkRequest &request, Type type, QByteArray data = {}, QByteArray verb = {})
        : RestRequest(-1, request, type, std::move(data), std::move(verb))
    {
    }

    explicit RestRequest(int id, const QNetworkRequest &request, Type type, QByteArray data, QByteArray verb)
        : m_id(id), m_type(type), m_request(request), m_data(std::move(data)), m_verb(std::move(verb))
    {
    }

    [[nodiscard]] auto id() const -> int
    {
        return m_id;
    }
    void id(int id)
    {
        m_id = id;
    }

    [[nodiscard]] auto type() const -> Type
    {
        return m_type;
    }

    [[nodiscard]] auto request() const -> QNetworkRequest
    {
        return m_request;
    }

    [[nodiscard]] auto data() const -> QByteArray
    {
        return m_data;
    }

    [[nodiscard]] auto verb() const -> QByteArray
    {
        return m_verb;
    }

    [[nodiscard]] auto isAuthRequired() const -> bool
    {
        return m_isAuthRequired;
    }
    void isAuthRequired(int required)
    {
        m_isAuthRequired = required;
    }

private:
    int m_id = -1;
    Type m_type;
    QNetworkRequest m_request;
    QByteArray m_data;
    QByteArray m_verb;
    bool m_isAuthRequired = true;
};

} // namespace Services

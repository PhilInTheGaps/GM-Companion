#pragma once

#include "../options.h"
#include <QByteArray>
#include <QNetworkRequest>

namespace Services
{

class RestRequest
{
public:
    enum class Type
    {
        GET,
        PUT,
        POST,
        CUSTOM
    };

    explicit RestRequest(const QNetworkRequest &request, Type type, Options options, QByteArray data = {},
                         QByteArray verb = {})
        : RestRequest(-1, request, type, options, std::move(data), std::move(verb))
    {
    }

    explicit RestRequest(int id, const QNetworkRequest &request, Type type, Options options, QByteArray data,
                         QByteArray verb)
        : m_id(id), m_type(type), m_options(options), m_request(request), m_data(std::move(data)),
          m_verb(std::move(verb))
    {
        if (m_verb.isEmpty())
        {
            switch (type)
            {
            case Type::GET:
                m_verb = "GET";
                break;
            case Type::PUT:
                m_verb = "PUT";
                break;
            case Type::POST:
                m_verb = "POST";
                break;
            default:
                break;
            }
        }
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

    [[nodiscard]] auto options() const -> Options
    {
        return m_options;
    }
    void options(Options options)
    {
        m_options = options;
    }

private:
    int m_id = -1;
    Type m_type;
    Options m_options;
    QNetworkRequest m_request;
    QByteArray m_data;
    QByteArray m_verb;
};

} // namespace Services

#include "networkutils.h"

/**
 * @brief Get a Basic Authorization header from username and password
 */
auto NetworkUtils::basicAuthHeader(QString username, QString password)->QByteArray
{
    auto credentials = username + ":" + password;
    auto data        = credentials.toUtf8().toBase64();

    return "Basic " + data;
}

#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <QString>

class NetworkUtils
{
public:
    static QByteArray basicAuthHeader(QString username, QString password);
};

#endif // NETWORKUTILS_H

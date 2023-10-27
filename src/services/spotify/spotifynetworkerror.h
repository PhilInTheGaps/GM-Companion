#pragma once

#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QByteArray>
#include <QString>

namespace Services
{

class SpotifyNetworkError
{
public:
    explicit SpotifyNetworkError(const QByteArray &data);

    explicit operator QString() const;

    READ_PROPERTY(QString, message);
    READ_PROPERTY(QString, reason);
};

} // namespace Services

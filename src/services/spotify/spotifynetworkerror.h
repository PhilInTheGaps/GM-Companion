#pragma once

#include <QByteArray>
#include <QString>
#include "thirdparty/propertyhelper/PropertyHelper.h"

class SpotifyNetworkError
{
public:
    SpotifyNetworkError(const QByteArray &data);

    operator QString() const;

    READ_PROPERTY(QString, message);
    READ_PROPERTY(QString, reason);
};


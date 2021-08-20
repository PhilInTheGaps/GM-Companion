#pragma once

#include <QString>

class SpotifyUtils
{
public:
    static int getUriType(const QString& uri);
    static QString getIdFromUri(QString uri);
    static QString getIdFromHref(const QString& href);
};

#pragma once

#include <QString>
#include <QPixmap>

struct ConstQString : public QLatin1String
{
    constexpr ConstQString(const char* const s) :
        QLatin1String(s, static_cast<int>(std::char_traits<char>::length(s))) {}
};

class StringUtils
{
public:
    static auto stringFromImage(const QPixmap &image) -> QString;
    static auto rot13(const QString &input) -> QString;
    static auto hasWildcardMatch(const QString &string, const QString &wildcard) -> bool;
};


#pragma once

#include <QImage>
#include <QLatin1StringView>
#include <QPixmap>
#include <QString>

struct ConstQString : public QLatin1String
{
    constexpr ConstQString(const char *const s) : QLatin1String(s, static_cast<int>(std::char_traits<char>::length(s)))
    {
    }
};

class StringUtils
{
public:
    /// Convert QImage/QPixmap to base64 encoded QByteArray
    template <typename T> static auto stringFromImage(const T &image) -> QByteArray
    {
        QByteArray data;
        QBuffer buffer(&data);

        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "JPEG");

        return "data:image/jpg;base64," + data.toBase64();
    }

    static auto imageFromString(const QByteArray &string) -> QImage;
    static auto pixmapFromString(const QByteArray &string) -> QPixmap;

    template <typename T> constexpr static auto removeFirst(T &string, int n) -> T &
    {
        return string.last(string.length() - n);
    }

    template <typename T> constexpr static auto removeFirst(const T &string, int n) -> T
    {
        return string.last(string.length() - n);
    }

    static auto rot13(const QString &input) -> QString;
    static auto hasWildcardMatch(const QString &string, const QString &wildcard) -> bool;

private:
    constexpr static auto JPG_BASE64_PREFIX = QLatin1StringView("data:image/jpg;base64,");
};

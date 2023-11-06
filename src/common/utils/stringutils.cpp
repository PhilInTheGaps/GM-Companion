#include "stringutils.h"
#include <QBuffer>
#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;

constexpr int ROT13_PLACES = 13;
constexpr int CENSOR_LENGTH = 4;

auto StringUtils::imageFromString(const QByteArray &string) -> QImage
{
    auto data = QByteArray::fromBase64(removeFirst(string, JPG_BASE64_PREFIX.length()));
    return QImage::fromData(data);
}

auto StringUtils::pixmapFromString(const QByteArray &string) -> QPixmap
{
    auto data = QByteArray::fromBase64(removeFirst(string, JPG_BASE64_PREFIX.length()));
    QPixmap pixmap;
    pixmap.loadFromData(data);
    return pixmap;
}

/**
 * @brief Takes a string and encrypts it in ROT13 encryption
 */
auto StringUtils::rot13(const QString &input) -> QString
{
    // This is probably really stupid, but it works:
    // The following strings are both 2 times the alphabet
    // When encrypting, a character is replaced by the one 13 spots to the right
    // The strings have to be 2 times as long as usual so that there are still
    // characters further to the right than 'z'
    const auto lowCaps = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"_L1;
    const auto upperCaps = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ"_L1;

    QString encrypted = u""_s;

    for (const auto &character : input)
    {
        if (lowCaps.contains(character))
        {
            const auto index = lowCaps.indexOf(character);
            encrypted.append(lowCaps.at(index + ROT13_PLACES));
        }
        else if (upperCaps.contains(character))
        {
            const auto index = upperCaps.indexOf(character);
            encrypted.append(upperCaps.at(index + ROT13_PLACES));
        }
        else
        {
            encrypted.append(character);
        }
    }

    return encrypted;
}

auto StringUtils::hasWildcardMatch(const QString &string, const QString &wildcard) -> bool
{
    bool hasMatch = false;

    auto re = QRegularExpression(QRegularExpression::wildcardToRegularExpression(wildcard));
    auto match = re.match(string);
    hasMatch = match.hasMatch();

    return hasMatch;
}

auto StringUtils::censor(QAnyStringView string) -> QString
{
    if (string.isEmpty()) return ""_L1;

    if (string.length() <= CENSOR_LENGTH) return string.toString();

    return string.first(CENSOR_LENGTH).toString() + u"..."_s;
}

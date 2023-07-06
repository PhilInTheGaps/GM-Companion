#include "stringutils.h"
#include <QBuffer>
#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;

constexpr int ROT13_PLACES = 13;

/**
 * @brief Convert QPixmap to base64 encoded QString
 */
auto StringUtils::stringFromImage(const QPixmap &image) -> QString
{
    QByteArray bArray;
    QBuffer buffer(&bArray);

    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG");

    auto imageString = u"data:image/jpg;base64,"_s;
    imageString.append(QString::fromLatin1(bArray.toBase64().data()));
    return imageString;
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
            const int index = lowCaps.indexOf(character);
            encrypted.append(lowCaps.at(index + ROT13_PLACES));
        }
        else if (upperCaps.contains(character))
        {
            const int index = upperCaps.indexOf(character);
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

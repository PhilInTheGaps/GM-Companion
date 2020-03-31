#include "utils.h"

#include <QBuffer>

/**
 * @brief Convert QPixmap to base64 encoded QString
 */
auto Utils::stringFromImage(const QPixmap& image)->QString
{
    QByteArray bArray;
    QBuffer    buffer(&bArray);

    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG");

    QString imageString("data:image/jpg;base64,");
    imageString.append(QString::fromLatin1(bArray.toBase64().data()));
    return imageString;
}

/**
 * @brief Takes a string and encrypts it in ROT13 encryption
 */
auto Utils::rot13(const QString& input)->QString
{
    // This is probably really stupid, but it works:
    // The following strings are both 2 times the alphabet
    // When encrypting, a character is replaced by the one 13 spots to the right
    // The strings have to be 2 times as long as usual so that there are still
    // characters further to the right than 'z'
    QString lowCaps   = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    QString upperCaps = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";

    QString encrypted = "";

    for (auto character : input)
    {
        if (lowCaps.contains(character))
        {
            int index = lowCaps.indexOf(character);
            encrypted.append(lowCaps.at(index + 13));
        }
        else if (upperCaps.contains(character))
        {
            int index = upperCaps.indexOf(character);
            encrypted.append(upperCaps.at(index + 13));
        }
        else
        {
            encrypted.append(character);
        }
    }

    return encrypted;
}

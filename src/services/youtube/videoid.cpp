#include "videoid.h"
#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr auto ID_LENGTH = 11;

VideoId::VideoId(const QString &idOrUrl) : m_id(normalize(idOrUrl))
{
}

auto Services::VideoId::toString() const -> QString
{
    return m_id;
}

auto VideoId::toUrl() const -> QString
{
    return u"https://www.youtube.com/watch?v=%1"_s.arg(m_id);
}

auto VideoId::isValid(const QString &id) -> bool
{
    if (id.isEmpty()) return false;

    // Video IDs are always 11 characters
    if (id.length() != ID_LENGTH) return false;

    static auto re = QRegularExpression(u"[^0-9a-zA-Z_\\-]"_s);
    auto match = re.match(id);
    return !match.hasMatch();
}

auto VideoId::isValid() const -> bool
{
    return isValid(m_id);
}

auto VideoId::normalize(const QString &idOrUrl) -> QString
{
    if (idOrUrl.isEmpty()) return u""_s;

    if (VideoId::isValid(idOrUrl)) return idOrUrl;

    static auto reRegular = QRegularExpression(u"(?<=(youtube\\.com)|^)\\/watch.*?v=(.*?)(?:&|\\/|$)"_s);
    auto matchRegular = reRegular.match(idOrUrl);
    if (!matchRegular.captured(2).isEmpty() && VideoId::isValid(matchRegular.captured(2)))
        return matchRegular.captured(2);

    static auto reShort = QRegularExpression(u"youtu\\.be\\/(.*?)(?:\\?|&|\\/|$)"_s);
    auto matchShort = reShort.match(idOrUrl);
    if (!matchShort.captured(1).isEmpty() && VideoId::isValid(matchShort.captured(1))) return matchShort.captured(1);

    static auto reEmbed = QRegularExpression(u"youtube\\..+?\\/embed\\/(.*?)(?:\\?|&|\\/|$)"_s);
    auto matchEmbed = reEmbed.match(idOrUrl);
    if (!matchEmbed.captured(1).isEmpty() && VideoId::isValid(matchEmbed.captured(1))) return matchEmbed.captured(1);

    return u""_s;
}

#include "playlistid.h"
#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

PlaylistId::PlaylistId(const QString &idOrUrl) : m_id(normalize(idOrUrl))
{
}

auto PlaylistId::toString() const -> QString
{
    return m_id;
}

auto PlaylistId::toUrl() const -> QString
{
    return u"https://www.youtube.com/playlist?list=%1"_s.arg(m_id);
}

auto PlaylistId::isValid(const QString &id) -> bool
{
    if (id.length() < 2) return false;

    static auto re = QRegularExpression(u"[^\\w\\d\\-_]+"_s);
    return !re.match(id).hasMatch();
}

auto PlaylistId::isValid() const -> bool
{
    return isValid(m_id);
}

auto PlaylistId::normalize(const QString &idOrUrl) -> QString
{
    if (idOrUrl.isEmpty()) return u""_s;

    if (PlaylistId::isValid(idOrUrl)) return idOrUrl;

    static auto reRegular = QRegularExpression(u"youtube\\..+?/playlist.*?list=(.*?)(?:&|/|$)"_s);
    auto matchRegular = reRegular.match(idOrUrl);
    if (!matchRegular.captured(1).isEmpty() && PlaylistId::isValid(matchRegular.captured(1)))
        return matchRegular.captured(1);

    static auto reComposite = QRegularExpression(u"youtube\\..+?/watch.*?list=(.*?)(?:&|/|$)"_s);
    auto matchComposite = reComposite.match(idOrUrl);
    if (!matchComposite.captured(1).isEmpty() && PlaylistId::isValid(matchComposite.captured(1)))
        return matchComposite.captured(1);

    static auto reShortComposite = QRegularExpression(u"youtu\\.be/.*?/.*?list=(.*?)(?:&|/|$)"_s);
    auto matchShortComposite = reShortComposite.match(idOrUrl);
    if (!matchShortComposite.captured(1).isEmpty() && PlaylistId::isValid(matchShortComposite.captured(1)))
        return matchShortComposite.captured(1);

    static auto reEmbedComposite = QRegularExpression(u"youtube\\..+?/embed/.*?/.*?list=(.*?)(?:&|/|$)"_s);
    auto matchEmbedComposite = reEmbedComposite.match(idOrUrl);
    if (!matchEmbedComposite.captured(1).isEmpty() && PlaylistId::isValid(matchEmbedComposite.captured(1)))
        return matchEmbedComposite.captured(1);

    return u""_s;
}

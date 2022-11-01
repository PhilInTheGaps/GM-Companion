#include "listnamegenerator.h"
#include <QJsonArray>
#include <QRandomGenerator>
#include <utility>

ListNameGenerator::ListNameGenerator(QObject *parent, QJsonObject json)
    : AbstractNameGenerator{parent, json[QStringLiteral("name")].toString(), getCategoryNames(json),
                            getPrefixNames(json), getSuffixNames(json)},
      m_json(std::move(json))
{
}

auto ListNameGenerator::generate(int count) -> bool
{
    if (count < 0) return false;

    QList<QStringList> list;

    for (int i = 0; i < categories().length(); i++)
    {
        list << generate(i, count);
    }

    generatedNames(list);

    return true;
}

auto ListNameGenerator::generate(int categoryIndex, int count) -> QStringList
{
    const auto enabledCategories = this->enabledCategories();
    if (!enabledCategories[categoryIndex]) return {};

    auto availableFirstNames = getAvailableFirstNames(categoryIndex);
    auto availableSurnames = getAvailableSurnames(categoryIndex);
    auto availablePrefixes = getAvailablePrefixes(activePrefix());
    auto availableSuffixes = getAvailableSuffixes(activeSuffix());

    QStringList names;
    names.reserve(count);

    for (int i = 0; i < count; i++)
    {
        auto prefix = getRandomValue(availablePrefixes);
        auto first = getRandomValue(availableFirstNames);
        auto second = getRandomValue(availableSurnames);
        auto suffix = getRandomValue(availableSuffixes);

        names << QStringLiteral("%1 %2 %3 %4").arg(prefix, first, second, suffix).trimmed();
    }

    return names;
}

auto ListNameGenerator::getAvailableFirstNames(int categoryIndex) -> QList<QStringList>
{
    return getAvailableNames(categoryIndex, QStringLiteral("firstname"));
}

auto ListNameGenerator::getAvailableSurnames(int categoryIndex) -> QList<QStringList>
{
    return getAvailableNames(categoryIndex, QStringLiteral("surname"));
}

auto ListNameGenerator::getAvailableNames(int categoryIndex, const QString &entryName) -> QList<QStringList>
{
    auto category = m_json[QStringLiteral("categories")].toArray()[categoryIndex].toObject();
    auto firstname = category[entryName].toArray();
    auto names = m_json[QStringLiteral("names")].toObject();

    QList<QStringList> availableNames;
    availableNames.reserve(firstname.count());

    for (const auto &listName : firstname)
    {
        auto jsonArray = names[listName.toString()].toArray();
        availableNames << jsonToStringList(jsonArray);
    }

    return availableNames;
}

auto ListNameGenerator::getAvailablePrefixes(int prefixIndex) -> QList<QStringList>
{
    return getAvailableSuffixesOrPrefixes(prefixIndex, QStringLiteral("prefixes"));
}

auto ListNameGenerator::getAvailableSuffixes(int suffixIndex) -> QList<QStringList>
{
    return getAvailableSuffixesOrPrefixes(suffixIndex, QStringLiteral("suffixes"));
}

auto ListNameGenerator::getAvailableSuffixesOrPrefixes(int index, const QString &field) -> QList<QStringList>
{
    if (index < 1) return {}; // index 0 == None
    index--;

    auto fieldObject = m_json[field].toObject();
    auto types = fieldObject[QStringLiteral("types")].toArray();
    auto lists = types[index].toObject()[QStringLiteral("lists")].toArray();

    QList<QStringList> availableEntries;
    availableEntries.reserve(lists.count());

    for (const auto &listName : lists)
    {
        auto jsonArray = fieldObject[listName.toString()].toArray();
        availableEntries << jsonToStringList(jsonArray);
    }

    return availableEntries;
}

auto ListNameGenerator::getCategoryNames(const QJsonObject &json) -> QStringList
{
    return getTypeNames(json[QStringLiteral("categories")].toArray());
}

auto ListNameGenerator::getPrefixNames(const QJsonObject &json) -> QStringList
{
    auto names = getTypeNames(json[QStringLiteral("prefixes")][QStringLiteral("types")].toArray());
    names.prepend(tr("None"));
    return names;
}

auto ListNameGenerator::getSuffixNames(const QJsonObject &json) -> QStringList
{
    auto names = getTypeNames(json[QStringLiteral("suffixes")][QStringLiteral("types")].toArray());
    names.prepend(tr("None"));
    return names;
}

auto ListNameGenerator::getTypeNames(const QJsonArray &array) -> QStringList
{
    QStringList list;
    list.reserve(array.count());

    for (const auto &entry : array)
    {
        list << entry.toObject()[QStringLiteral("name")].toString();
    }

    return list;
}

auto ListNameGenerator::getRandomValue(const QList<QStringList> &names) -> QString
{
    if (names.isEmpty()) return {};

    auto listIndex = QRandomGenerator::global()->bounded(names.length());
    if (names[listIndex].isEmpty()) return {};

    auto nameIndex = QRandomGenerator::global()->bounded(names[listIndex].length());
    return names[listIndex][nameIndex];
}

auto ListNameGenerator::jsonToStringList(const QJsonArray &json) -> QStringList
{
    QStringList strings;
    strings.reserve(json.count());

    for (const auto &entry : json)
    {
        strings << entry.toString();
    }

    return strings;
}

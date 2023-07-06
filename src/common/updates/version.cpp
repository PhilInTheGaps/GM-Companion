#include "version.h"
#include "utils/utils.h"
#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;

Version::Version(const QString &str)
{
    auto regex = QRegularExpression(QStringLiteral(R"((?'numbers'[\.\d]+)(-(?'suffix'[a-zA-Z]+)(?'suffixVer'\d+))?)"));
    auto match = regex.match(str);

    auto numbers = match.captured("numbers"_L1).split('.');
    m_numbers.reserve(numbers.length());
    for (const auto &number : numbers)
    {
        bool ok = false;
        int num = number.toInt(&ok);
        m_numbers << (ok ? num : 0);
    }

    auto suffix = match.captured("suffix"_L1);
    m_suffix = suffixFromString(suffix);

    auto suffixVer = match.captured("suffixVer"_L1);
    if (!suffixVer.isNull() && !suffixVer.isEmpty())
    {
        bool ok = false;
        m_suffixVer = suffixVer.toInt(&ok);
        if (!ok) m_suffixVer = 0;
    }
}

auto Version::isGreater(const Version &other) const -> bool
{
    return isGreater(other, *this);
}

auto Version::isGreater(const Version &first, const Version &second) -> bool
{
    if (auto compareNumbers = compare(first.m_numbers, second.m_numbers); compareNumbers != 0)
        return compareNumbers > 0;

    if (auto compareSuffixes = compare(first.m_suffix, second.m_suffix); compareSuffixes != 0)
        return compareSuffixes > 0;

    return first.m_suffixVer > second.m_suffixVer;
}

auto Version::compare(const QList<int> &first, const QList<int> &second) -> int
{
    if (first.isEmpty() && second.isEmpty()) return 0;

    for (int i = 0; i < first.length(); i++)
    {
        if (!Utils::isInBounds(second, i)) return 1;

        if (first[i] != second[i])
        {
            return first[i] > second[i] ? 1 : -1;
        }
    }

    // Numbers are exactly equal
    if (first.length() == second.length()) return 0;

    // The version with more numbers is greater
    return first.length() > second.length() ? 1 : -1;
}

auto Version::compare(Suffix suffix0, Suffix suffix1) -> int
{
    if (suffix0 == suffix1) return 0;

    if (static_cast<int>(suffix0) > static_cast<int>(suffix1)) return 1;

    return -1;
}

auto Version::suffixFromString(const QString &str) -> Suffix
{
    if (str.isNull() || str.isEmpty()) return Suffix::None;

    if (str == "alpha"_L1) return Suffix::Alpha;

    if (str == "beta"_L1) return Suffix::Beta;

    if (str == "rc"_L1) return Suffix::RC;

    return Suffix::Other;
}

#include "abstractnamegenerator.h"

#include <utility>

AbstractNameGenerator::AbstractNameGenerator(QObject *parent, QString name, QStringList categories,
                                             QStringList prefixes, QStringList suffixes)
    : QObject{parent}, a_name{std::move(name)}, a_categories(std::move(categories)), a_prefixes(std::move(prefixes)),
      a_suffixes(std::move(suffixes)), a_generatedNames(buildEmptyNameList()),
      a_enabledCategories(buildInitialEnabledCategoryList())
{
}

auto AbstractNameGenerator::setCategoryEnabled(int index, bool value) -> bool
{
    if (index < 0 || index >= categories().length()) return false;

    auto list = enabledCategories();
    list[index] = value;

    a_enabledCategories = list;
    emit enabledCategoriesChanged(list);

    return true;
}

auto AbstractNameGenerator::buildEmptyNameList() const -> QList<QStringList>
{
    QList<QStringList> list;
    list.reserve(categories().length());

    for (int i = 0; i < categories().length(); i++)
    {
        list << QStringList();
    }

    return list;
}

auto AbstractNameGenerator::buildInitialEnabledCategoryList() const -> QList<bool>
{
    QList<bool> list;
    list.reserve(categories().length());

    for (int i = 0; i < categories().length(); i++)
    {
        list << true;
    }

    return list;
}

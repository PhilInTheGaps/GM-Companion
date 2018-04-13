#include "itemeditor.h"

#include <QDebug>

ItemEditor::ItemEditor(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Item Editor ...";

    sManager = new SettingsManager;

    l_filePath = sManager->getSetting(Setting::shopPath) + "/CustomItems.items";

    settings = new QSettings(l_filePath, QSettings::IniFormat);
}

QStringList ItemEditor::categories()
{
    return l_categories;
}

// Update category list
void ItemEditor::updateCategories()
{
    qDebug() << "Updating Item Categories ...";

    l_categories = settings->value("categories", { "" }).toStringList();
    emit categoriesChanged();
}

// Add a Category
void ItemEditor::addCategory(QString category)
{
    qDebug() << "Adding Item Category:" << category << "...";

    if (!l_categories.contains(category))
    {
        l_categories.append(category);

        settings->setValue("categories", l_categories);

        emit categoriesChanged();
    }
}

// Update Item Lists
void ItemEditor::updateItems()
{
    qDebug() << "Updating Item List ...";

    l_itemNames.clear();
    l_itemPrices.clear();
    l_itemCategories.clear();
    l_itemDescriptions.clear();

    int count = settings->beginReadArray("items");

    for (int i = 0; i < count; i++)
    {
        settings->setArrayIndex(i);

        l_itemNames.append(settings->value("name", "").toString());
        l_itemPrices.append(settings->value("price", "").toString());
        l_itemCategories.append(settings->value("category", "").toString());
        l_itemDescriptions.append(settings->value("description", "").toString());
    }

    settings->endArray();

    emit itemsChanged();
}

// Add an Item
void ItemEditor::addItem(QString name, QString price, QString category, QString description)
{
    qDebug() << "Adding Item:" << name << "...";

    int index = l_itemNames.size();

    settings->beginWriteArray("items");
    settings->setArrayIndex(index);

    l_itemNames.append(name);
    settings->setValue("name", name);

    l_itemPrices.append(price);
    settings->setValue("price", price);

    l_itemCategories.append(category);
    settings->setValue("category", category);

    l_itemDescriptions.append(description);
    settings->setValue("description", description);

    settings->endArray();

    emit itemsChanged();
}

// Delete an Item
void ItemEditor::deleteItem(int index)
{
    qDebug() << "Deleting Item:" << l_itemNames.at(index);

    l_itemNames.removeAt(index);
    l_itemPrices.removeAt(index);
    l_itemCategories.removeAt(index);
    l_itemDescriptions.removeAt(index);

    settings->beginWriteArray("items");

    for (int i = 0; i < l_itemNames.size(); i++)
    {
        settings->setArrayIndex(i);

        settings->setValue("name",        l_itemNames.at(i));
        settings->setValue("price",       l_itemPrices.at(i));
        settings->setValue("category",    l_itemCategories.at(i));
        settings->setValue("description", l_itemDescriptions.at(i));
    }

    settings->endArray();

    emit itemsChanged();
}

QStringList ItemEditor::getItemNames()
{
    return l_itemNames;
}

QStringList ItemEditor::getItemPrices()
{
    return l_itemPrices;
}

QStringList ItemEditor::getItemCategories()
{
    return l_itemCategories;
}

QStringList ItemEditor::getItemDescriptions()
{
    return l_itemDescriptions;
}

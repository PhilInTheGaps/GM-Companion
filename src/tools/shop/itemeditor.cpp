#include "itemeditor.h"

#include <QDebug>

ItemEditor::ItemEditor(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Item Editor ...";

    sManager = new SettingsManager;

    m_filePath = sManager->getSetting(Setting::shopPath) + "/CustomItems.items";

    settings = new QSettings(m_filePath, QSettings::IniFormat);
}

QStringList ItemEditor::categories()
{
    return m_categories;
}

// Update category list
void ItemEditor::updateCategories()
{
    qDebug() << "Updating Item Categories ...";

    m_categories = settings->value("categories", { "" }).toStringList();
    emit categoriesChanged();
}

// Add a Category
void ItemEditor::addCategory(QString category)
{
    qDebug() << "Adding Item Category:" << category << "...";

    if (!m_categories.contains(category))
    {
        m_categories.append(category);

        settings->setValue("categories", m_categories);

        emit categoriesChanged();
    }
}

// Update Item Lists
void ItemEditor::updateItems()
{
    qDebug() << "Updating Item List ...";

    m_itemNames.clear();
    m_itemPrices.clear();
    m_itemCategories.clear();
    m_itemDescriptions.clear();

    int count = settings->beginReadArray("items");

    for (int i = 0; i < count; i++)
    {
        settings->setArrayIndex(i);

        m_itemNames.append(settings->value("name", "").toString());
        m_itemPrices.append(settings->value("price", "").toString());
        m_itemCategories.append(settings->value("category", "").toString());
        m_itemDescriptions.append(settings->value("description", "").toString());
    }

    settings->endArray();

    emit itemsChanged();
}

// Add an Item
void ItemEditor::addItem(QString name, QString price, QString category, QString description)
{
    qDebug() << "Adding Item:" << name << "...";

    int index = m_itemNames.size();

    settings->beginWriteArray("items");
    settings->setArrayIndex(index);

    m_itemNames.append(name);
    settings->setValue("name", name);

    m_itemPrices.append(price);
    settings->setValue("price", price);

    m_itemCategories.append(category);
    settings->setValue("category", category);

    m_itemDescriptions.append(description);
    settings->setValue("description", description);

    settings->endArray();

    emit itemsChanged();
}

// Delete an Item
void ItemEditor::deleteItem(int index)
{
    qDebug() << "Deleting Item:" << m_itemNames.at(index);

    m_itemNames.removeAt(index);
    m_itemPrices.removeAt(index);
    m_itemCategories.removeAt(index);
    m_itemDescriptions.removeAt(index);

    settings->beginWriteArray("items");

    for (int i = 0; i < m_itemNames.size(); i++)
    {
        settings->setArrayIndex(i);

        settings->setValue("name",        m_itemNames.at(i));
        settings->setValue("price",       m_itemPrices.at(i));
        settings->setValue("category",    m_itemCategories.at(i));
        settings->setValue("description", m_itemDescriptions.at(i));
    }

    settings->endArray();

    emit itemsChanged();
}

QStringList ItemEditor::getItemNames()
{
    return m_itemNames;
}

QStringList ItemEditor::getItemPrices()
{
    return m_itemPrices;
}

QStringList ItemEditor::getItemCategories()
{
    return m_itemCategories;
}

QStringList ItemEditor::getItemDescriptions()
{
    return m_itemDescriptions;
}

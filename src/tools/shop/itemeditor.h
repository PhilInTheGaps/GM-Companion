#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include <QObject>
#include <QStringList>
#include <QSettings>
#include "src/settings/settingsmanager.h"

class ItemEditor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)

public:
    explicit ItemEditor(QObject *parent = nullptr);

    QStringList categories();
    Q_INVOKABLE void updateCategories();
    Q_INVOKABLE void addCategory(QString category);

    Q_INVOKABLE void updateItems();
    Q_INVOKABLE void addItem(QString name, QString price, QString category, QString description);
    Q_INVOKABLE void deleteItem(int index);

    Q_INVOKABLE QStringList getItemNames();
    Q_INVOKABLE QStringList getItemPrices();
    Q_INVOKABLE QStringList getItemCategories();
    Q_INVOKABLE QStringList getItemDescriptions();

signals:
    void categoriesChanged();
    void itemsChanged();

private:
    SettingsManager *sManager;
    QSettings *settings;

    QString l_filePath;
    QStringList l_categories;

    QStringList l_itemNames;
    QStringList l_itemPrices;
    QStringList l_itemCategories;
    QStringList l_itemDescriptions;
};

#endif // ITEMEDITOR_H

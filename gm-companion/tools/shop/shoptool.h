#ifndef SHOPTOOL_H
#define SHOPTOOL_H

#include <QObject>
#include <QStringList>

#include "gm-companion/settings/settingsmanager.h"

class ShopTool : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList projects READ projects NOTIFY projectsChanged)
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(QStringList shops READ shops NOTIFY shopsChanged)
    Q_PROPERTY(QString shopName READ shopName NOTIFY shopNameChanged)
    Q_PROPERTY(QString shopOwner READ shopOwner NOTIFY shopOwnerChanged)
    Q_PROPERTY(QString shopDescription READ shopDescription  NOTIFY shopDescriptionChanged)

    Q_PROPERTY(QStringList item_names READ item_names NOTIFY itemsChanged)
    Q_PROPERTY(QStringList item_prices READ item_prices NOTIFY itemsChanged)
    Q_PROPERTY(QStringList item_descriptions READ item_descriptions NOTIFY itemsChanged)

public:
    explicit ShopTool(QObject *parent = nullptr);

    QStringList projects();
    QString category();
    void setCategory(QString category);
    QStringList categories();

    QStringList shops();

    QString shopName();
    QString shopOwner();
    QString shopDescription();

    QStringList item_names();
    QStringList item_prices();
    QStringList item_descriptions();

    Q_INVOKABLE void load(QString project, QString shop);
    Q_INVOKABLE void loadCategories(QString project);
    Q_INVOKABLE void loadShops(QString project);

signals:
    void projectsChanged();
    void categoryChanged();
    void categoriesChanged();
    void shopsChanged();
    void shopNameChanged();
    void shopOwnerChanged();
    void shopDescriptionChanged();
    void itemsChanged();

private:
    SettingsManager *sManager;

    QString l_category;
    QStringList l_categories;
    QStringList l_shops;
    QString l_shopName;
    QString l_shopDescription;
    QString l_shopOwner;

    QStringList l_item_names;
    QStringList l_item_prices;
    QStringList l_item_descriptions;
};

#endif // SHOPTOOL_H

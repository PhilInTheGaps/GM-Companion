#ifndef SHOPEDITOR_H
#define SHOPEDITOR_H

#include <QObject>

#include "itemeditor.h"
#include <QQmlApplicationEngine>

class ShopEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList projectNames READ projectNames NOTIFY projectListChanged)
    Q_PROPERTY(QStringList categoryNames READ categoryNames NOTIFY projectChanged)
    Q_PROPERTY(QStringList shopNames READ shopNames NOTIFY categoryChanged)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY shopChanged)
    Q_PROPERTY(QString owner READ owner WRITE setOwner NOTIFY shopChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY shopChanged)

    Q_PROPERTY(QStringList itemGroups READ itemGroups NOTIFY itemGroupChanged)
    Q_PROPERTY(QStringList itemCategories READ itemCategories NOTIFY itemGroupChanged)

    Q_PROPERTY(bool isSaved READ isSaved NOTIFY isSavedChanged)

public:
    explicit ShopEditor(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    ItemEditor* getItemEditor() const { return itemEditor; }

    void findShops();
    void findItems();

    QStringList projectNames() const;
    QStringList categoryNames() const;
    QStringList shopNames() const;

    Q_INVOKABLE void setCurrentProject(int index);
    Q_INVOKABLE void setCurrentCategory(int index);
    Q_INVOKABLE void setCurrentShop(int index);

    QString name() const;
    void setName(QString name);

    QString owner() const;
    Q_INVOKABLE void setOwner(QString owner);

    QString description() const;
    Q_INVOKABLE void setDescription(QString description);

    Q_INVOKABLE void moveShop(int positions);
    Q_INVOKABLE void deleteShop();
    Q_INVOKABLE void deleteItem(int index);
    Q_INVOKABLE void createThing(QString name, int index);
    Q_INVOKABLE void addItem(int index);
    Q_INVOKABLE void save();

    QStringList itemGroups() const;
    QStringList itemCategories() const;

    Q_INVOKABLE void setCurrentItemGroup(int index);
    Q_INVOKABLE void enableAllItemCategories(bool b = true);
    Q_INVOKABLE void setItemCategoryEnabled(QString category, bool b = true);
    Q_INVOKABLE bool isItemCategoryEnabled(QString category) const { return !m_disabledItemCategories.contains(category); }

    bool isSaved() const { return m_isSaved; }

signals:
    void projectListChanged();
    void projectChanged();
    void categoryChanged();
    void shopChanged();

    void itemGroupChanged();
    void itemGroupsChanged();

    void isSavedChanged();
    void showInfoBar(QString message);
    void projectsSaved(QList<ShopProject*> projects);

private:
    ItemEditor *itemEditor;
    QQmlApplicationEngine *qmlEngine;
    ItemModel *itemModel;
    ItemModel *itemModel2;

    QList<ShopProject*> m_projects;
    ShopProject *m_currentProject = nullptr;

    QList<ItemGroup*> m_itemGroups;
    ItemGroup *m_currentItemGroup = nullptr;
    QStringList m_disabledItemCategories;
    QList<Item*> m_items;

    bool m_isSaved = true;
    void madeChanges();

    void createProject(QString name);
    void createCategory(QString name);
    void createShop(QString name);

private slots:
    void onShopChanged();
    void onItemsChanged();
    void itemEditorSaved(ItemGroup *group);
};

#endif // SHOPEDITOR_H

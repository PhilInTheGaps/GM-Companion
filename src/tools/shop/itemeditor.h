#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include "tools/shop/shopproject.h"
#include "filesystem/file.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

#include <QObject>
#include <QStringList>
#include <QQmlApplicationEngine>

class ItemEditor : public QObject
{
    Q_OBJECT
    READ_PROPERTY(QStringList, categories)
    AUTO_PROPERTY(bool, isSaved)

public:
    explicit ItemEditor(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    void findItems();

    Q_INVOKABLE void addCategory(const QString &name);
    Q_INVOKABLE void addItem(const QString &name, const QString &price, const QString &category, QString description);
    Q_INVOKABLE void deleteItem(int index);

    Q_INVOKABLE void save();

signals:
    void showInfoBar(const QString &message);
    void itemsSaved(ItemGroup *group);

private:
    QQmlApplicationEngine *qmlEngine;
    ItemModel *itemModel;

    ItemGroup *m_itemGroup = nullptr;

    void updateCategories();
    void updateItemModel();
    void madeChanges();
};

#endif // ITEMEDITOR_H

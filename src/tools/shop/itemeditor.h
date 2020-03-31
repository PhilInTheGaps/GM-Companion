#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include "tools/shop/shopproject.h"

#include <QObject>
#include <QStringList>
#include <QSettings>
#include <QQmlApplicationEngine>

class ItemEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(bool isSaved READ isSaved NOTIFY isSavedChanged)

public:
    explicit ItemEditor(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    void findItems();

    QStringList categories() const { return m_categories; }

    Q_INVOKABLE void addCategory(QString name);
    Q_INVOKABLE void addItem(QString name, QString price, QString category, QString description);
    Q_INVOKABLE void deleteItem(int index);

    Q_INVOKABLE void save();
    bool isSaved() const { return m_isSaved; }

signals:
    void categoriesChanged();
    void isSavedChanged();
    void showInfoBar(QString message);
    void itemsSaved(ItemGroup *group);

private:
    QSettings *settings;
    QQmlApplicationEngine *qmlEngine;
    ItemModel *itemModel;

    ItemGroup *m_itemGroup = nullptr;
    QStringList m_categories;

    void updateCategories();
    void updateItemModel();
    void madeChanges();

    bool m_isSaved = true;

};

#endif // ITEMEDITOR_H

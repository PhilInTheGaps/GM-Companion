#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include <QObject>
#include <QStringList>
#include <QSettings>
#include <QQmlApplicationEngine>
#include "managers/filemanager.h"

class ItemEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(bool isSaved READ isSaved NOTIFY isSavedChanged)

public:
    explicit ItemEditor(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent = nullptr);

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
    FileManager *fileManager;
    QQmlApplicationEngine *qmlEngine;
    ItemModel *itemModel;

    ItemGroup *m_itemGroup = nullptr;
    QStringList m_categories;

    void updateCategories();
    void updateItemModel();
    void madeChanges();

    bool m_isSaved = true;

private slots:
    void receivedItems(ItemGroup *group);

};

#endif // ITEMEDITOR_H

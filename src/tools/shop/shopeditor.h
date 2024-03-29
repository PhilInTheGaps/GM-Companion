#pragma once

#include "baseshoptool.h"
#include "itemeditor.h"
#include "project/shopproject.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QSortFilterProxyModel>
#include <QtQml/qqmlregistration.h>

namespace Files
{
class FileListResult;
}

class ShopEditor : public BaseShopTool
{
    Q_OBJECT
    QML_ELEMENT

    AUTO_LIST_PROPERTY(ItemGroup, itemGroups)
    AUTO_PROPERTY_VAL2(ItemGroup *, currentItemGroup, nullptr)
    READ_PROPERTY(QStringList, disabledItemCategories)

    AUTO_PROPERTY_VAL2(bool, isSaved, true)

public:
    explicit ShopEditor(QObject *parent = nullptr);

    enum class Type
    {
        Project,
        Category,
        Shop
    };
    Q_ENUM(Type)

    Q_PROPERTY(ItemEditor *itemEditor READ itemEditor CONSTANT)
    [[nodiscard]] auto itemEditor() -> ItemEditor *;

    Q_PROPERTY(ItemModel *itemModelShop READ itemModelShop CONSTANT)
    [[nodiscard]] auto itemModelShop() -> ItemModel *;

    Q_PROPERTY(QSortFilterProxyModel *itemModelGroup READ itemModelGroup CONSTANT)
    [[nodiscard]] auto itemModelGroup() -> QSortFilterProxyModel *;

    Q_INVOKABLE bool createThing(const QString &name, ShopEditor::Type type);

    Q_INVOKABLE bool deleteProject(ShopProject *project);
    Q_INVOKABLE bool deleteCategory(ShopCategory *category) const;

    Q_INVOKABLE bool moveShop(int positions) const;
    Q_INVOKABLE bool deleteShop() const;

    Q_INVOKABLE bool addItem(int index);
    Q_INVOKABLE bool deleteItem(int index);

    Q_INVOKABLE void save();

    Q_INVOKABLE void enableAllItemCategories(bool enable = true);
    Q_INVOKABLE void setItemCategoryEnabled(const QString &category, bool enable = true);
    [[nodiscard]] Q_INVOKABLE bool isItemCategoryEnabled(const QString &category) const;

public slots:
    void loadData() override;

signals:
    void showInfoBar(const QString &message);
    void projectsSaved(QList<ShopProject *> projects);

private:
    ItemEditor m_itemEditor;
    ItemModel m_itemModelShop = ItemModel(false, nullptr);
    ItemModel m_itemModelGroup = ItemModel(false, nullptr);
    QSortFilterProxyModel m_itemModelGroupProxy = QSortFilterProxyModel();

    void findItems();
    void madeChanges();

    auto createProject(const QString &name) -> bool;
    auto createCategory(const QString &name) -> bool;
    auto createShop(const QString &name) -> bool;

    void connectProject(const ShopProject *project) const;
    void sendProjectCopiesToTool();

    [[nodiscard]] auto isCurrentShopValid() const -> bool;
    [[nodiscard]] auto currentShop() const -> ItemShop *;

    void onItemFilesFound(Files::FileListResult &&result);

private slots:
    void itemEditorSaved(ItemGroup *group);
    void onProjectWasEdited();
    void onCurrentProjectChanged(const ShopProject *currentProject);
    void onCurrentShopChanged(const ItemShop *currentShop);
    void onCurrentItemGroupChanged(const ItemGroup *currentGroup);
    void onDisabledItemCategoriesChanged(const QStringList &categories);
    void onIsLoadingChanged(bool isLoading) const;
};

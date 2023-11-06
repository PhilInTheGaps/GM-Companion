#pragma once

#include "abstracttool.h"
#include "project/itemgroup.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QObject>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

namespace Files
{
class FileCheckResult;
class FileDataResult;
} // namespace Files

class ItemEditor : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ItemModel *itemModel READ itemModel CONSTANT)
    READ_PROPERTY(QStringList, categories)
    AUTO_PROPERTY_VAL2(bool, isSaved, true)

public:
    using AbstractTool::AbstractTool;

    [[nodiscard]] auto itemModel() -> ItemModel *;

    Q_INVOKABLE bool addCategory(const QString &name);
    Q_INVOKABLE bool addItem(const QString &name, const QString &price, const QString &category, QString description);
    Q_INVOKABLE bool deleteItem(int index);

    Q_INVOKABLE void save();

public slots:
    void loadData() override;

signals:
    void showInfoBar(const QString &message);
    void itemsSaved(ItemGroup *group);

private:
    ItemModel m_itemModel = ItemModel(true, nullptr);

    void updateCategories(const ItemGroup &group);
    void initItemModel(const ItemGroup &group);
    void madeChanges();

    static auto defaultGroupName() -> QString;

    void onFileCheckReceived(const Files::FileCheckResult &checkResult);
    void onDataReceived(const Files::FileDataResult &result);
};

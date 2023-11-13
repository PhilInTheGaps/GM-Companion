#include "settings/settingsmanager.h"
#include "src/tools/shop/shopeditor.h"
#include "src/tools/shop/shoptool.h"
#include "tests/testhelper/staticabstracttest.h"
#include "utils/fileutils.h"
#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Common::Settings;

class ShopEditorTest : public StaticAbstractTest
{
public:
    static void SetUpTestSuite()
    {
        origCloudMode = SettingsManager::instance()->get(QStringLiteral("cloudMode"), QStringLiteral("local"));
        SettingsManager::instance()->set(QStringLiteral("cloudMode"), QStringLiteral("local"));

        userPath = SettingsManager::getPath(QStringLiteral("shops"));
        backupPath = backupUserFolder(userPath, tempDir);

        copyResourceToFile(QStringLiteral(":/resources/shopproject/project.shop"),
                           FileUtils::fileInDir(QStringLiteral("project.shop"), userPath));
        copyResourceToFile(QStringLiteral(":/resources/shopproject/group.items"),
                           FileUtils::fileInDir(QStringLiteral("group.items"), userPath));

        // wait for tool to finish loading data
        tool.loadData();
        QSignalSpy spyTool(&tool, &ShopEditor::isLoadingChanged);
        spyTool.wait();
        EXPECT_FALSE(tool.isLoading());
        EXPECT_TRUE(tool.isDataLoaded());

        // load editor data
        editor = tool.editor();
        EXPECT_TRUE(editor);

        EXPECT_FALSE(editor->isDataLoaded());
        editor->loadData();

        QSignalSpy spyEditor(editor, &ShopEditor::isLoadingChanged);
        EXPECT_TRUE(spyEditor.wait());

        if (!editor->currentItemGroup())
        {
            QSignalSpy spyEditor2(editor, &ShopEditor::itemGroupsChanged);
            EXPECT_TRUE(spyEditor2.wait());
        }

        EXPECT_FALSE(editor->isLoading());
        EXPECT_TRUE(editor->isDataLoaded());
    }

    static void TearDownTestSuite()
    {
        SettingsManager::instance()->set(QStringLiteral("cloudMode"), origCloudMode);
        restoreUserFolder(backupPath, userPath);
    }

protected:
    inline static QString origCloudMode;
    inline static QString backupPath;
    inline static QString userPath;
    inline static QTemporaryDir tempDir;

    inline static ShopTool tool = ShopTool(nullptr);
    inline static ShopEditor *editor = nullptr;
};

TEST_F(ShopEditorTest, VerifyComponents)
{
    EXPECT_TRUE(editor);
    EXPECT_TRUE(editor->itemModelShop());
    EXPECT_TRUE(editor->itemModelGroup());
}

TEST_F(ShopEditorTest, CanEditShops)
{
    ASSERT_TRUE(editor);
    EXPECT_TRUE(editor->currentProject());
    EXPECT_TRUE(editor->currentProject()->currentCategory());
    EXPECT_TRUE(editor->isSaved());

    // can not add invalid shop
    EXPECT_FALSE(editor->createThing(QLatin1String(), ShopEditor::Type::Shop));
    EXPECT_TRUE(editor->isSaved());

    // can create shop
    const auto shopCount = editor->currentProject()->currentCategory()->shops().count();
    EXPECT_TRUE(editor->createThing(QStringLiteral("test-shop"), ShopEditor::Type::Shop));
    EXPECT_EQ(editor->currentProject()->currentCategory()->shops().count(), shopCount + 1);
    EXPECT_EQ(editor->currentProject()->currentCategory()->shops().constLast()->name(), QStringLiteral("test-shop"));
    auto *shop = editor->currentProject()->currentCategory()->shops().constLast();
    editor->currentProject()->currentCategory()->currentShop(shop);
    EXPECT_FALSE(editor->isSaved());
    editor->isSaved(true);

    // can not move shop to invalid positions
    EXPECT_FALSE(editor->moveShop(1));
    EXPECT_FALSE(editor->moveShop(-(shopCount + 1)));
    EXPECT_TRUE(editor->isSaved());

    // can move to valid position
    EXPECT_TRUE(editor->moveShop(-1));
    EXPECT_EQ(editor->currentProject()->currentCategory()->shops().at(shopCount - 1)->name(),
              QStringLiteral("test-shop"));
    EXPECT_FALSE(editor->isSaved());
    editor->isSaved(true);

    // can delete shop
    EXPECT_TRUE(editor->deleteShop());
    EXPECT_FALSE(editor->isSaved());
    editor->isSaved(true);

    // should change to the next valid shop
    EXPECT_TRUE(editor->currentProject()->currentCategory()->currentShop());

    // can not delete invalid shop
    editor->currentProject()->currentCategory()->currentShop(nullptr);
    EXPECT_FALSE(editor->deleteShop());
    EXPECT_TRUE(editor->isSaved());
}

TEST_F(ShopEditorTest, CanEditShopItems)
{
    ASSERT_TRUE(editor);

    editor->createThing(QStringLiteral("project"), ShopEditor::Type::Project);
    auto *project = editor->projects().constLast();
    EXPECT_EQ(project->name(), QStringLiteral("project"));

    editor->currentProject(project);
    editor->createThing(QStringLiteral("category"), ShopEditor::Type::Category);
    EXPECT_TRUE(project->currentCategory());
    EXPECT_EQ(project->currentCategory()->name(), QStringLiteral("category"));

    editor->createThing(QStringLiteral("shop"), ShopEditor::Type::Shop);
    auto *shop = project->currentCategory()->currentShop();

    EXPECT_FALSE(editor->addItem(-1));
    EXPECT_TRUE(shop->items().isEmpty());

    auto item0 = Item("item0", "0", "desc", "cat0", nullptr);
    auto item1 = Item("item1", "0", "desc", "cat1", nullptr);
    auto item2 = Item("item2", "0", "desc", "cat0", nullptr);

    auto group = ItemGroup(QStringLiteral("items"), QList<Item *>({&item0, &item1, &item2}), nullptr);
    editor->currentItemGroup(&group);

    editor->setItemCategoryEnabled(QStringLiteral("cat0"), false);
    EXPECT_TRUE(editor->disabledItemCategories().contains(QStringLiteral("cat0")));
    EXPECT_TRUE(editor->addItem(0));
    EXPECT_EQ(shop->items().count(), 1);
    EXPECT_EQ(shop->items().at(0)->name(), QStringLiteral("item1"));

    editor->enableAllItemCategories();
    editor->setItemCategoryEnabled(QStringLiteral("cat1"), false);
    EXPECT_TRUE(editor->addItem(0));
    EXPECT_EQ(shop->items().count(), 2);
    EXPECT_EQ(shop->items().at(1)->name(), QStringLiteral("item0"));

    EXPECT_TRUE(editor->addItem(1));
    EXPECT_EQ(shop->items().count(), 3);
    EXPECT_EQ(shop->items().at(2)->name(), QStringLiteral("item2"));

    EXPECT_FALSE(editor->deleteItem(-1));
    EXPECT_FALSE(editor->deleteItem(3));

    editor->setItemCategoryEnabled(QStringLiteral("cat1"), false);
    EXPECT_EQ(editor->itemModelGroup()->rowCount(), 2);
    editor->setItemCategoryEnabled(QStringLiteral("cat0"), false);
    EXPECT_EQ(editor->itemModelGroup()->rowCount(), 0);
    editor->enableAllItemCategories();
    EXPECT_EQ(editor->itemModelGroup()->rowCount(), 3);

    EXPECT_TRUE(editor->deleteItem(2));
    EXPECT_TRUE(editor->deleteItem(1));
    EXPECT_TRUE(editor->deleteItem(0));

    EXPECT_TRUE(editor->deleteShop());

    EXPECT_FALSE(editor->deleteCategory(nullptr));
    EXPECT_TRUE(editor->deleteCategory(project->currentCategory()));

    EXPECT_FALSE(editor->deleteProject(nullptr));
    EXPECT_TRUE(editor->deleteProject(project));

    editor->currentItemGroup(nullptr);
    editor->isSaved(true);
}

TEST_F(ShopEditorTest, CanEnableCategories)
{
    ASSERT_TRUE(editor);

    editor->setItemCategoryEnabled(QStringLiteral("cat0"), false);
    editor->setItemCategoryEnabled(QStringLiteral("cat1"), false);
    editor->setItemCategoryEnabled(QStringLiteral("cat2"), false);

    EXPECT_FALSE(editor->isItemCategoryEnabled(QStringLiteral("cat0")));
    EXPECT_FALSE(editor->isItemCategoryEnabled(QStringLiteral("cat1")));
    EXPECT_FALSE(editor->isItemCategoryEnabled(QStringLiteral("cat2")));

    editor->setItemCategoryEnabled(QStringLiteral("cat0"));
    EXPECT_TRUE(editor->isItemCategoryEnabled(QStringLiteral("cat0")));

    editor->enableAllItemCategories();
    EXPECT_TRUE(editor->isItemCategoryEnabled(QStringLiteral("cat0")));
    EXPECT_TRUE(editor->isItemCategoryEnabled(QStringLiteral("cat1")));
    EXPECT_TRUE(editor->isItemCategoryEnabled(QStringLiteral("cat2")));
}

TEST_F(ShopEditorTest, CanSave)
{
    ASSERT_TRUE(editor);

    editor->createThing(QStringLiteral("new-project"), ShopEditor::Type::Project);
    EXPECT_FALSE(editor->isSaved());

    QSignalSpy spy(editor, &ShopEditor::isSavedChanged);
    editor->save();
    EXPECT_TRUE(spy.wait());

    EXPECT_TRUE(editor->isSaved());
    EXPECT_EQ(tool.projects().count(), editor->projects().count());
}

TEST_F(ShopEditorTest, CanEditItems)
{
    auto *itemEditor = editor->itemEditor();
    EXPECT_FALSE(itemEditor->isLoading());
    EXPECT_TRUE(itemEditor->isSaved());

    auto categoryCount = itemEditor->categories().count();
    EXPECT_TRUE(itemEditor->addCategory(QStringLiteral("cat0")));
    EXPECT_EQ(itemEditor->categories().count(), categoryCount + 1);
    EXPECT_FALSE(itemEditor->isSaved());
    itemEditor->isSaved(true);
    categoryCount++;

    EXPECT_FALSE(itemEditor->addCategory(QStringLiteral("cat0")));
    EXPECT_EQ(itemEditor->categories().count(), categoryCount);
    EXPECT_TRUE(itemEditor->isSaved());

    EXPECT_FALSE(itemEditor->addItem("", "", "cat0", ""));
    EXPECT_FALSE(itemEditor->addItem("item0", "", "", ""));
    EXPECT_TRUE(itemEditor->isSaved());

    int itemCount = itemEditor->itemModel()->rowCount();
    EXPECT_TRUE(itemEditor->addItem("item0", "", "cat0", ""));
    EXPECT_EQ(itemEditor->itemModel()->rowCount(), ++itemCount);
    EXPECT_FALSE(itemEditor->isSaved());
    itemEditor->isSaved(true);

    EXPECT_TRUE(itemEditor->addItem("item1", "", "cat1", ""));
    EXPECT_TRUE(itemEditor->addItem("item2", "price2", "cat0", "desc2"));
    itemCount += 2;
    EXPECT_FALSE(itemEditor->isSaved());
    itemEditor->isSaved(true);

    EXPECT_EQ(qobject_cast<Item *>(itemEditor->itemModel()->get(itemCount - 2))->name(), QStringLiteral("item2"));

    const auto index = itemEditor->itemModel()->index(itemCount - 2);
    EXPECT_EQ(itemEditor->itemModel()->data(index, Qt::DisplayRole), QStringLiteral("item2"));
    EXPECT_EQ(itemEditor->itemModel()->data(index, Qt::ToolTipRole), QVariant());
    EXPECT_EQ(itemEditor->itemModel()->data(index, static_cast<int>(ItemModel::Roles::Name)), QStringLiteral("item2"));
    EXPECT_EQ(itemEditor->itemModel()->data(index, static_cast<int>(ItemModel::Roles::Price)),
              QStringLiteral("price2"));
    EXPECT_EQ(itemEditor->itemModel()->data(index, static_cast<int>(ItemModel::Roles::Description)),
              QStringLiteral("desc2"));
    EXPECT_EQ(itemEditor->itemModel()->data(index, static_cast<int>(ItemModel::Roles::Category)),
              QStringLiteral("cat0"));

    EXPECT_FALSE(itemEditor->deleteItem(-1));
    EXPECT_FALSE(itemEditor->deleteItem(itemCount));
    EXPECT_TRUE(itemEditor->isSaved());

    EXPECT_TRUE(itemEditor->deleteItem(--itemCount));
    EXPECT_EQ(itemEditor->itemModel()->rowCount(), itemCount);
    EXPECT_FALSE(itemEditor->isSaved());
    itemEditor->isSaved(true);
}

TEST_F(ShopEditorTest, CanSaveItems)
{
    auto *itemEditor = editor->itemEditor();
    EXPECT_TRUE(itemEditor->isSaved());

    EXPECT_TRUE(itemEditor->addCategory(QStringLiteral("save_cat")));
    EXPECT_TRUE(
        itemEditor->addItem(QStringLiteral("save_item"), QLatin1String(), QStringLiteral("save_cat"), QLatin1String()));
    EXPECT_FALSE(itemEditor->isSaved());

    QSignalSpy spy(itemEditor, &ItemEditor::isSavedChanged);
    EXPECT_TRUE(spy.isValid());

    itemEditor->save();

    EXPECT_TRUE(spy.wait());
    EXPECT_TRUE(itemEditor->isSaved());

    // have items been copied to shop editor?
    bool containsItem = false;

    foreach (const auto *group, editor->itemGroups())
    {
        foreach (const auto *item, group->items())
        {
            if (item->name() == QStringLiteral("save_item") && item->category() == QStringLiteral("save_cat"))
            {
                containsItem = true;
                break;
            }
        }
    }

    EXPECT_TRUE(containsItem);

    // editor should be able load CustomItems.items now
    ItemEditor secondEditor;
    EXPECT_FALSE(secondEditor.isDataLoaded());
    secondEditor.loadData();

    QSignalSpy spy2(&secondEditor, &ShopEditor::isLoadingChanged);
    spy2.wait();

    EXPECT_FALSE(secondEditor.isLoading());
    EXPECT_TRUE(secondEditor.isDataLoaded());
    EXPECT_TRUE(secondEditor.categories().contains(QStringLiteral("save_cat")));
}

#include "settings/settingsmanager.h"
#include "src/tools/shop/shopeditor.h"
#include "src/tools/shop/shoptool.h"
#include "tests/testhelper/abstracttest.h"
#include "utils/fileutils.h"

class TestShopEditor : public AbstractTest
{
    Q_OBJECT
public:
    using AbstractTest::AbstractTest;

private slots:
    void initTestCase();
    void verifyComponents();
    void canEditShops();
    void canEditShopItems();
    void canEnableCategories();
    void canSave();
    void canEditItems();
    void canSaveItems();
    void cleanupTestCase();

private:
    QString origCloudMode;
    QString backupPath;
    QString userPath;

    ShopTool tool = ShopTool(nullptr, nullptr);
    ShopEditor *editor = nullptr;
};

void TestShopEditor::initTestCase()
{
    origCloudMode = SettingsManager::instance()->get(QStringLiteral("cloudMode"), QStringLiteral("local"));
    SettingsManager::instance()->set(QStringLiteral("cloudMode"), QStringLiteral("local"));

    userPath = SettingsManager::getPath(QStringLiteral("shops"));
    backupPath = backupUserFolder(userPath);

    copyResourceToFile(QStringLiteral(":/resources/shopproject/project.shop"),
                       FileUtils::fileInDir(QStringLiteral("project.shop"), userPath));
    copyResourceToFile(QStringLiteral(":/resources/shopproject/group.items"),
                       FileUtils::fileInDir(QStringLiteral("group.items"), userPath));

    tool.loadData();

    editor = tool.editor();
    QVERIFY(editor);

    QVERIFY(!editor->isDataLoaded());
    editor->loadData();

    QSignalSpy spy(editor, &ShopEditor::isLoadingChanged);
    spy.wait();

    QVERIFY(!editor->isLoading());
    QVERIFY(editor->isDataLoaded());
}

void TestShopEditor::verifyComponents()
{
    QVERIFY(editor);
    QVERIFY(editor->itemModelShop());
    QVERIFY(editor->itemModelGroup());
}

void TestShopEditor::canEditShops()
{
    QVERIFY(editor);
    QVERIFY(editor->currentProject());
    QVERIFY(editor->currentProject()->currentCategory());
    QVERIFY(editor->isSaved());

    // can not add invalid shop
    QVERIFY(!editor->createThing(QLatin1String(), ShopEditor::Type::Shop));
    QVERIFY(editor->isSaved());

    // can create shop
    const int shopCount = editor->currentProject()->currentCategory()->shops().count();
    QVERIFY(editor->createThing(QStringLiteral("test-shop"), ShopEditor::Type::Shop));
    QCOMPARE(editor->currentProject()->currentCategory()->shops().count(), shopCount + 1);
    QCOMPARE(editor->currentProject()->currentCategory()->shops().constLast()->name(), QStringLiteral("test-shop"));
    auto *shop = editor->currentProject()->currentCategory()->shops().constLast();
    editor->currentProject()->currentCategory()->currentShop(shop);
    QVERIFY(!editor->isSaved());
    editor->isSaved(true);

    // can not move shop to invalid positions
    QVERIFY(!editor->moveShop(1));
    QVERIFY(!editor->moveShop(-(shopCount + 1)));
    QVERIFY(editor->isSaved());

    // can move to valid position
    QVERIFY(editor->moveShop(-1));
    QCOMPARE(editor->currentProject()->currentCategory()->shops().at(shopCount - 1)->name(),
             QStringLiteral("test-shop"));
    QVERIFY(!editor->isSaved());
    editor->isSaved(true);

    // can delete shop
    QVERIFY(editor->deleteShop());
    QVERIFY(!editor->isSaved());
    editor->isSaved(true);

    // should change to the next valid shop
    QVERIFY(editor->currentProject()->currentCategory()->currentShop());

    // can not delete invalid shop
    editor->currentProject()->currentCategory()->currentShop(nullptr);
    QVERIFY(!editor->deleteShop());
    QVERIFY(editor->isSaved());
}

void TestShopEditor::canEditShopItems()
{
    QVERIFY(editor);

    editor->createThing(QStringLiteral("project"), ShopEditor::Type::Project);
    auto *project = editor->projects().constLast();
    QCOMPARE(project->name(), QStringLiteral("project"));

    editor->currentProject(project);
    editor->createThing(QStringLiteral("category"), ShopEditor::Type::Category);
    QVERIFY(project->currentCategory());
    QCOMPARE(project->currentCategory()->name(), QStringLiteral("category"));

    editor->createThing(QStringLiteral("shop"), ShopEditor::Type::Shop);
    auto *shop = project->currentCategory()->currentShop();

    QVERIFY(!editor->addItem(-1));
    QVERIFY(shop->items().isEmpty());

    auto item0 = Item("item0", "0", "desc", "cat0", nullptr);
    auto item1 = Item("item1", "0", "desc", "cat1", nullptr);
    auto item2 = Item("item2", "0", "desc", "cat0", nullptr);

    auto group = ItemGroup(QStringLiteral("items"), QList<Item *>({&item0, &item1, &item2}), nullptr);
    editor->currentItemGroup(&group);

    editor->setItemCategoryEnabled(QStringLiteral("cat0"), false);
    QVERIFY(editor->disabledItemCategories().contains(QStringLiteral("cat0")));
    QVERIFY(editor->addItem(0));
    QCOMPARE(shop->items().count(), 1);
    QCOMPARE(shop->items().at(0)->name(), QStringLiteral("item1"));

    editor->enableAllItemCategories();
    editor->setItemCategoryEnabled(QStringLiteral("cat1"), false);
    QVERIFY(editor->addItem(0));
    QCOMPARE(shop->items().count(), 2);
    QCOMPARE(shop->items().at(1)->name(), QStringLiteral("item0"));

    QVERIFY(editor->addItem(1));
    QCOMPARE(shop->items().count(), 3);
    QCOMPARE(shop->items().at(2)->name(), QStringLiteral("item2"));

    QVERIFY(!editor->deleteItem(-1));
    QVERIFY(!editor->deleteItem(3));

    editor->setItemCategoryEnabled(QStringLiteral("cat1"), false);
    QCOMPARE(editor->itemModelGroup()->rowCount(), 2);
    editor->setItemCategoryEnabled(QStringLiteral("cat0"), false);
    QCOMPARE(editor->itemModelGroup()->rowCount(), 0);
    editor->enableAllItemCategories();
    QCOMPARE(editor->itemModelGroup()->rowCount(), 3);

    QVERIFY(editor->deleteItem(2));
    QVERIFY(editor->deleteItem(1));
    QVERIFY(editor->deleteItem(0));

    QVERIFY(editor->deleteShop());

    QVERIFY(!editor->deleteCategory(nullptr));
    QVERIFY(editor->deleteCategory(project->currentCategory()));

    QVERIFY(!editor->deleteProject(nullptr));
    QVERIFY(editor->deleteProject(project));

    editor->currentItemGroup(nullptr);
    editor->isSaved(true);
}

void TestShopEditor::canEnableCategories()
{
    QVERIFY(editor);

    editor->setItemCategoryEnabled(QStringLiteral("cat0"), false);
    editor->setItemCategoryEnabled(QStringLiteral("cat1"), false);
    editor->setItemCategoryEnabled(QStringLiteral("cat2"), false);

    QVERIFY(!editor->isItemCategoryEnabled(QStringLiteral("cat0")));
    QVERIFY(!editor->isItemCategoryEnabled(QStringLiteral("cat1")));
    QVERIFY(!editor->isItemCategoryEnabled(QStringLiteral("cat2")));

    editor->setItemCategoryEnabled(QStringLiteral("cat0"));
    QVERIFY(editor->isItemCategoryEnabled(QStringLiteral("cat0")));

    editor->enableAllItemCategories();
    QVERIFY(editor->isItemCategoryEnabled(QStringLiteral("cat0")));
    QVERIFY(editor->isItemCategoryEnabled(QStringLiteral("cat1")));
    QVERIFY(editor->isItemCategoryEnabled(QStringLiteral("cat2")));
}

void TestShopEditor::canSave()
{
    QVERIFY(editor);

    editor->createThing(QStringLiteral("new-project"), ShopEditor::Type::Project);
    QVERIFY(!editor->isSaved());

    editor->save();

    QSignalSpy spy(editor, &ShopEditor::isSavedChanged);
    spy.wait();

    QVERIFY(editor->isSaved());

    // has tool been updated?
    QCOMPARE(tool.projects().count(), editor->projects().count());
}

void TestShopEditor::canEditItems()
{
    auto *itemEditor = editor->itemEditor();
    QVERIFY(!itemEditor->isLoading());
    QVERIFY(itemEditor->isSaved());

    int categoryCount = itemEditor->categories().count();
    QVERIFY(itemEditor->addCategory(QStringLiteral("cat0")));
    QCOMPARE(itemEditor->categories().count(), categoryCount + 1);
    QVERIFY(!itemEditor->isSaved());
    itemEditor->isSaved(true);
    categoryCount++;

    QVERIFY(!itemEditor->addCategory(QStringLiteral("cat0")));
    QCOMPARE(itemEditor->categories().count(), categoryCount);
    QVERIFY(itemEditor->isSaved());

    QVERIFY(!itemEditor->addItem("", "", "cat0", ""));
    QVERIFY(!itemEditor->addItem("item0", "", "", ""));
    QVERIFY(itemEditor->isSaved());

    int itemCount = itemEditor->itemModel()->rowCount();
    QVERIFY(itemEditor->addItem("item0", "", "cat0", ""));
    QCOMPARE(itemEditor->itemModel()->rowCount(), ++itemCount);
    QVERIFY(!itemEditor->isSaved());
    itemEditor->isSaved(true);

    QVERIFY(itemEditor->addItem("item1", "", "cat1", ""));
    QVERIFY(itemEditor->addItem("item2", "price2", "cat0", "desc2"));
    itemCount += 2;
    QVERIFY(!itemEditor->isSaved());
    itemEditor->isSaved(true);

    QCOMPARE(qobject_cast<Item *>(itemEditor->itemModel()->get(itemCount - 2))->name(), QStringLiteral("item2"));

    const auto index = itemEditor->itemModel()->index(itemCount - 2);
    QCOMPARE(itemEditor->itemModel()->data(index, Qt::DisplayRole), QStringLiteral("item2"));
    QCOMPARE(itemEditor->itemModel()->data(index, Qt::ToolTipRole), {});
    QCOMPARE(itemEditor->itemModel()->data(index, static_cast<int>(ItemModel::Roles::Name)), QStringLiteral("item2"));
    QCOMPARE(itemEditor->itemModel()->data(index, static_cast<int>(ItemModel::Roles::Price)), QStringLiteral("price2"));
    QCOMPARE(itemEditor->itemModel()->data(index, static_cast<int>(ItemModel::Roles::Description)),
             QStringLiteral("desc2"));
    QCOMPARE(itemEditor->itemModel()->data(index, static_cast<int>(ItemModel::Roles::Category)),
             QStringLiteral("cat0"));

    QVERIFY(!itemEditor->deleteItem(-1));
    QVERIFY(!itemEditor->deleteItem(itemCount));
    QVERIFY(itemEditor->isSaved());

    QVERIFY(itemEditor->deleteItem(--itemCount));
    QCOMPARE(itemEditor->itemModel()->rowCount(), itemCount);
    QVERIFY(!itemEditor->isSaved());
    itemEditor->isSaved(true);
}

void TestShopEditor::canSaveItems()
{
    auto *itemEditor = editor->itemEditor();
    QVERIFY(itemEditor->isSaved());

    QVERIFY(itemEditor->addCategory(QStringLiteral("save_cat")));
    QVERIFY(
        itemEditor->addItem(QStringLiteral("save_item"), QLatin1String(), QStringLiteral("save_cat"), QLatin1String()));
    QVERIFY(!itemEditor->isSaved());

    itemEditor->save();

    QSignalSpy spy(itemEditor, &ItemEditor::isSavedChanged);
    spy.wait();
    QVERIFY(itemEditor->isSaved());

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

    QVERIFY(containsItem);

    // editor should be able load CustomItems.items now
    ItemEditor secondEditor;
    QVERIFY(!secondEditor.isDataLoaded());
    secondEditor.loadData();

    QSignalSpy spy2(&secondEditor, &ShopEditor::isLoadingChanged);
    spy2.wait();

    QVERIFY(!secondEditor.isLoading());
    QVERIFY(secondEditor.isDataLoaded());
    QVERIFY(secondEditor.categories().contains(QStringLiteral("save_cat")));
}

void TestShopEditor::cleanupTestCase()
{
    SettingsManager::instance()->set(QStringLiteral("cloudMode"), origCloudMode);

    restoreUserFolder(backupPath, userPath);
}

QTEST_GUILESS_MAIN(TestShopEditor)
#include "testshopeditor.moc"

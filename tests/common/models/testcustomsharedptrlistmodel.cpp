#include "models/customsharedptrlistmodel.h"
#include <QAbstractItemModelTester>
#include <QList>
#include <gtest/gtest.h>

using namespace Common;

namespace CustomSharedPtrListTest
{

struct TestItem
{
    explicit TestItem(int id) : id(id)
    {
    }

    int id;
};

class TestModel : public CustomSharedPtrListModel<TestItem>
{
public:
    using CustomSharedPtrListModel::CustomSharedPtrListModel;

    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override
    {
        Q_UNUSED(role)

        const auto item = get(index.row());
        if (!item) return {};

        return item->id;
    }
};

class CustomSharedPtrListTest : public ::testing::Test
{
protected:
    static void addItems();
    static void removeItems();
    static void replaceItems();

    inline static TestModel model = TestModel(nullptr);
};

void CustomSharedPtrListTest::addItems()
{
    auto item0 = std::make_shared<TestItem>(0);
    auto item1 = std::make_shared<TestItem>(1);
    auto item2 = std::make_shared<TestItem>(2);
    auto item3 = std::make_shared<TestItem>(3);
    auto item4 = std::make_shared<TestItem>(4);

    model.append(item0);
    model.append(item1);
    EXPECT_EQ(model.rowCount(), 2);
    EXPECT_EQ(model.get(1)->id, 1);

    EXPECT_TRUE(model.insert(1, item2));
    EXPECT_EQ(model.rowCount(), 3);
    EXPECT_EQ(model.get(1)->id, 2);

    model.prepend(item3);
    EXPECT_EQ(model.rowCount(), 4);
    EXPECT_EQ(model.get(0)->id, 3);

    EXPECT_TRUE(model.insert(4, item4)) << "Could not insert at the end";
    EXPECT_EQ(model.rowCount(), 5);
    EXPECT_EQ(model.get(4)->id, 4);

    EXPECT_EQ(model.rowCount(), model.getAll().size());
}

void CustomSharedPtrListTest::removeItems()
{
    EXPECT_FALSE(model.removeRows(0, 100));

    EXPECT_TRUE(model.removeRows(2, 3));
    EXPECT_EQ(model.rowCount(), 2);

    EXPECT_TRUE(model.removeRow(0));
    EXPECT_EQ(model.rowCount(), 1);

    EXPECT_TRUE(model.removeRow(0));
    EXPECT_EQ(model.rowCount(), 0);

    EXPECT_FALSE(model.removeRow(0));
}

void CustomSharedPtrListTest::replaceItems()
{
    auto item0 = std::make_shared<TestItem>(0);
    model.append(item0);
    model.clear();
    EXPECT_EQ(model.rowCount(), 0);

    auto item1 = std::make_shared<TestItem>(1);
    model.append(item1);

    auto item2 = std::make_shared<TestItem>(2);
    auto item3 = std::make_shared<TestItem>(3);

    model.replaceAll({item2, item3});
    EXPECT_EQ(model.rowCount(), 2);
    EXPECT_EQ(model.get(0)->id, 2);
    EXPECT_EQ(model.get(1)->id, 3);
}

TEST_F(CustomSharedPtrListTest, TestModel)
{
    EXPECT_EQ(model.rowCount(), 0);
    EXPECT_EQ(model.headerData(0, Qt::Horizontal), QVariant());

    EXPECT_FALSE(model.removeRow(0));
    EXPECT_FALSE(model.removeRow(-1));

    addItems();
    removeItems();
    replaceItems();
}

TEST_F(CustomSharedPtrListTest, TestModelUsingAbstractItemModelTester)
{
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
}

} // namespace CustomSharedPtrListTest

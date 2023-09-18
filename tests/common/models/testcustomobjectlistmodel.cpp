#include "models/customobjectlistmodel.h"
#include <QList>
#include <gtest/gtest.h>

class TestModel : public CustomObjectListModel
{
    Q_OBJECT
public:
    using CustomObjectListModel::CustomObjectListModel;

    [[nodiscard]] auto data(const QModelIndex &index, int role) const -> QVariant override
    {
        Q_UNUSED(role)

        const auto *item = get(index.row());
        if (!item) return {};

        return item->objectName();
    }
};

class CustomObjectListTest : public ::testing::Test
{
protected:
    static auto createObject(const QString &name) -> QObject *;
    static void testModel(TestModel &model);
    static void addItems(TestModel &model);
    static void removeItems(TestModel &model);
    static void replaceItems(TestModel &model);

    inline static QObject root = QObject(nullptr);
    inline static TestModel owningModel = TestModel(true, nullptr);
    inline static TestModel nonOwningModel = TestModel(false, nullptr);
};

auto CustomObjectListTest::createObject(const QString &name) -> QObject *
{
    auto *object = new QObject(&root);
    object->setObjectName(name);
    return object;
}

void CustomObjectListTest::testModel(TestModel &model)
{
    EXPECT_EQ(model.rowCount(), 0);
    EXPECT_EQ(model.headerData(0, Qt::Horizontal), QVariant());

    EXPECT_FALSE(model.removeRow(0));
    EXPECT_FALSE(model.removeRow(-1));

    addItems(model);
    removeItems(model);
    replaceItems(model);
}

void CustomObjectListTest::addItems(TestModel &model)
{
    auto *item0 = createObject(QStringLiteral("item0"));
    auto *item1 = createObject(QStringLiteral("item1"));
    auto *item2 = createObject(QStringLiteral("item2"));
    auto *item3 = createObject(QStringLiteral("item3"));
    auto *item4 = createObject(QStringLiteral("item4"));

    model.append(item0);
    model.append(item1);
    EXPECT_EQ(model.rowCount(), 2);
    EXPECT_EQ(model.get(1)->objectName(), QStringLiteral("item1"));

    EXPECT_TRUE(model.insert(1, item2));
    EXPECT_EQ(model.rowCount(), 3);
    EXPECT_EQ(model.get(1)->objectName(), QStringLiteral("item2"));

    model.prepend(item3);
    EXPECT_EQ(model.rowCount(), 4);
    EXPECT_EQ(model.get(0)->objectName(), QStringLiteral("item3"));

    EXPECT_TRUE(model.insert(4, item4)) << "Could not insert at the end";
    EXPECT_EQ(model.rowCount(), 5);
    EXPECT_EQ(model.get(4)->objectName(), QStringLiteral("item4"));

    EXPECT_EQ(model.rowCount(), model.getAll().count());
}

void CustomObjectListTest::removeItems(TestModel &model)
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

void CustomObjectListTest::replaceItems(TestModel &model)
{
    auto *item0 = createObject(QStringLiteral("item0"));
    model.append(item0);
    model.clear();
    EXPECT_EQ(model.rowCount(), 0);

    auto *item1 = createObject(QStringLiteral("item1"));
    model.append(item1);

    auto *item2 = createObject(QStringLiteral("item2"));
    auto *item3 = createObject(QStringLiteral("item3"));
    QList<QObject *> list = {item2, item3};

    model.replaceAll(list);
    EXPECT_EQ(model.rowCount(), 2);
    EXPECT_EQ(model.get(0)->objectName(), QStringLiteral("item2"));
    EXPECT_EQ(model.get(1)->objectName(), QStringLiteral("item3"));
}

TEST_F(CustomObjectListTest, TestOwningModel)
{
    testModel(owningModel);
}

TEST_F(CustomObjectListTest, TestNonOwningModel)
{
    testModel(nonOwningModel);
}

#include "testcustomobjectlistmodel.moc"

#include "models/customobjectlistmodel.h"
#include <QList>
#include <QObject>
#include <QtTest>

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

class TestCustomObjectList : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

private:
    TestModel owningModel = TestModel(true, nullptr);
    TestModel nonOwningModel = TestModel(false, nullptr);

    auto createObject(const QString &name) -> QObject *;

    void testModel(TestModel &model);
    void addItems(TestModel &model);
    void removeItems(TestModel &model);
    void replaceItems(TestModel &model);

private slots:
    void testOwningModel();
    void testNonOwningModel();
};

auto TestCustomObjectList::createObject(const QString &name) -> QObject *
{
    auto *object = new QObject(this);
    object->setObjectName(name);
    return object;
}

void TestCustomObjectList::testModel(TestModel &model)
{
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(model.headerData(0, Qt::Horizontal), {});

    QVERIFY(!model.removeRow(0));
    QVERIFY(!model.removeRow(-1));

    addItems(model);
    removeItems(model);
    replaceItems(model);
}

void TestCustomObjectList::addItems(TestModel &model)
{
    auto *item0 = createObject(QStringLiteral("item0"));
    auto *item1 = createObject(QStringLiteral("item1"));
    auto *item2 = createObject(QStringLiteral("item2"));
    auto *item3 = createObject(QStringLiteral("item3"));

    model.append(item0);
    model.append(item1);
    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(model.get(1)->objectName(), QStringLiteral("item1"));

    QVERIFY(model.insert(1, item2));
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(model.get(1)->objectName(), QStringLiteral("item2"));

    model.prepend(item3);
    QCOMPARE(model.rowCount(), 4);
    QCOMPARE(model.get(0)->objectName(), QStringLiteral("item3"));

    QCOMPARE(model.rowCount(), model.getAll().count());
}

void TestCustomObjectList::removeItems(TestModel &model)
{
    QVERIFY(!model.removeRows(0, 100));

    QVERIFY(model.removeRows(2, 2));
    QCOMPARE(model.rowCount(), 2);

    QVERIFY(model.removeRow(0));
    QCOMPARE(model.rowCount(), 1);

    QVERIFY(model.removeRow(0));
    QCOMPARE(model.rowCount(), 0);

    QVERIFY(!model.removeRow(0));
}

void TestCustomObjectList::replaceItems(TestModel &model)
{
    auto *item0 = createObject(QStringLiteral("item0"));
    model.append(item0);
    model.clear();
    QCOMPARE(model.rowCount(), 0);

    auto *item1 = createObject(QStringLiteral("item1"));
    model.append(item1);

    auto *item2 = createObject(QStringLiteral("item2"));
    auto *item3 = createObject(QStringLiteral("item3"));
    QList<QObject *> list = {item2, item3};

    model.replaceAll(list);
    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(model.get(0)->objectName(), QStringLiteral("item2"));
    QCOMPARE(model.get(1)->objectName(), QStringLiteral("item3"));
}

void TestCustomObjectList::testOwningModel()
{
    testModel(owningModel);
}

void TestCustomObjectList::testNonOwningModel()
{
    testModel(nonOwningModel);
}

QTEST_GUILESS_MAIN(TestCustomObjectList)
#include "testcustomobjectlistmodel.moc"

#include "utils/utils.h"
#include <QObject>
#include <QtTest>

class TestUtils : public QObject
{
    Q_OBJECT
public:
    TestUtils() = default;

private:
    template <typename T> void isInBoundsHelper(T &list);

private slots:
    void isInBounds();
    void isInHierarchy();
    void canCopyLists();
};

template <typename T> void TestUtils::isInBoundsHelper(T &list)
{
    QVERIFY(!Utils::isInBounds(list, 0));
    QVERIFY(!Utils::isInBounds(list, 1));
    QVERIFY(!Utils::isInBounds(list, -1));
    QVERIFY(!Utils::isInBounds(list, -5));

    list.push_back(new QObject(this));
    list.push_back(new QObject(this));

    QVERIFY(Utils::isInBounds(list, 0));
    QVERIFY(Utils::isInBounds(list, 1));
    QVERIFY(!Utils::isInBounds(list, 2));
    QVERIFY(!Utils::isInBounds(list, -1));
}

void TestUtils::isInBounds()
{
    QList<QObject *> list;
    isInBoundsHelper(list);
}

void TestUtils::isInHierarchy()
{
    // hierarchy for testing
    auto *root = new QObject(nullptr);

    auto *child0 = new QObject(root);
    auto *child1 = new QObject(root);

    auto *child00 = new QObject(child0);
    auto *child01 = new QObject(child0);
    auto *child10 = new QObject(child1);

    QVERIFY(!Utils::isInHierarchy(*root, this));

    QVERIFY(Utils::isInHierarchy(*child0, root));
    QVERIFY(Utils::isInHierarchy(*child1, root));

    QVERIFY(Utils::isInHierarchy(*child00, root));
    QVERIFY(Utils::isInHierarchy(*child00, child0));
    QVERIFY(!Utils::isInHierarchy(*child00, child1));
    QVERIFY(Utils::isInHierarchy(*child01, root));

    QVERIFY(Utils::isInHierarchy(*child10, root));
    QVERIFY(Utils::isInHierarchy(*child10, child1));
    QVERIFY(!Utils::isInHierarchy(*child10, child0));

    root->deleteLater();
}

class Copyable
{
public:
    explicit Copyable(int id) : m_id(id)
    {
    }

    explicit Copyable(const Copyable &other) : Copyable(other.m_id)
    {
    }

    int m_id;
};

void TestUtils::canCopyLists()
{
    QList<Copyable *> list{new Copyable(0), new Copyable(1), new Copyable(2)};
    auto copy = Utils::copyList(list);

    QCOMPARE(copy.length(), list.length());

    for (int i = 0; i < list.length(); i++)
    {
        // Ids should match, but objects should not be the same
        QCOMPARE(copy[i]->m_id, list[i]->m_id);
        QVERIFY(copy[i] != list[i]);
    }

    // Cleanup
    while (!list.isEmpty())
    {
        delete list.takeLast();
    }

    while (!copy.isEmpty())
    {
        delete copy.takeLast();
    }
}

QTEST_APPLESS_MAIN(TestUtils)
#include "testutils.moc"

#include <QtTest>
#include <QObject>
#include "utils/utils.h"

class TestUtils : public QObject
{
    Q_OBJECT
public:
    TestUtils() = default;

private:
    template<typename T>
    void isInBoundsHelper(T &list);

private slots:
    void isInBounds();

    void isInHierarchy();
};

template<typename T>
void TestUtils::isInBoundsHelper(T &list)
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
    QList<QObject*> list;
    isInBoundsHelper(list);

    QVector<QObject*> vector;
    isInBoundsHelper(vector);
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

QTEST_APPLESS_MAIN(TestUtils)
#include "testutils.moc"

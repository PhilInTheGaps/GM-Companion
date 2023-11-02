#include "utils/utils.h"
#include <QObject>
#include <gtest/gtest.h>

template <typename T> void isInBoundsHelper(T &list)
{
    QObject root;

    EXPECT_FALSE(Utils::isInBounds(list, 0));
    EXPECT_FALSE(Utils::isInBounds(list, 1));
    EXPECT_FALSE(Utils::isInBounds(list, -1));
    EXPECT_FALSE(Utils::isInBounds(list, -5));

    list.push_back(new QObject(&root));
    list.push_back(new QObject(&root));

    EXPECT_TRUE(Utils::isInBounds(list, 0));
    EXPECT_TRUE(Utils::isInBounds(list, 1));
    EXPECT_FALSE(Utils::isInBounds(list, 2));
    EXPECT_FALSE(Utils::isInBounds(list, -1));
}

TEST(UtilsTest, IsInBoundsQList)
{
    QList<QObject *> list;
    isInBoundsHelper(list);
}

TEST(UtilsTest, IsInBoundsVector)
{
    std::vector<QObject *> vector;
    isInBoundsHelper(vector);
}

TEST(UtilsTest, IsInHierarchy)
{
    // hierarchy for testing
    QObject root;

    auto *child0 = new QObject(&root);
    auto *child1 = new QObject(&root);

    auto *child00 = new QObject(child0);
    auto *child01 = new QObject(child0);
    auto *child10 = new QObject(child1);

    EXPECT_TRUE(Utils::isInHierarchy(*child0, &root));
    EXPECT_TRUE(Utils::isInHierarchy(*child1, &root));

    EXPECT_TRUE(Utils::isInHierarchy(*child00, &root));
    EXPECT_TRUE(Utils::isInHierarchy(*child00, child0));
    EXPECT_FALSE(Utils::isInHierarchy(*child00, child1));
    EXPECT_TRUE(Utils::isInHierarchy(*child01, &root));

    EXPECT_TRUE(Utils::isInHierarchy(*child10, &root));
    EXPECT_TRUE(Utils::isInHierarchy(*child10, child1));
    EXPECT_FALSE(Utils::isInHierarchy(*child10, child0));
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

TEST(UtilsTest, CanCopyLists)
{
    QList<Copyable *> list{new Copyable(0), new Copyable(1), new Copyable(2)};
    auto copy = Utils::copyList(list);

    EXPECT_EQ(copy.length(), list.length());

    for (int i = 0; i < list.length(); i++)
    {
        // Ids should match, but objects should not be the same
        EXPECT_EQ(copy[i]->m_id, list[i]->m_id);
        EXPECT_NE(copy[i], list[i]);
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

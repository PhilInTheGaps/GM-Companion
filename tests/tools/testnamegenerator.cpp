#include "src/tools/generators/names/namegenerator.h"
#include "tests/testhelper/abstracttest.h"
#include <QDebug>
#include <gtest/gtest.h>
#include <limits>

class NameGeneratorTest : public AbstractTest
{
public:
    NameGeneratorTest()
    {
        testAddons = enableTestAddons();

        EXPECT_FALSE(ng.isDataLoaded());
        ng.loadData();
    }

    ~NameGeneratorTest()
    {
        disableTestAddons(testAddons);
    }

protected:
    NameGenerator ng = NameGenerator(nullptr);

private:
    QList<Addon *> testAddons;
};

TEST_F(NameGeneratorTest, TestNormalUse)
{
    const auto categories = ng.categories();
    EXPECT_GT(categories.length(), 2); // default category + 2 addon categories

    for (int i = 0; i < 3; i++)
    {
        qDebug() << "Testing category" << categories[i] << "...";

        // can load category
        EXPECT_TRUE(ng.loadCategory(i));

        // can select a generator
        const auto generators = ng.generators();
        EXPECT_TRUE(generators.length() > 0);
        EXPECT_FALSE(generators[0]->name().isEmpty());

        for (int j = 0; j < ng.generators().length(); j++)
        {
            qDebug() << "Testing generator" << generators[j]->name() << "...";

            // can generate names
            EXPECT_TRUE(ng.loadGenerator(j));
            EXPECT_TRUE(ng.currentGenerator()->generate(5));
            auto names = ng.currentGenerator()->generatedNames()[0];
            EXPECT_EQ(names.length(), 5);

            int namePartsCount = names.first().split(QStringLiteral(" ")).length();

            // can disable a category and only generate names from the second one
            EXPECT_TRUE(ng.currentGenerator()->setCategoryEnabled(0, false));
            EXPECT_TRUE(ng.currentGenerator()->generate(5));
            EXPECT_EQ(ng.currentGenerator()->generatedNames()[0].length(), 0);
            EXPECT_EQ(ng.currentGenerator()->generatedNames()[1].length(), 5);
            EXPECT_TRUE(ng.currentGenerator()->setCategoryEnabled(0, true));

            // can generate name with prefix
            if (ng.currentGenerator()->prefixes().length() > 1) // prefix 0 is "none"
            {
                ng.currentGenerator()->activePrefix(1);
                EXPECT_TRUE(ng.currentGenerator()->generate(1));
                EXPECT_EQ(ng.currentGenerator()->generatedNames()[0][0].split(QStringLiteral(" ")).length(),
                          namePartsCount + 1); // Should be like "Dr. Name Something"
                ng.currentGenerator()->activePrefix(0);
            }

            // can generate name with suffix
            if (ng.currentGenerator()->suffixes().length() > 1) // suffix 0 is "none"
            {
                ng.currentGenerator()->activeSuffix(1);
                EXPECT_TRUE(ng.currentGenerator()->generate(1));
                EXPECT_EQ(ng.currentGenerator()->generatedNames()[0][0].split(QStringLiteral(" ")).length(),
                          namePartsCount + 1); // Should be like "Name Something III"
            }

            // can generate name with prefix and suffix
            if (ng.currentGenerator()->prefixes().length() > 1 && ng.currentGenerator()->suffixes().length() > 1)
            {
                ng.currentGenerator()->activePrefix(1);
                ng.currentGenerator()->activeSuffix(1);
                EXPECT_TRUE(ng.currentGenerator()->generate(1));
                EXPECT_EQ(ng.currentGenerator()->generatedNames()[0][0].split(QStringLiteral(" ")).length(),
                          namePartsCount + 2); // Should be like "Prof. Name Something III"
            }
        }
    }
}

TEST_F(NameGeneratorTest, CannotLoadInvalidGenerators)
{
    const auto maxInt = std::numeric_limits<int>::max();

    EXPECT_FALSE(ng.loadCategory(-1));
    EXPECT_FALSE(ng.loadCategory(maxInt));
    EXPECT_TRUE(ng.loadCategory(0));

    EXPECT_FALSE(ng.loadGenerator(-1));
    EXPECT_FALSE(ng.loadGenerator(maxInt));
    EXPECT_TRUE(ng.loadGenerator(0));

    EXPECT_FALSE(ng.currentGenerator()->generate(-1));
}

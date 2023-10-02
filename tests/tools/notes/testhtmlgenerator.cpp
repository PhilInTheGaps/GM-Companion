#include "src/tools/notes/htmlgenerator.h"
#include "staticabstracttest.h"
#include <QString>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

TEST(HtmlGeneratorTest, CanGenerateFromMarkdown)
{
    const auto markdown = u"# Test"_s;

    auto future = HtmlGenerator::generateFromMarkdownAsync(markdown);

    StaticAbstractTest::testFutureNoAuth(future, "generateFromMarkdownAsync", [future]() {
        const auto html = future.result();
        EXPECT_TRUE(html.contains("<h1>"_L1));
        EXPECT_TRUE(html.contains("Test"_L1));
    });
}

TEST(HtmlGeneratorTest, DoesUseCorrectStrikethroughNotation)
{
    const auto markdown = u"# Test\n~~strikethrough~~"_s;

    auto future = HtmlGenerator::generateFromMarkdownAsync(markdown);

    StaticAbstractTest::testFutureNoAuth(future, "generateFromMarkdownAsync", [future]() {
        const auto html = future.result();
        EXPECT_TRUE(html.contains("<s>"_L1));
        EXPECT_TRUE(html.contains("</s>"_L1));
        EXPECT_FALSE(html.contains("<del>"_L1));
        EXPECT_FALSE(html.contains("</del>"_L1));
    });
}

TEST(HtmlGeneratorTest, DoesInsertTableStyle)
{
    const auto markdown = u"# Test\n"
                          "a | b | c\n"
                          "--|---|--\n"
                          "1 | 2 | 3"_s;

    auto future = HtmlGenerator::generateFromMarkdownAsync(markdown);

    StaticAbstractTest::testFutureNoAuth(future, "generateFromMarkdownAsync", [future]() {
        const auto html = future.result();
        EXPECT_TRUE(html.contains("<table"_L1));
        EXPECT_TRUE(html.contains("<table border="_L1));
    });
}

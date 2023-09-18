#include "utils/markdownutils.h"
#include <gtest/gtest.h>

TEST(MarkdownUtilsTest, MarkdownToHtml)
{
    auto markdown = QStringLiteral("# Test 1\n\nThis is a test.");

    EXPECT_TRUE(MarkdownUtils::markdownToHtml(markdown).contains("<h1>Test 1</h1>"));
}

#include "utils/stringutils.h"
#include <gtest/gtest.h>

struct Rot13Test
{
    const char *description;
    QString input;
    QString output;
};

TEST(StringUtilsTest, Rot13)
{
    std::vector<Rot13Test> tests = {{"alphabet lower", "abcdefghijklmnopqrstuvwxyz", "nopqrstuvwxyzabcdefghijklm"},
                                    {"alphabet upper", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "NOPQRSTUVWXYZABCDEFGHIJKLM"},
                                    {"random string 1", "DzFmnYqGVmsHcNnqDWqO", "QmSzaLdTIzfUpAadQJdB"},
                                    {"random string 2", "bqiqxJFmqwrIkhFmFEvT", "odvdkWSzdjeVxuSzSRiG"},
                                    {"random string 3", "qiFJglgZCVgiXLUcFmGF", "dvSWtytMPItvKYHpSzTS"}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(StringUtils::rot13(test.input), test.output) << test.description;
    }
}

struct WildcardTest
{
    const char *description;
    QString string;
    QString wildcard;
    bool output;
};

TEST(StringUtilsTest, HasWildcardMatch)
{
    std::vector<WildcardTest> tests = {{"mp3 file match", "file.mp3", "*.mp3", true},
                                       {"mp3 file no match", "file.mp3_", "*.mp3", false},
                                       {"doc file match", "file.doc", "*.doc", true},
                                       {"doc file no match", "file.not_a_doc", "*.doc", false}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(StringUtils::hasWildcardMatch(test.string, test.wildcard), test.output) << test.description;
    }
}

constexpr ConstQString constString = "Test String";

TEST(StringUtilsTest, CanConstructCompileTimeString)
{
    EXPECT_EQ(constString.data(), "Test String");
}

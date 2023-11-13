#include "utils/stringutils.h"
#include <QImage>
#include <QPixmap>
#include <gtest/gtest.h>

using namespace Qt::Literals::StringLiterals;

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
    std::vector<WildcardTest> tests = {{"mp3 file match", u"file.mp3"_s, u"*.mp3"_s, true},
                                       {"mp3 file no match", u"file.mp3_"_s, u"*.mp3"_s, false},
                                       {"doc file match", u"file.doc"_s, u"*.doc"_s, true},
                                       {"doc file no match", u"file.not_a_doc"_s, u"*.doc"_s, false}};

    for (const auto &test : tests)
    {
        EXPECT_EQ(StringUtils::hasWildcardMatch(test.string, test.wildcard), test.output) << test.description;
    }
}

TEST(StringUtilsTest, CanConvertJpgToStringAndBack)
{
    QImage image(64, 64, QImage::Format_RGB32);
    image.fill(QColor::fromRgba(0xA0B0C0));

    auto data = StringUtils::stringFromImage(image);
    auto image2 = StringUtils::imageFromString(data);
    EXPECT_EQ(image2.pixelColor(0, 0), image.pixelColor(0, 0));

    auto pixmap = StringUtils::pixmapFromString(data);
    auto image3 = pixmap.toImage();
    EXPECT_EQ(image3.pixelColor(0, 0), image.pixelColor(0, 0));
}

TEST(StringUtilsTest, CanCensorStrings)
{
    EXPECT_EQ(StringUtils::censor("").toStdString(), "");
    EXPECT_EQ(StringUtils::censor("1234").toStdString(), "1234");
    EXPECT_EQ(StringUtils::censor("12345").toStdString(), "1234...");
    EXPECT_EQ(StringUtils::censor("123456789abcdefghijklmnopqrstuvwxyz").toStdString(), "1234...");
    EXPECT_EQ(StringUtils::censor(R"(Yf)jH?BaGY>>N-{'!qF.y{^BkKt\~aqB:LpoBzXNffY#y>/n*@>N<Q&%n|FNs{S")").toStdString(),
              "Yf)j...");
}

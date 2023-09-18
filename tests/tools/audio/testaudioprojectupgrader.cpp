#include "src/tools/audio/audiosaveload.h"
#include "src/tools/audio/project/audioproject.h"
#include "src/tools/audio/project/audioprojectupgrader.h"
#include "src/tools/audio/thumbnails/audiothumbnail.h"
#include "tests/testhelper/abstracttest.h"

class AudioProjectUpgraderTest : public AbstractTest
{
};

TEST_F(AudioProjectUpgraderTest, CanNotUpgradeProjectWithWrongVersion)
{
    const auto projectJson = readResource(QStringLiteral(":/resources/audioproject/project.audio"));
    EXPECT_FALSE(projectJson.isEmpty()) << "Project file does not contain data";

    AudioProjectUpgrader upgrader;
    upgrader.parse(projectJson);
    const auto result = upgrader.run();
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(AudioProjectUpgraderTest, CanUpgradeProject)
{
    const auto projectJson = readResource(QStringLiteral(":/resources/audioproject/project_v3.audio"));
    EXPECT_FALSE(projectJson.isEmpty()) << "Project file does not contain data";

    AudioProjectUpgrader upgrader;
    upgrader.parse(projectJson);

    const auto result = upgrader.run();
    EXPECT_FALSE(result.isEmpty()) << "Upgraded project file does not contain data";

    const auto json = QJsonDocument::fromJson(result).object();
    const AudioProject project(json);

    EXPECT_EQ(project.version(), 4);
    EXPECT_EQ(project.name(), QStringLiteral("TestProject"));
    EXPECT_EQ(project.categories().length(), 2);
    EXPECT_EQ(project.currentCategory()->scenarios().length(), 2);

    auto *scenario = project.currentScenario();

    const auto elements = scenario->elements();
    EXPECT_EQ(elements.length(), 5);
    EXPECT_EQ(elements.at(0)->name(), QStringLiteral("Music"));
    EXPECT_EQ(elements.at(0)->mode(), AudioElement::Mode::RandomList);
    EXPECT_EQ(elements.at(0)->type(), AudioElement::Type::Music);
    EXPECT_EQ(elements.at(0)->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    EXPECT_EQ(elements.at(0)->files().length(), 1);
    EXPECT_EQ(elements.at(0)->files().at(0)->source(), AudioFile::Source::File);
    EXPECT_EQ(elements.at(0)->files().at(0)->url(), QStringLiteral("/test.mp3"));

    EXPECT_EQ(elements.at(1)->name(), QStringLiteral("Spotify"));
    EXPECT_EQ(elements.at(1)->mode(), AudioElement::Mode::RandomList);
    EXPECT_EQ(elements.at(1)->type(), AudioElement::Type::Music);
    EXPECT_EQ(elements.at(1)->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    EXPECT_EQ(elements.at(1)->files().length(), 1);
    EXPECT_EQ(elements.at(1)->files().at(0)->source(), AudioFile::Source::Spotify);
    EXPECT_EQ(elements.at(1)->files().at(0)->url(), QStringLiteral("spotify:album:test"));

    EXPECT_EQ(elements.at(2)->name(), QStringLiteral("Sounds"));
    EXPECT_EQ(elements.at(2)->type(), AudioElement::Type::Sound);
    EXPECT_EQ(elements.at(2)->mode(), AudioElement::Mode::RandomList);
    EXPECT_EQ(elements.at(2)->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    EXPECT_EQ(elements.at(2)->files().length(), 1);
    EXPECT_EQ(elements.at(2)->files().at(0)->source(), AudioFile::Source::File);
    EXPECT_EQ(elements.at(2)->files().at(0)->url(), QStringLiteral("/test.wav"));

    EXPECT_EQ(elements.at(3)->name(), QStringLiteral("Radio"));
    EXPECT_EQ(elements.at(3)->mode(), AudioElement::Mode::RandomList);
    EXPECT_EQ(elements.at(3)->type(), AudioElement::Type::Radio);
    EXPECT_EQ(elements.at(3)->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    EXPECT_EQ(elements.at(3)->files().length(), 1);
    EXPECT_EQ(elements.at(3)->files().at(0)->source(), AudioFile::Source::Web);
    EXPECT_EQ(elements.at(3)->files().at(0)->url(), QStringLiteral("http://localhost/radio"));

    EXPECT_EQ(elements.at(4)->name(), QStringLiteral("Radio Local"));
    EXPECT_EQ(elements.at(4)->mode(), AudioElement::Mode::RandomList);
    EXPECT_EQ(elements.at(4)->type(), AudioElement::Type::Radio);
    EXPECT_EQ(elements.at(4)->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    EXPECT_EQ(elements.at(4)->files().length(), 1);
    EXPECT_EQ(elements.at(4)->files().at(0)->source(), AudioFile::Source::File);
    EXPECT_EQ(elements.at(4)->files().at(0)->url(), QStringLiteral("/test.m3u"));
}

TEST_F(AudioProjectUpgraderTest, AudioSaveLoadUpgradesProject)
{
    const auto projectJson = readResource(QStringLiteral(":/resources/audioproject/project_v3.audio"));
    EXPECT_FALSE(projectJson.isEmpty()) << "Project file does not contain data";

    auto *project = AudioSaveLoad::loadProject(projectJson, nullptr);
    EXPECT_EQ(project->version(), 4);
    project->deleteLater();
}

TEST_F(AudioProjectUpgraderTest, ShouldNotUpgradeFileWithGarbageData)
{
    const QByteArray data = "Sample Text";
    expectWarning();

    AudioProjectUpgrader upgrader;
    upgrader.parse(data);
    const auto result = upgrader.run();
    EXPECT_TRUE(result.isEmpty());
}

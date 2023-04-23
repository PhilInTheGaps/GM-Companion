#include "src/tools/audio/audiosaveload.h"
#include "src/tools/audio/project/audioproject.h"
#include "src/tools/audio/project/audioprojectupgrader.h"
#include "src/tools/audio/thumbnails/audiothumbnail.h"
#include "tests/testhelper/abstracttest.h"
#include <QObject>
#include <QtTest>

class TestAudioProjectUpgrader : public AbstractTest
{
    Q_OBJECT

private slots:
    void canNotUpgradeProjectWithWrongVersion();
    void canUpgradeProject();
    void audioSaveLoadUpgradesProject();
    void shouldNotUpgradeFileWithGarbageData();
};

void TestAudioProjectUpgrader::canNotUpgradeProjectWithWrongVersion()
{
    const auto projectJson = readResource(QStringLiteral(":/resources/audioproject/project.audio"));
    QVERIFY2(!projectJson.isEmpty(), "Project file does not contain data");

    AudioProjectUpgrader upgrader;
    upgrader.parse(projectJson);
    const auto result = upgrader.run();
    QVERIFY(result.isEmpty());
}

void TestAudioProjectUpgrader::canUpgradeProject()
{
    const auto projectJson = readResource(QStringLiteral(":/resources/audioproject/project_v3.audio"));
    QVERIFY2(!projectJson.isEmpty(), "Project file does not contain data");

    AudioProjectUpgrader upgrader;
    upgrader.parse(projectJson);

    const auto result = upgrader.run();
    QVERIFY2(!result.isEmpty(), "Upgraded project file does not contain data");

    const auto json = QJsonDocument::fromJson(result).object();
    const AudioProject project(json);

    QCOMPARE(project.version(), 4);
    QCOMPARE(project.name(), QStringLiteral("TestProject"));
    QCOMPARE(project.categories().length(), 2);
    QCOMPARE(project.currentCategory()->scenarios().length(), 2);

    auto *scenario = project.currentScenario();

    const auto musicElements = scenario->elements(AudioElement::Type::Music);
    QCOMPARE(musicElements.length(), 2);
    QCOMPARE(musicElements[0]->name(), QStringLiteral("Music"));
    QCOMPARE(musicElements[0]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(musicElements[0]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(musicElements[0]->files().length(), 1);
    QCOMPARE(musicElements[0]->files()[0]->source(), AudioFile::Source::File);
    QCOMPARE(musicElements[0]->files()[0]->url(), QStringLiteral("/test.mp3"));

    QCOMPARE(musicElements[1]->name(), QStringLiteral("Spotify"));
    QCOMPARE(musicElements[1]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(musicElements[1]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(musicElements[1]->files().length(), 1);
    QCOMPARE(musicElements[1]->files()[0]->source(), AudioFile::Source::Spotify);
    QCOMPARE(musicElements[1]->files()[0]->url(), QStringLiteral("spotify:album:test"));

    const auto soundElements = scenario->elements(AudioElement::Type::Sound);
    QCOMPARE(soundElements.length(), 1);
    QCOMPARE(soundElements[0]->name(), QStringLiteral("Sounds"));
    QCOMPARE(soundElements[0]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(soundElements[0]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(soundElements[0]->files().length(), 1);
    QCOMPARE(soundElements[0]->files()[0]->source(), AudioFile::Source::File);
    QCOMPARE(soundElements[0]->files()[0]->url(), QStringLiteral("/test.wav"));

    const auto radioElements = scenario->elements(AudioElement::Type::Radio);
    QCOMPARE(radioElements.length(), 2);
    QCOMPARE(radioElements[0]->name(), QStringLiteral("Radio"));
    QCOMPARE(radioElements[0]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(radioElements[0]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(radioElements[0]->files().length(), 1);
    QCOMPARE(radioElements[0]->files()[0]->source(), AudioFile::Source::Web);
    QCOMPARE(radioElements[0]->files()[0]->url(), QStringLiteral("http://localhost/radio"));

    QCOMPARE(radioElements[1]->name(), QStringLiteral("Radio Local"));
    QCOMPARE(radioElements[1]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(radioElements[1]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(radioElements[1]->files().length(), 1);
    QCOMPARE(radioElements[1]->files()[0]->source(), AudioFile::Source::File);
    QCOMPARE(radioElements[1]->files()[0]->url(), QStringLiteral("/test.m3u"));
}

void TestAudioProjectUpgrader::audioSaveLoadUpgradesProject()
{
    const auto projectJson = readResource(QStringLiteral(":/resources/audioproject/project_v3.audio"));
    QVERIFY2(!projectJson.isEmpty(), "Project file does not contain data");

    const auto *project = AudioSaveLoad::loadProject(projectJson, this);
    QCOMPARE(project->version(), 4);
}

void TestAudioProjectUpgrader::shouldNotUpgradeFileWithGarbageData()
{
    const QByteArray data = "Sample Text";
    expectWarning();

    AudioProjectUpgrader upgrader;
    upgrader.parse(data);
    const auto result = upgrader.run();
    QVERIFY(result.isEmpty());
}

QTEST_APPLESS_MAIN(TestAudioProjectUpgrader)
#include "testaudioprojectupgrader.moc"

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

    const auto elements = scenario->elements();
    QCOMPARE(elements.length(), 5);
    QCOMPARE(elements[0]->name(), QStringLiteral("Music"));
    QCOMPARE(elements[0]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(elements[0]->type(), AudioElement::Type::Music);
    QCOMPARE(elements[0]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(elements[0]->files().length(), 1);
    QCOMPARE(elements[0]->files()[0]->source(), AudioFile::Source::File);
    QCOMPARE(elements[0]->files()[0]->url(), QStringLiteral("/test.mp3"));

    QCOMPARE(elements[1]->name(), QStringLiteral("Spotify"));
    QCOMPARE(elements[1]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(elements[1]->type(), AudioElement::Type::Music);
    QCOMPARE(elements[1]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(elements[1]->files().length(), 1);
    QCOMPARE(elements[1]->files()[0]->source(), AudioFile::Source::Spotify);
    QCOMPARE(elements[1]->files()[0]->url(), QStringLiteral("spotify:album:test"));

    QCOMPARE(elements[2]->name(), QStringLiteral("Sounds"));
    QCOMPARE(elements[2]->type(), AudioElement::Type::Sound);
    QCOMPARE(elements[2]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(elements[2]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(elements[2]->files().length(), 1);
    QCOMPARE(elements[2]->files()[0]->source(), AudioFile::Source::File);
    QCOMPARE(elements[2]->files()[0]->url(), QStringLiteral("/test.wav"));

    QCOMPARE(elements[3]->name(), QStringLiteral("Radio"));
    QCOMPARE(elements[3]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(elements[3]->type(), AudioElement::Type::Radio);
    QCOMPARE(elements[3]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(elements[3]->files().length(), 1);
    QCOMPARE(elements[3]->files()[0]->source(), AudioFile::Source::Web);
    QCOMPARE(elements[3]->files()[0]->url(), QStringLiteral("http://localhost/radio"));

    QCOMPARE(elements[4]->name(), QStringLiteral("Radio Local"));
    QCOMPARE(elements[4]->mode(), AudioElement::Mode::RandomList);
    QCOMPARE(elements[4]->type(), AudioElement::Type::Radio);
    QCOMPARE(elements[4]->thumbnail()->relativeUrl(), QStringLiteral("/test.jpg"));
    QCOMPARE(elements[4]->files().length(), 1);
    QCOMPARE(elements[4]->files()[0]->source(), AudioFile::Source::File);
    QCOMPARE(elements[4]->files()[0]->url(), QStringLiteral("/test.m3u"));
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

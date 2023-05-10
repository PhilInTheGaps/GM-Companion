#include "src/tools/audio/project/audioproject.h"
#include "tests/testhelper/abstracttest.h"
#include <QObject>
#include <QtTest>

class TestAudioProject : public AbstractTest
{
    Q_OBJECT

private:
    AudioProject *project = nullptr;
    QByteArray projectJson;

private slots:
    void initTestCase();
    void saveToJson();
    void checkProjectContents();
    void testModifications();
};

/**
 * @brief Load the audio project from json
 */
void TestAudioProject::initTestCase()
{
    // Read project file
    QFile projectFile(":/resources/audioproject/project.audio");
    QVERIFY2(projectFile.open(QIODevice::ReadOnly), "Could not open test project file");
    projectJson = projectFile.readAll();
    projectFile.close();

    QVERIFY2(!projectJson.isEmpty(), "Project file does not contain data");
    project = new AudioProject(QJsonDocument::fromJson(projectJson).object(), this);
}

/**
 * @brief Save project to json and check if the result is the same as the loaded json
 */
void TestAudioProject::saveToJson()
{
    auto newJson = project->toJson();
    QCOMPARE(newJson, QJsonDocument::fromJson(projectJson).object());
}

/**
 * @brief Check if the project has been imported correctly
 */
void TestAudioProject::checkProjectContents()
{
    // Check meta data
    QVERIFY2(project, "Project is null after constructing from json");
    QCOMPARE(project->name(), QStringLiteral("Project"));
    QCOMPARE(project->version(), 4);
    QCOMPARE(project->categories().length(), 2);
    QVERIFY2(project->currentCategory(), "Current category is null");
    QVERIFY2(project->currentScenario(), "Current scenario is null");
    QVERIFY2(project->currentCategory()->currentScenario(), "Current scenario is null");
    QVERIFY(!project->wasRenamed());
    QVERIFY(project->isSaved());

    // Check categories
    for (int i = 0; i < project->categories().length(); i++)
    {
        auto category = project->categories()[i];
        QVERIFY2(category, "Category is null");
        QCOMPARE(category->name(), QString("Category %1").arg(i));
    }

    // Check elements
    QVERIFY2(!project->elements().isEmpty(), "Project contains no elements");
    for (auto element : project->elements())
    {
        QVERIFY2(element, "Audio element is null");
    }
}

/**
 * @brief Modify the project and test if everything works correctly
 */
void TestAudioProject::testModifications()
{
    // Name
    project->oldName(project->name());
    project->name("Modified Name");
    project->wasRenamed(true);
    QCOMPARE(project->name(), QStringLiteral("Modified Name"));
    QCOMPARE(project->oldName(), QStringLiteral("Project"));
    QVERIFY(project->wasRenamed());
    QVERIFY(!project->isSaved());

    project->name("Modified Again");
    QCOMPARE(project->name(), QStringLiteral("Modified Again"));
    QCOMPARE(project->oldName(), QStringLiteral("Project"));

    // Categories
    auto categoryCount = project->categories().length();
    QVERIFY(project->addCategory(new AudioCategory("Added in test", project->name(), {}, project)));
    QVERIFY(project->addCategory(new AudioCategory("Added in test too", project->name(), {}, project)));
    QVERIFY(!project->addCategory(nullptr));
    QCOMPARE(categoryCount + 2, project->categories().length());

    // Scenarios
    auto elementCount = project->elements().count();
    auto *category = new AudioCategory("Added in test for scenarios", project->name(), {}, project);
    QVERIFY(project->addCategory(category));

    auto *scenario = new AudioScenario("Added in test", category->path(), {}, category);
    QVERIFY(category->addScenario(scenario));

    auto *subscenario = new AudioScenario("Sub added in test", scenario->path(), {}, scenario);
    auto *subscenario2 = new AudioScenario("Sub added in test 2", scenario->path(), {}, scenario);
    QVERIFY(scenario->addScenario(subscenario));
    QVERIFY(scenario->addScenario(subscenario2));

    QCOMPARE(category->scenarios().length(), 1);
    QCOMPARE(scenario->scenarios().length(), 2);
    QCOMPARE(scenario->model().length(), 2); // Only include main scenario if it contains elements

    // Elements
    auto *music1 = new AudioElement("Music1", AudioElement::Type::Music, scenario->path(), scenario);
    auto *music2 = new AudioElement("Music2", AudioElement::Type::Music, scenario->path(), subscenario);
    auto *music3 = new AudioElement("Music3", AudioElement::Type::Music, scenario->path(), subscenario2);
    QVERIFY(scenario->addElement(music1));
    QVERIFY(subscenario->addElement(music2));
    QVERIFY(subscenario2->addElement(music3));

    auto *sound1 = new AudioElement("Sound1", AudioElement::Type::Sound, scenario->path(), scenario);
    auto *radio1 = new AudioElement("Radio1", AudioElement::Type::Radio, scenario->path(), scenario);
    QVERIFY(scenario->addElement(sound1));
    QVERIFY(scenario->addElement(radio1));

    QCOMPARE(scenario->model().length(), 3); // Main scenario now has elements
    QCOMPARE(scenario->elements(false).length(), 3);
    QCOMPARE(scenario->elements(true).length(), 5);
    QCOMPARE(scenario->elements(AudioElement::Type::Music, false).length(), 1);
    QCOMPARE(scenario->elements(AudioElement::Type::Sound, false).length(), 1);
    QCOMPARE(scenario->elements(AudioElement::Type::Radio, false).length(), 1);
    QCOMPARE(elementCount + 5, project->elements().length());
}

QTEST_APPLESS_MAIN(TestAudioProject)
#include "testaudioproject.moc"

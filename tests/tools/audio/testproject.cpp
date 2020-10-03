#include <QtTest>
#include <QObject>
#include "../../../src/tools/audio/project/audioproject.h"

class TestAudioProject : public QObject
{
    Q_OBJECT

private:
    AudioProject *project = nullptr;
    QByteArray projectJson;

private slots:
    void initTestCase();
    void checkProjectContents();
    void saveToJson();
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
 * @brief TestAudioProject::loadFromJson
 */
void TestAudioProject::checkProjectContents()
{
    // Check meta data
    QVERIFY2(project, "Project is null after constructing from json");
    QCOMPARE(project->name(), "Project");
    QCOMPARE(project->version(), 4);
    QCOMPARE(project->categories().length(), 2);
    QVERIFY2(project->currentCategory(), "Current category is null");
    QVERIFY2(project->currentCategory()->currentScenario(), "Current scenario is null");

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
 * @brief Save project to json and check if the result is the same as the loaded json
 */
void TestAudioProject::saveToJson()
{
    auto newJson = project->toJson();
    QCOMPARE(newJson, QJsonDocument::fromJson(projectJson).object());
}

QTEST_APPLESS_MAIN(TestAudioProject)
#include "testproject.moc"

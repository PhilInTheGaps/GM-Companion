#include "src/tools/audio/project/audioproject.h"
#include "tests/testhelper/abstracttest.h"

class AudioProjectTest : public AbstractTest
{
public:
    explicit AudioProjectTest()
    {
        // Read project file
        QFile projectFile(":/resources/audioproject/project.audio");
        EXPECT_TRUE(projectFile.open(QIODevice::ReadOnly)) << "Could not open test project file";
        projectJson = projectFile.readAll();
        projectFile.close();

        EXPECT_FALSE(projectJson.isEmpty()) << "Project file does not contain data";
        project = std::make_unique<AudioProject>(QJsonDocument::fromJson(projectJson).object(), nullptr);
    }

protected:
    std::unique_ptr<AudioProject> project = nullptr;
    QByteArray projectJson;
};

/**
 * @brief Save project to json and check if the result is the same as the loaded json
 */
TEST_F(AudioProjectTest, SaveToJson)
{
    auto newJson = project->toJson();
    EXPECT_EQ(newJson, QJsonDocument::fromJson(projectJson).object());
}

/**
 * @brief Check if the project has been imported correctly
 */
TEST_F(AudioProjectTest, CheckProjectContents)
{
    // Check meta data
    EXPECT_TRUE(project) << "Project is null after constructing from json";
    EXPECT_EQ(project->name(), QStringLiteral("Project"));
    EXPECT_EQ(project->version(), 4);
    EXPECT_EQ(project->categories().length(), 2);
    EXPECT_TRUE(project->currentCategory()) << "Current category is null";
    EXPECT_TRUE(project->currentScenario()) << "Current scenario is null";
    EXPECT_TRUE(project->currentCategory()->currentScenario()) << "Current scenario is null";
    EXPECT_FALSE(project->wasRenamed());
    EXPECT_TRUE(project->isSaved());

    // Check categories
    for (int i = 0; i < project->categories().length(); i++)
    {
        auto category = project->categories().at(i);
        EXPECT_TRUE(category) << "Category is null";
        EXPECT_EQ(category->name(), QString("Category %1").arg(i));
    }

    // Check elements
    EXPECT_FALSE(project->elements().isEmpty()) << "Project contains no elements";
    for (auto element : project->elements())
    {
        EXPECT_TRUE(element) << "Audio element is null";
    }
}

/**
 * @brief Modify the project and test if everything works correctly
 */
TEST_F(AudioProjectTest, TestModifications)
{
    // Name
    project->name("Modified Name");
    project->wasRenamed(true);
    EXPECT_EQ(project->name(), QStringLiteral("Modified Name"));
    EXPECT_EQ(project->originalName(), QStringLiteral("Project"));
    EXPECT_TRUE(project->wasRenamed());
    EXPECT_FALSE(project->isSaved());

    project->name("Modified Again");
    EXPECT_EQ(project->name(), QStringLiteral("Modified Again"));
    EXPECT_EQ(project->originalName(), QStringLiteral("Project"));

    // Categories
    auto categoryCount = project->categories().length();
    EXPECT_TRUE(project->addCategory(new AudioCategory("Added in test", project->name(), {}, project.get())));
    EXPECT_TRUE(project->addCategory(new AudioCategory("Added in test too", project->name(), {}, project.get())));
    EXPECT_FALSE(project->addCategory(nullptr));
    EXPECT_EQ(categoryCount + 2, project->categories().length());

    // Scenarios
    auto elementCount = project->elements().count();
    auto *category = new AudioCategory("Added in test for scenarios", project->name(), {}, project.get());
    EXPECT_TRUE(project->addCategory(category));

    auto *scenario = new AudioScenario("Added in test", category->path(), {}, {}, category);
    EXPECT_TRUE(category->addScenario(scenario));

    auto *subscenario = new AudioScenario("Sub added in test", scenario->path(), {}, {}, scenario);
    auto *subscenario2 = new AudioScenario("Sub added in test 2", scenario->path(), {}, {}, scenario);
    EXPECT_TRUE(scenario->addScenario(subscenario));
    EXPECT_TRUE(scenario->addScenario(subscenario2));

    EXPECT_EQ(category->scenarios().length(), 1);
    EXPECT_EQ(scenario->scenarios().length(), 2);
    EXPECT_EQ(scenario->model().length(), 2); // Only include main scenario if it contains elements

    // Elements
    auto *music1 = new AudioElement("Music1", AudioElement::Type::Music, scenario->path(), scenario);
    auto *music2 = new AudioElement("Music2", AudioElement::Type::Music, scenario->path(), subscenario);
    auto *music3 = new AudioElement("Music3", AudioElement::Type::Music, scenario->path(), subscenario2);
    EXPECT_TRUE(scenario->addElement(music1));
    EXPECT_TRUE(subscenario->addElement(music2));
    EXPECT_TRUE(subscenario2->addElement(music3));

    auto *sound1 = new AudioElement("Sound1", AudioElement::Type::Sound, scenario->path(), scenario);
    auto *radio1 = new AudioElement("Radio1", AudioElement::Type::Radio, scenario->path(), scenario);
    EXPECT_TRUE(scenario->addElement(sound1));
    EXPECT_TRUE(scenario->addElement(radio1));

    EXPECT_EQ(scenario->model().length(), 3); // Main scenario now has elements
    EXPECT_EQ(scenario->elements(false).length(), 3);
    EXPECT_EQ(scenario->elements(true).length(), 5);
    EXPECT_EQ(elementCount + 5, project->elements().length());
}

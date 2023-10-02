#include "src/filesystem/file.h"
#include "src/tools/audio/audiosaveload.h"
#include "src/tools/audio/project/audioproject.h"
#include "tests/testhelper/abstracttest.h"
#include "utils/fileutils.h"
#include <QJsonDocument>
#include <QUuid>

using namespace Qt::Literals::StringLiterals;

class AudioSaveLoadTest : public AbstractTest
{
protected:
    static void validateResult(QFuture<bool> &future, bool expected)
    {
        testFutureNoAuth(future, "validateResult", [future, expected]() {
            EXPECT_TRUE(future.isFinished());
            EXPECT_EQ(future.result(), expected);
        });
    }

    void initProjects();
    void saveProject();
    void findProjects();
    void deleteProjects();

    [[nodiscard]] QByteArray projectAsJson() const;

    std::unique_ptr<AudioProject> m_project = nullptr;
    std::unique_ptr<AudioProject> m_project1 = nullptr;
    std::unique_ptr<AudioProject> m_project2 = nullptr;

    QObject m_context;
};

auto AudioSaveLoadTest::projectAsJson() const -> QByteArray
{
    return QJsonDocument(m_project->toJson()).toJson(QJsonDocument::Indented);
}

void AudioSaveLoadTest::initProjects()
{
    // Read project file
    QFile projectFile(u":/resources/audioproject/project.audio"_s);
    EXPECT_TRUE(projectFile.open(QIODevice::ReadOnly)) << "Could not open test project file";
    m_project = std::make_unique<AudioProject>(QJsonDocument::fromJson(projectFile.readAll()).object(), nullptr);
    projectFile.close();

    EXPECT_TRUE(m_project->isSaved());

    // Verify that no saved projects exist
    auto future = AudioSaveLoad::findProjectsAsync(&m_context, getFilePath());
    testFuture(future, "AudioSaveLoad::findProjectsAsync", [future]() {
        auto list = future.result();
        EXPECT_TRUE(list.empty()) << "Project list is not empty, when it should be!";
    });

    // Create two empty projects
    m_project1 = std::make_unique<AudioProject>(u"Project_1"_s, 4, QList<AudioCategory *>(), nullptr);
    m_project1->isSaved(false);

    m_project2 = std::make_unique<AudioProject>(u"Project_2"_s, 4, QList<AudioCategory *>(), nullptr);
    m_project2->isSaved(false);
}

void AudioSaveLoadTest::saveProject()
{
    m_project->isSaved(false);
    EXPECT_FALSE(m_project->originalName().isEmpty());

    // Save first time
    auto future = AudioSaveLoad::saveProject(m_project.get(), getFilePath());
    validateResult(future, true);
    verifyFileContent(getFilePath(u"Project.audio"_s), projectAsJson());

    // Try to save again without making changes
    future = AudioSaveLoad::saveProject(m_project.get(), getFilePath());
    validateResult(future, true);
    verifyFileContent(getFilePath(u"Project.audio"_s), projectAsJson());

    // Rename project
    const auto newName = FileUtils::incrementName(m_project->name());
    m_project->wasRenamed(true);
    m_project->name(newName);

    future = AudioSaveLoad::saveProject(m_project.get(), getFilePath());
    validateResult(future, true);
    verifyFileContent(getFilePath(newName + ".audio"), projectAsJson());
    verifyThatFileExists(getFilePath(u"Project.audio"_s), false);

    // Try to save nullptr project
    expectWarning();
    future = AudioSaveLoad::saveProject(nullptr, getFilePath());
    validateResult(future, false);
}

void AudioSaveLoadTest::findProjects()
{
    auto future = AudioSaveLoad::findProjectsAsync(&m_context, getFilePath());
    testFuture(future, "AudioSaveLoad::findProjectsAsync", [future]() {
        auto list = future.result();
        EXPECT_FALSE(list.empty()) << "Project list is empty, when it should not be!";
        EXPECT_EQ(list.size(), 1);

        auto *project = list[0];
        EXPECT_EQ(project->name(), "Project_0");
    });

    auto future1 = AudioSaveLoad::saveProject(m_project1.get(), getFilePath());
    validateResult(future1, true);
    auto future2 = AudioSaveLoad::saveProject(m_project2.get(), getFilePath());
    validateResult(future2, true);

    future = AudioSaveLoad::findProjectsAsync(&m_context, getFilePath());
    testFuture(future, "AudioSaveLoad::findProjectsAsync", [future]() {
        auto list = future.result();
        EXPECT_FALSE(list.empty()) << "Project list is empty, when it should not be!";
        EXPECT_EQ(list.size(), 3);

        auto *project = list[0];
        EXPECT_EQ(project->name(), "Project_0");
        auto *project1 = list[1];
        EXPECT_EQ(project1->name(), "Project_1");
        auto *project2 = list[2];
        EXPECT_EQ(project2->name(), "Project_2");
    });
}

void AudioSaveLoadTest::deleteProjects()
{
    auto future = AudioSaveLoad::deleteProject(m_project1.get(), getFilePath());
    validateResult(future, true);

    QObject context;
    auto future2 = AudioSaveLoad::findProjectsAsync(&context, getFilePath());
    testFuture(future2, "AudioSaveLoad::findProjectsAsync", [future2]() {
        auto list = future2.result();
        EXPECT_FALSE(list.empty()) << "Project list is empty, when it should not be!";
        EXPECT_EQ(list.size(), 2);

        auto *project = list[0];
        EXPECT_EQ(project->name(), "Project_0");
        auto *project2 = list[1];
        EXPECT_EQ(project2->name(), "Project_2");
    });

    future = AudioSaveLoad::deleteProject(m_project.get(), getFilePath());
    validateResult(future, true);

    future = AudioSaveLoad::deleteProject(m_project2.get(), getFilePath());
    validateResult(future, true);

    future = AudioSaveLoad::deleteProject(nullptr, getFilePath());
    validateResult(future, false);

    future2 = AudioSaveLoad::findProjectsAsync(&context, getFilePath());
    testFuture(future2, "AudioSaveLoad::findProjectsAsync", [future2]() {
        auto list = future2.result();
        EXPECT_TRUE(list.empty()) << "Project list is not empty, when it should be!";
    });
}

TEST_F(AudioSaveLoadTest, TestNormalBehavior)
{
    initProjects();
    saveProject();
    findProjects();
    deleteProjects();
}

TEST_F(AudioSaveLoadTest, FindMissingFiles)
{
    QList<AudioFile *> files;
    files.reserve(5);

    for (int i = 0; i < 5; i++)
    {
        files.append(new AudioFile(QUuid::createUuid().toString() + ".mp3", AudioFile::Source::File,
                                   QStringLiteral("Test File %1").arg(i), nullptr));
    }

    files.append(new AudioFile(QUuid::createUuid().toString() + ".mp3", AudioFile::Source::Web,
                               QStringLiteral("Test File (URL)"), nullptr));
    files.append(new AudioFile(QUuid::createUuid().toString(), AudioFile::Source::Spotify,
                               QStringLiteral("Test File (Spotify))"), nullptr));
    files.append(new AudioFile(QUuid::createUuid().toString() + ".mp3", AudioFile::Source::Youtube,
                               QStringLiteral("Test File (Youtube)"), nullptr));

    auto future = AudioSaveLoad::findMissingFilesAsync(&m_context, files, getFilePath());
    validateResult(future, true);

    for (auto *file : files)
    {
        EXPECT_EQ(file->missing(), file->source() == AudioFile::Source::File);
    }

    auto fileName = files[0]->url();
    auto future1 = Files::File::saveAsync(getFilePath(fileName), "test-data", fileAccess);
    testFuture(future1, "File::saveAsync", []() {});
    verifyThatFileExists(getFilePath(fileName), true);

    future = AudioSaveLoad::findMissingFilesAsync(&m_context, files, getFilePath());
    validateResult(future, true);

    for (auto *file : files)
    {
        EXPECT_EQ(file->missing(), file->source() == AudioFile::Source::File && file->url() != fileName);
        file->deleteLater();
    }
}

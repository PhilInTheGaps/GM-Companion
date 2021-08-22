#include <QtTest>
#include <QObject>
#include <QUuid>
#include <QJsonDocument>
#include "src/tools/audio/project/audioproject.h"
#include "src/tools/audio/audiosaveload.h"
#include "src/filesystem_new/file.h"
#include "src/filesystem_new/fileaccesslocal.h"
#include "utils/fileutils.h"

#include "tests/testhelper/abstracttest.h"

class TestAudioSaveLoad : public AbstractTest
{
    Q_OBJECT

private:
    AudioProject *m_project = nullptr;
    AudioProject *m_project1 = nullptr;
    AudioProject *m_project2 = nullptr;

    QByteArray projectAsJson() const;
    static void validateResult(QFuture<bool> &future, bool expected);

private slots:
    void initTestCase();
    void saveProject();
    void findProjects();
    void findMissingFiles();
    void deleteProject();
};

auto TestAudioSaveLoad::projectAsJson() const -> QByteArray
{
    return QJsonDocument(m_project->toJson()).toJson(QJsonDocument::Indented);
}

void TestAudioSaveLoad::validateResult(QFuture<bool> &future, bool expected)
{
    QTRY_VERIFY2(future.isFinished() && future.result() == expected,
                 QString("IsFinished: %1, Result: %2")
                 .arg(future.isFinished())
                 .arg(future.result())
                 .toUtf8());
}

void TestAudioSaveLoad::initTestCase()
{
    Files::FileAccess::setInstance(new Files::FileAccessLocal(this));

    // Read project file
    QFile projectFile(":/resources/audioproject/project.audio");
    QVERIFY2(projectFile.open(QIODevice::ReadOnly), "Could not open test project file");
    m_project = new AudioProject(QJsonDocument::fromJson(projectFile.readAll()).object(), this);
    projectFile.close();

    // Verify that no saved projects exist
    auto future = AudioSaveLoad::findProjectsAsync(getFilePath());
    testFuture(future, "AudioSaveLoad::findProjectsAsync", [future]() {
        auto list = future.result();
        QVERIFY2(list.isEmpty(), "Project list is not empty, when it should be!");
    });

    // Create two empty projects
    m_project1 = new AudioProject("Project_1", 4, {}, this);
    m_project1->isSaved(false);

    m_project2 = new AudioProject("Project_2", 4, {}, this);
    m_project2->isSaved(false);
}

void TestAudioSaveLoad::saveProject()
{
    m_project->isSaved(false);

    // Save first time
    auto future = AudioSaveLoad::saveProject(m_project, getFilePath());
    validateResult(future, true);
    verifyFileContent(getFilePath("Project.audio"), projectAsJson());

    // Try to save again without making changes
    future = AudioSaveLoad::saveProject(m_project, getFilePath());
    validateResult(future, true);
    verifyFileContent(getFilePath("Project.audio"), projectAsJson());

    // Rename project
    const auto newName = FileUtils::incrementName(m_project->name());
    m_project->oldName(m_project->name());
    m_project->wasRenamed(true);
    m_project->setName(newName);

    future = AudioSaveLoad::saveProject(m_project, getFilePath());
    validateResult(future, true);
    verifyFileContent(getFilePath(newName + ".audio"), projectAsJson());
    verifyThatFileExists(getFilePath("Project.audio"), false);

    // Try to save nullptr project
    expectWarning();
    future = AudioSaveLoad::saveProject(nullptr, getFilePath());
    validateResult(future, false);
}

void TestAudioSaveLoad::findProjects()
{
    auto future = AudioSaveLoad::findProjectsAsync(getFilePath());
    testFuture(future, "AudioSaveLoad::findProjectsAsync", [future]() {
        auto list = future.result();
        QVERIFY2(!list.isEmpty(), "Project list is empty, when it should not be!");
        QCOMPARE(list.length(), 1);

        auto *project = list[0];
        QCOMPARE(project->name(), "Project_0");
    });

    auto future1 = AudioSaveLoad::saveProject(m_project1, getFilePath());
    validateResult(future1, true);
    auto future2 = AudioSaveLoad::saveProject(m_project2, getFilePath());
    validateResult(future2, true);

    future = AudioSaveLoad::findProjectsAsync(getFilePath());
    testFuture(future, "AudioSaveLoad::findProjectsAsync", [future]() {
        auto list = future.result();
        QVERIFY2(!list.isEmpty(), "Project list is empty, when it should not be!");
        QCOMPARE(list.length(), 3);

        auto *project = list[0];
        QCOMPARE(project->name(), "Project_0");
        auto *project1 = list[1];
        QCOMPARE(project1->name(), "Project_1");
        auto *project2 = list[2];
        QCOMPARE(project2->name(), "Project_2");
    });
}

void TestAudioSaveLoad::findMissingFiles()
{    
    QList<AudioFile*> files;

    for (int i = 0; i < 5; i++)
    {
        files.append(new AudioFile(
                         QUuid::createUuid().toString() + ".mp3", 0,
                         QString("Test File %1").arg(i), this));
    }

    files.append(new AudioFile(
                     QUuid::createUuid().toString() + ".mp3", 1,
                     QString("Test File (URL)"), this));
    files.append(new AudioFile(
                     QUuid::createUuid().toString(), 2,
                     QString("Test File (Spotify))"), this));
    files.append(new AudioFile(
                     QUuid::createUuid().toString() + ".mp3", 3,
                     QString("Test File (Youtube)"), this));

    auto future = AudioSaveLoad::findMissingFilesAsync(files, getFilePath());
    validateResult(future, true);

    for (auto *file : files)
    {
        QCOMPARE(file->missing(), file->source() == 0);
    }

    auto fileName = files[0]->url();
    auto future1 = Files::File::saveAsync(getFilePath(fileName), "test-data", fileAccess);
    testFuture(future1, "File::saveAsync", [](){});
    verifyThatFileExists(getFilePath(fileName), true);

    future = AudioSaveLoad::findMissingFilesAsync(files, getFilePath());
    validateResult(future, true);

    for (auto *file : files)
    {
        QCOMPARE(file->missing(), file->source() == 0 && file->url() != fileName);
    }
}

void TestAudioSaveLoad::deleteProject()
{
    auto future = AudioSaveLoad::deleteProject(m_project1, getFilePath());
    validateResult(future, true);

    auto future2 = AudioSaveLoad::findProjectsAsync(getFilePath());
    testFuture(future2, "AudioSaveLoad::findProjectsAsync", [future2]() {
        auto list = future2.result();
        QVERIFY2(!list.isEmpty(), "Project list is empty, when it should not be!");
        QCOMPARE(list.length(), 2);

        auto *project = list[0];
        QCOMPARE(project->name(), "Project_0");
        auto *project2 = list[1];
        QCOMPARE(project2->name(), "Project_2");
    });

    future = AudioSaveLoad::deleteProject(m_project, getFilePath());
    validateResult(future, true);

    future = AudioSaveLoad::deleteProject(m_project2, getFilePath());
    validateResult(future, true);

    future = AudioSaveLoad::deleteProject(nullptr, getFilePath());
    validateResult(future, false);

    future2 = AudioSaveLoad::findProjectsAsync(getFilePath());
    testFuture(future2, "AudioSaveLoad::findProjectsAsync", [future2]() {
        auto list = future2.result();
        QVERIFY2(list.isEmpty(), "Project list is not empty, when it should be!");
    });
}

QTEST_GUILESS_MAIN(TestAudioSaveLoad)
#include "testaudiosaveload.moc"

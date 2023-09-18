#include "src/common/utils/fileutils.h"
#include "src/tools/converter/convertereditor.h"
#include "tests/testhelper/abstracttest.h"
#include <QDir>
#include <gtest/gtest.h>

class ConverterEditorTest : public AbstractTest
{
};

TEST_F(ConverterEditorTest, CanCreateNewProject)
{
    ConverterEditor editor(nullptr);
    EXPECT_TRUE(editor.projects().isEmpty());
    EXPECT_TRUE(editor.isSaved());

    EXPECT_FALSE(editor.createProject(QLatin1String()));

    EXPECT_TRUE(editor.createProject(QStringLiteral("Test Project")));
    EXPECT_EQ(editor.projects().length(), 1);
    EXPECT_FALSE(editor.isSaved());
}

TEST_F(ConverterEditorTest, CanRenameProject)
{
    ConverterEditor editor(nullptr);
    EXPECT_TRUE(editor.createProject(QStringLiteral("Test")));
    editor.isSaved(true);

    const auto projects = editor.projects();
    EXPECT_TRUE(editor.renameProject(projects.first(), QStringLiteral("Renamed")));
    EXPECT_EQ(projects.first()->name(), QStringLiteral("Renamed"));
    EXPECT_FALSE(editor.isSaved());

    EXPECT_FALSE(editor.renameProject(nullptr, QStringLiteral("Renamed")));
}

TEST_F(ConverterEditorTest, CanDeleteProject)
{
    ConverterEditor editor(nullptr);
    EXPECT_TRUE(editor.createProject(QStringLiteral("Test")));
    auto projects = editor.projects();

    EXPECT_FALSE(editor.deleteProject(nullptr));
    EXPECT_EQ(editor.projects().length(), 1);

    EXPECT_TRUE(editor.deleteProject(projects.first()));
    EXPECT_TRUE(editor.projects().isEmpty());
    EXPECT_FALSE(editor.currentProject());

    EXPECT_TRUE(editor.createProject(QStringLiteral("Test1")));
    EXPECT_TRUE(editor.createProject(QStringLiteral("Test2")));
    EXPECT_EQ(editor.projects().length(), 2);
    projects = editor.projects();

    EXPECT_TRUE(editor.deleteProject(projects.takeFirst()));
    EXPECT_EQ(editor.projects().length(), 1);
    EXPECT_TRUE(editor.deleteProject(projects.takeFirst()));
    EXPECT_TRUE(editor.projects().isEmpty());
}

TEST_F(ConverterEditorTest, CanCreateCategory)
{
    ConverterEditor editor(nullptr);

    // needs project first
    EXPECT_FALSE(editor.createCategory(QStringLiteral("Category")));

    EXPECT_TRUE(editor.createProject(QStringLiteral("Test")));
    EXPECT_TRUE(editor.currentProject()->categories().isEmpty());

    // neds a name
    EXPECT_FALSE(editor.createCategory(QLatin1String()));
    EXPECT_TRUE(editor.currentProject()->categories().isEmpty());

    EXPECT_TRUE(editor.createCategory(QStringLiteral("Category")));
    EXPECT_EQ(editor.currentCategory()->name(), QStringLiteral("Category"));
    EXPECT_EQ(editor.currentProject()->categories().length(), 1);
    EXPECT_FALSE(editor.isSaved());
}

TEST_F(ConverterEditorTest, CanRenameCategory)
{
    ConverterEditor editor(nullptr);
    EXPECT_TRUE(editor.createProject(QStringLiteral("Test")));
    EXPECT_TRUE(editor.createCategory(QStringLiteral("Category")));

    EXPECT_FALSE(editor.renameCategory(nullptr, QStringLiteral("Renamed")));

    EXPECT_TRUE(editor.renameCategory(editor.currentCategory(), QStringLiteral("Renamed")));
    EXPECT_EQ(editor.currentCategory()->name(), QStringLiteral("Renamed"));
    EXPECT_FALSE(editor.isSaved());
}

TEST_F(ConverterEditorTest, CanDeleteCategory)
{
    ConverterEditor editor(nullptr);
    EXPECT_TRUE(editor.createProject(QStringLiteral("Test")));

    EXPECT_FALSE(editor.deleteCategory(nullptr));

    EXPECT_TRUE(editor.createCategory(QStringLiteral("Cat1")));
    EXPECT_TRUE(editor.deleteCategory(editor.currentCategory()));
    EXPECT_TRUE(editor.currentProject()->categories().isEmpty());
    EXPECT_FALSE(editor.currentCategory());
    EXPECT_FALSE(editor.isSaved());

    EXPECT_TRUE(editor.createCategory(QStringLiteral("Cat1")));
    EXPECT_TRUE(editor.createCategory(QStringLiteral("Cat2")));
    auto categories = editor.currentProject()->categories();

    EXPECT_TRUE(editor.deleteCategory(categories.takeFirst()));
    EXPECT_EQ(editor.currentProject()->categories().length(), 1);
    EXPECT_TRUE(editor.currentCategory());

    EXPECT_TRUE(editor.deleteCategory(editor.currentCategory()));
    EXPECT_FALSE(editor.currentCategory());
}

TEST_F(ConverterEditorTest, CanCreateUnit)
{
    ConverterEditor editor(nullptr);

    EXPECT_FALSE(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));
    EXPECT_TRUE(editor.createProject(QStringLiteral("Project")));

    EXPECT_FALSE(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));
    EXPECT_TRUE(editor.createCategory(QStringLiteral("Category")));

    EXPECT_FALSE(editor.createUnit(QLatin1String(), QStringLiteral("1")));
    EXPECT_FALSE(editor.createUnit(QStringLiteral("Unit"), QLatin1String()));
    EXPECT_FALSE(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("NOT A NUMBER")));

    EXPECT_TRUE(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));
    EXPECT_EQ(editor.currentCategory()->units().length(), 1);

    EXPECT_TRUE(editor.createUnit(QStringLiteral("Unit2"), QStringLiteral("2")));
    EXPECT_EQ(editor.currentCategory()->units().length(), 2);
    EXPECT_FALSE(editor.isSaved());
}

TEST_F(ConverterEditorTest, CanRenameUnit)
{
    ConverterEditor editor(nullptr);
    EXPECT_TRUE(editor.createProject(QStringLiteral("Project")));
    EXPECT_TRUE(editor.createCategory(QStringLiteral("Category")));
    EXPECT_TRUE(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));

    EXPECT_FALSE(editor.renameUnit(nullptr, QStringLiteral("Renamed")));

    auto units = editor.currentCategory()->units();
    EXPECT_TRUE(editor.renameUnit(units.first(), QStringLiteral("Renamed")));
    EXPECT_EQ(units.first()->name(), QStringLiteral("Renamed"));
    EXPECT_FALSE(editor.isSaved());
}

TEST_F(ConverterEditorTest, CanChangeUnitValue)
{
    ConverterEditor editor(nullptr);
    EXPECT_TRUE(editor.createProject(QStringLiteral("Project")));
    EXPECT_TRUE(editor.createCategory(QStringLiteral("Category")));
    EXPECT_TRUE(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));

    EXPECT_FALSE(editor.changeUnitValue(nullptr, QStringLiteral("2")));

    auto units = editor.currentCategory()->units();
    EXPECT_TRUE(editor.changeUnitValue(units.first(), QStringLiteral("2")));
    EXPECT_EQ(units.first()->value(), 2);
    EXPECT_FALSE(editor.isSaved());
}

TEST_F(ConverterEditorTest, CanDeleteUnit)
{
    ConverterEditor editor(nullptr);
    EXPECT_TRUE(editor.createProject(QStringLiteral("Project")));
    EXPECT_TRUE(editor.createCategory(QStringLiteral("Category")));

    EXPECT_FALSE(editor.deleteUnit(nullptr));

    EXPECT_TRUE(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));
    auto units = editor.currentCategory()->units();
    EXPECT_EQ(units.length(), 1);
    EXPECT_TRUE(editor.deleteUnit(units.first()));
    EXPECT_TRUE(editor.currentCategory()->units().isEmpty());

    EXPECT_TRUE(editor.createUnit(QStringLiteral("Unit1"), QStringLiteral("1")));
    EXPECT_TRUE(editor.createUnit(QStringLiteral("Unit2"), QStringLiteral("2")));
    units = editor.currentCategory()->units();
    EXPECT_EQ(units.length(), 2);

    EXPECT_TRUE(editor.deleteUnit(units.first()));
    units = editor.currentCategory()->units();
    EXPECT_EQ(units.length(), 1);
    EXPECT_TRUE(editor.deleteUnit(units.first()));
    EXPECT_TRUE(editor.currentCategory()->units().isEmpty());
    EXPECT_FALSE(editor.isSaved());
}

TEST_F(ConverterEditorTest, CanSaveProjects)
{
    const auto unitDir = FileUtils::fileInDir(FileUtils::dirFromFolders({".gm-companion", "units"}), QDir::homePath());
    const auto backupDir = backupUserFolder(unitDir);

    QDir d(unitDir);
    if (!d.exists()) d.mkpath(unitDir);

    EXPECT_TRUE(d.exists());
    EXPECT_TRUE(d.entryList(QDir::Files).isEmpty());

    // create old v1 project that should get backed up
    QFile f(FileUtils::fileInDir(QStringLiteral("old.ini"), unitDir));
    EXPECT_FALSE(f.exists());
    EXPECT_TRUE(f.open(QIODevice::WriteOnly));
    f.write("version=0");
    f.close();

    // create v2 project that should get removed
    QFile f1(FileUtils::fileInDir(QStringLiteral("old.json"), unitDir));
    EXPECT_FALSE(f1.exists());
    EXPECT_TRUE(f1.open(QIODevice::WriteOnly));
    f1.write(R"({"name": "Test", "version": 2, "categories": []})");
    f1.close();

    ConverterEditor editor(nullptr);

    // first test project
    editor.createProject(QStringLiteral("Test"));
    editor.createCategory(QStringLiteral("C0"));
    editor.createUnit(QStringLiteral("U0"), QStringLiteral("1.0"));
    editor.createUnit(QStringLiteral("U1"), QStringLiteral("2.0"));
    editor.createCategory(QStringLiteral("C1"));
    editor.createUnit(QStringLiteral("U2"), QStringLiteral("3.0"));
    editor.createUnit(QStringLiteral("U3"), QStringLiteral("4.0"));

    // second test project with same name (!)
    editor.createProject(QStringLiteral("Test"));
    editor.createCategory(QStringLiteral("C2"));
    editor.createUnit(QStringLiteral("U4"), QStringLiteral("5.0"));
    editor.createUnit(QStringLiteral("U5"), QStringLiteral("6.0"));
    editor.createCategory(QStringLiteral("C3"));
    editor.createUnit(QStringLiteral("U6"), QStringLiteral("7.0"));
    editor.createUnit(QStringLiteral("U7"), QStringLiteral("8.0"));

    EXPECT_FALSE(editor.isSaved());
    editor.save();
    EXPECT_TRUE(editor.isSaved());

    EXPECT_EQ(d.entryList(QDir::Files).length(), 2);
    EXPECT_TRUE(d.exists(QStringLiteral("v1")));
    d.cd(QStringLiteral("v1"));
    EXPECT_EQ(d.entryList(QDir::Files).length(), 1);

    restoreUserFolder(backupDir, unitDir);
}

TEST_F(ConverterEditorTest, CanSaveAndCreateDestinationDir)
{
    const auto unitDir = FileUtils::fileInDir(FileUtils::dirFromFolders({".gm-companion", "units"}), QDir::homePath());
    const auto backupDir = backupUserFolder(unitDir);

    QDir d(unitDir);
    if (d.exists()) d.removeRecursively();

    ConverterEditor editor(nullptr);
    editor.createProject(QStringLiteral("Test"));
    editor.save();
    EXPECT_TRUE(editor.isSaved());
    EXPECT_EQ(d.entryList(QDir::Files).length(), 1);

    restoreUserFolder(backupDir, unitDir);
}

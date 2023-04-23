#include "src/common/utils/fileutils.h"
#include "src/tools/converter/convertereditor.h"
#include "tests/testhelper/abstracttest.h"
#include <QDir>
#include <QtTest>

class TestConverterEditor : public AbstractTest
{
    Q_OBJECT

private slots:
    void canCreateNewProject();
    void canRenameProject();
    void canDeleteProject();

    void canCreateCategory();
    void canRenameCategory();
    void canDeleteCategory();

    void canCreateUnit();
    void canRenameUnit();
    void canChangeUnitValue();
    void canDeleteUnit();

    void canSaveProjects();
};

void TestConverterEditor::canCreateNewProject()
{
    ConverterEditor editor(nullptr, nullptr);
    QVERIFY(editor.projects().isEmpty());
    QVERIFY(editor.isSaved());

    QVERIFY(!editor.createProject(QLatin1String()));

    QVERIFY(editor.createProject(QStringLiteral("Test Project")));
    QCOMPARE(editor.projects().length(), 1);
    QVERIFY(!editor.isSaved());
}

void TestConverterEditor::canRenameProject()
{
    ConverterEditor editor(nullptr, nullptr);
    QVERIFY(editor.createProject(QStringLiteral("Test")));
    editor.isSaved(true);

    const auto projects = editor.projects();
    QVERIFY(editor.renameProject(projects.first(), QStringLiteral("Renamed")));
    QCOMPARE(projects.first()->name(), QStringLiteral("Renamed"));
    QVERIFY(!editor.isSaved());

    QVERIFY(!editor.renameProject(nullptr, QStringLiteral("Renamed")));
}

void TestConverterEditor::canDeleteProject()
{
    ConverterEditor editor(nullptr, nullptr);
    QVERIFY(editor.createProject(QStringLiteral("Test")));
    auto projects = editor.projects();

    QVERIFY(!editor.deleteProject(nullptr));
    QCOMPARE(editor.projects().length(), 1);

    QVERIFY(editor.deleteProject(projects.first()));
    QVERIFY(editor.projects().isEmpty());
    QVERIFY(!editor.currentProject());

    QVERIFY(editor.createProject(QStringLiteral("Test1")));
    QVERIFY(editor.createProject(QStringLiteral("Test2")));
    QCOMPARE(editor.projects().length(), 2);
    projects = editor.projects();

    QVERIFY(editor.deleteProject(projects.takeFirst()));
    QCOMPARE(editor.projects().length(), 1);
    QVERIFY(editor.deleteProject(projects.takeFirst()));
    QVERIFY(editor.projects().isEmpty());
}

void TestConverterEditor::canCreateCategory()
{
    ConverterEditor editor(nullptr, nullptr);

    // needs project first
    QVERIFY(!editor.createCategory(QStringLiteral("Category")));

    QVERIFY(editor.createProject(QStringLiteral("Test")));
    QVERIFY(editor.currentProject()->categories().isEmpty());

    // neds a name
    QVERIFY(!editor.createCategory(QLatin1String()));
    QVERIFY(editor.currentProject()->categories().isEmpty());

    QVERIFY(editor.createCategory(QStringLiteral("Category")));
    QCOMPARE(editor.currentCategory()->name(), QStringLiteral("Category"));
    QCOMPARE(editor.currentProject()->categories().length(), 1);
    QVERIFY(!editor.isSaved());
}

void TestConverterEditor::canRenameCategory()
{
    ConverterEditor editor(nullptr, nullptr);
    QVERIFY(editor.createProject(QStringLiteral("Test")));
    QVERIFY(editor.createCategory(QStringLiteral("Category")));

    QVERIFY(!editor.renameCategory(nullptr, QStringLiteral("Renamed")));

    QVERIFY(editor.renameCategory(editor.currentCategory(), QStringLiteral("Renamed")));
    QCOMPARE(editor.currentCategory()->name(), QStringLiteral("Renamed"));
    QVERIFY(!editor.isSaved());
}

void TestConverterEditor::canDeleteCategory()
{
    ConverterEditor editor(nullptr, nullptr);
    QVERIFY(editor.createProject(QStringLiteral("Test")));

    QVERIFY(!editor.deleteCategory(nullptr));

    QVERIFY(editor.createCategory(QStringLiteral("Cat1")));
    QVERIFY(editor.deleteCategory(editor.currentCategory()));
    QVERIFY(editor.currentProject()->categories().isEmpty());
    QVERIFY(!editor.currentCategory());
    QVERIFY(!editor.isSaved());

    QVERIFY(editor.createCategory(QStringLiteral("Cat1")));
    QVERIFY(editor.createCategory(QStringLiteral("Cat2")));
    auto categories = editor.currentProject()->categories();

    QVERIFY(editor.deleteCategory(categories.takeFirst()));
    QCOMPARE(editor.currentProject()->categories().length(), 1);
    QVERIFY(editor.currentCategory());

    QVERIFY(editor.deleteCategory(editor.currentCategory()));
    QVERIFY(!editor.currentCategory());
}

void TestConverterEditor::canCreateUnit()
{
    ConverterEditor editor(nullptr, nullptr);

    QVERIFY(!editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));
    QVERIFY(editor.createProject(QStringLiteral("Project")));

    QVERIFY(!editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));
    QVERIFY(editor.createCategory(QStringLiteral("Category")));

    QVERIFY(!editor.createUnit(QLatin1String(), QStringLiteral("1")));
    QVERIFY(!editor.createUnit(QStringLiteral("Unit"), QLatin1String()));
    QVERIFY(!editor.createUnit(QStringLiteral("Unit"), QStringLiteral("NOT A NUMBER")));

    QVERIFY(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));
    QCOMPARE(editor.currentCategory()->units().length(), 1);

    QVERIFY(editor.createUnit(QStringLiteral("Unit2"), QStringLiteral("2")));
    QCOMPARE(editor.currentCategory()->units().length(), 2);
    QVERIFY(!editor.isSaved());
}

void TestConverterEditor::canRenameUnit()
{
    ConverterEditor editor(nullptr, nullptr);
    QVERIFY(editor.createProject(QStringLiteral("Project")));
    QVERIFY(editor.createCategory(QStringLiteral("Category")));
    QVERIFY(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));

    QVERIFY(!editor.renameUnit(nullptr, QStringLiteral("Renamed")));

    auto units = editor.currentCategory()->units();
    QVERIFY(editor.renameUnit(units.first(), QStringLiteral("Renamed")));
    QCOMPARE(units.first()->name(), QStringLiteral("Renamed"));
    QVERIFY(!editor.isSaved());
}

void TestConverterEditor::canChangeUnitValue()
{
    ConverterEditor editor(nullptr, nullptr);
    QVERIFY(editor.createProject(QStringLiteral("Project")));
    QVERIFY(editor.createCategory(QStringLiteral("Category")));
    QVERIFY(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));

    QVERIFY(!editor.changeUnitValue(nullptr, QStringLiteral("2")));

    auto units = editor.currentCategory()->units();
    QVERIFY(editor.changeUnitValue(units.first(), QStringLiteral("2")));
    QCOMPARE(units.first()->value(), 2);
    QVERIFY(!editor.isSaved());
}

void TestConverterEditor::canDeleteUnit()
{
    ConverterEditor editor(nullptr, nullptr);
    QVERIFY(editor.createProject(QStringLiteral("Project")));
    QVERIFY(editor.createCategory(QStringLiteral("Category")));

    QVERIFY(!editor.deleteUnit(nullptr));

    QVERIFY(editor.createUnit(QStringLiteral("Unit"), QStringLiteral("1")));
    auto units = editor.currentCategory()->units();
    QCOMPARE(units.length(), 1);
    QVERIFY(editor.deleteUnit(units.first()));
    QVERIFY(editor.currentCategory()->units().isEmpty());

    QVERIFY(editor.createUnit(QStringLiteral("Unit1"), QStringLiteral("1")));
    QVERIFY(editor.createUnit(QStringLiteral("Unit2"), QStringLiteral("2")));
    units = editor.currentCategory()->units();
    QCOMPARE(units.length(), 2);

    QVERIFY(editor.deleteUnit(units.first()));
    units = editor.currentCategory()->units();
    QCOMPARE(units.length(), 1);
    QVERIFY(editor.deleteUnit(units.first()));
    QVERIFY(editor.currentCategory()->units().isEmpty());
    QVERIFY(!editor.isSaved());
}

void TestConverterEditor::canSaveProjects()
{
    const auto unitDir = FileUtils::fileInDir(FileUtils::dirFromFolders({".gm-companion", "units"}), QDir::homePath());
    const auto backupDir = backupUserFolder(unitDir);

    QDir d(unitDir);
    if (!d.exists()) d.mkpath(unitDir);

    QVERIFY(d.exists());
    QVERIFY(d.entryList(QDir::Files).isEmpty());

    // create old v1 project that should get backed up
    QFile f(FileUtils::fileInDir(QStringLiteral("old.ini"), unitDir));
    QVERIFY(!f.exists());
    QVERIFY(f.open(QIODevice::WriteOnly));
    f.write("version=0");
    f.close();

    // create v2 project that should get removed
    QFile f1(FileUtils::fileInDir(QStringLiteral("old.json"), unitDir));
    QVERIFY(!f1.exists());
    QVERIFY(f1.open(QIODevice::WriteOnly));
    f1.write(R"({"name": "Test", "version": 2, "categories": []})");
    f1.close();

    ConverterEditor editor(nullptr, nullptr);

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

    QVERIFY(!editor.isSaved());
    editor.save();
    QVERIFY(editor.isSaved());

    QCOMPARE(d.entryList(QDir::Files).length(), 2);
    QVERIFY(d.exists(QStringLiteral("v1")));
    d.cd(QStringLiteral("v1"));
    QCOMPARE(d.entryList(QDir::Files).length(), 1);

    restoreUserFolder(backupDir, unitDir);
}

QTEST_APPLESS_MAIN(TestConverterEditor)
#include "testconvertereditor.moc"

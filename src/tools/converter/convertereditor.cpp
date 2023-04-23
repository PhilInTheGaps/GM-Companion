#include "convertereditor.h"
#include "convertertool.h"
#include "src/common/utils/fileutils.h"
#include <QDir>
#include <QLoggingCategory>
#include <QQmlContext>
#include <gsl/gsl>

Q_LOGGING_CATEGORY(gmConverterEditor, "gm.converter.editor")

ConverterEditor::ConverterEditor(const QQmlApplicationEngine *engine, QObject *parent) : AbstractTool{parent}
{
    if (engine)
    {
        engine->rootContext()->setContextProperty(QStringLiteral("converter_editor"), this);
    }

    connect(this, &ConverterEditor::projectsChanged, this, &ConverterEditor::onProjectsChanged);
    connect(this, &ConverterEditor::currentProjectChanged, this, &ConverterEditor::onCurrentProjectChanged);
}

auto ConverterEditor::createProject(const QString &name) -> bool
{
    if (name.isEmpty()) return false;

    gsl::owner<ConverterProject *> project = new ConverterProject(name, this);

    a_projects << project;
    emit projectsChanged(a_projects);

    currentProject(project);

    madeChanges();
    return true;
}

auto ConverterEditor::renameProject(ConverterProject *project, const QString &name) -> bool
{
    if (!project) return false;

    project->name(name);

    madeChanges();
    return true;
}

auto ConverterEditor::deleteProject(ConverterProject *project) -> bool
{
    if (!project) return false;

    if (currentProject() == project)
    {
        currentProject(nullptr);
    }

    a_projects.removeOne(project);
    project->deleteLater();

    emit projectsChanged(a_projects);
    madeChanges();
    return true;
}

auto ConverterEditor::createCategory(const QString &name) -> bool
{
    if (!currentProject() || name.isEmpty()) return false;

    gsl::owner<ConverterCategory *> category = new ConverterCategory(name, currentProject());

    auto categories = currentProject()->categories();
    categories << category;
    currentProject()->categories(categories);
    currentCategory(category);

    madeChanges();
    return true;
}

auto ConverterEditor::renameCategory(ConverterCategory *category, const QString &name) -> bool
{
    if (!category) return false;

    category->name(name);

    madeChanges();
    return true;
}

auto ConverterEditor::deleteCategory(ConverterCategory *category) -> bool
{
    if (!category) return false;

    if (currentCategory() == category)
    {
        currentCategory(nullptr);
    }

    if (currentProject())
    {
        auto categories = currentProject()->categories();
        categories.removeOne(category);
        currentProject()->categories(categories);
    }

    if (!currentProject()->categories().isEmpty())
    {
        auto categories = currentProject()->categories();
        currentCategory(categories.first());
    }

    category->deleteLater();

    madeChanges();
    return true;
}

auto ConverterEditor::createUnit(const QString &name, const QString &value) -> bool
{
    if (!currentCategory() || name.isEmpty() || value.isEmpty()) return false;

    bool ok = false;
    auto number = ConverterTool::textToNumber(value, &ok);

    if (!ok) return false;

    gsl::owner<ConverterUnit *> unit = new ConverterUnit(name, number, currentCategory());

    auto units = currentCategory()->units();
    units << unit;
    currentCategory()->units(units);

    madeChanges();
    return true;
}

auto ConverterEditor::renameUnit(ConverterUnit *unit, const QString &name) -> bool
{
    if (!unit) return false;

    unit->name(name);

    madeChanges();
    return true;
}

auto ConverterEditor::changeUnitValue(ConverterUnit *unit, const QString &value) -> bool
{
    if (!unit) return false;

    unit->value(ConverterTool::textToNumber(value));

    madeChanges();
    return true;
}

auto ConverterEditor::deleteUnit(ConverterUnit *unit) -> bool
{
    if (!unit) return false;

    if (currentCategory())
    {
        auto units = currentCategory()->units();
        units.removeOne(unit);
        currentCategory()->units(units);
    }

    unit->deleteLater();

    madeChanges();
    return true;
}

void ConverterEditor::save()
{
    if (isSaved()) return;

    backupV1Projects();
    deleteOldProjectFiles();

    const auto projects = this->projects();

    for (const auto *project : projects)
    {
        saveProject(project);
    }

    // TODO: Apply changes to tool

    isSaved(true);
}

void ConverterEditor::loadData()
{
    if (isLoading() || isDataLoaded()) return;

    qCDebug(gmConverterEditor()) << "Loading custom project ...";

    isLoading(true);

    const auto paths = {getLocalProjectPath()};
    a_projects = ConverterTool::loadLocalProjects(paths, this);

    isLoading(false);
    setIsDataLoaded(true);

    emit projectsChanged(a_projects);
}

void ConverterEditor::onProjectsChanged(const QList<ConverterProject *> &projects)
{
    if (projects.isEmpty() || currentProject()) return;

    currentProject(projects.first());
}

void ConverterEditor::onCurrentProjectChanged(ConverterProject *project)
{
    if (!project) return;

    const auto categories = project->categories();
    if (categories.isEmpty()) return;

    currentCategory(categories.first());
}

auto ConverterEditor::getLocalProjectPath() -> QString
{
    return FileUtils::fileInDir(FileUtils::dirFromFolders({".gm-companion", "units"}), QDir::homePath());
}

void ConverterEditor::madeChanges()
{
    isSaved(false);
}

/// Move old v1 ini projects to a sub folder so that they are not loaded next time
void ConverterEditor::backupV1Projects()
{
    const auto backupPath = FileUtils::fileInDir(QStringLiteral("v1"), getLocalProjectPath());

    QDir const projectDir(getLocalProjectPath());
    if (!projectDir.exists()) return;

    const auto entries = projectDir.entryList({"*.ini"}, QDir::Files);

    if (entries.isEmpty()) return;

    QDir const backupDir(backupPath);
    if (!backupDir.exists()) backupDir.mkpath(backupPath);

    for (const auto &entry : entries)
    {
        QFile file(FileUtils::fileInDir(entry, projectDir.path()));

        auto newPath = FileUtils::fileInDir(entry, backupPath);

        while (QFileInfo::exists(newPath))
        {
            newPath = FileUtils::incrementFileName(newPath);
        }

        file.rename(newPath);
    }
}

void ConverterEditor::deleteOldProjectFiles()
{
    QDir const projectDir(getLocalProjectPath());
    if (!projectDir.exists()) return;

    const auto entries = projectDir.entryList(QDir::Files);
    for (const auto &entry : entries)
    {
        QFile file(FileUtils::fileInDir(entry, projectDir.path()));
        file.remove();
    }
}

auto ConverterEditor::saveProject(const ConverterProject *project) -> bool
{
    if (!project) return true;

    const auto name = QStringLiteral("%1.json").arg(project->name().isEmpty() ? QStringLiteral("unnamed project") : project->name());

    auto path = FileUtils::fileInDir(name, getLocalProjectPath());

    while (QFileInfo::exists(path))
    {
        path = FileUtils::incrementFileName(path);
    }

    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
        qCWarning(gmConverterEditor()) << "Could not open file to save project:" << path;
        return false;
    }

    const auto data = project->toJson().toJson();
    f.write(data);
    f.close();

    return true;
}

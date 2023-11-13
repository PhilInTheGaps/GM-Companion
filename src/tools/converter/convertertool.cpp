#include "convertertool.h"
#include "project/converterprojectupgrader.h"
#include "settings/settingsmanager.h"
#include "src/addons/addon_reader/addonreader.h"
#include "src/addons/addonmanager.h"
#include "src/common/utils/fileutils.h"
#include <QDir>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QSettings>

using namespace Qt::Literals::StringLiterals;
using namespace Common::Settings;

Q_LOGGING_CATEGORY(gmConverter, "gm.converter")

ConverterTool::ConverterTool(QObject *parent) : AbstractTool(parent), m_editor(new ConverterEditor(this))
{
    connect(this, &ConverterTool::projectsChanged, this, &ConverterTool::onProjectsChanged);
    connect(this, &ConverterTool::currentProjectChanged, this, &ConverterTool::onCurrentProjectChanged);
    connect(this, &ConverterTool::currentCategoryChanged, this, &ConverterTool::onCurrentCategoryChanged);
    connect(&m_editor, &ConverterEditor::isSavedChanged, this, &ConverterTool::onEditorSavedChanged);
    connect(AddonManager::instance(), &AddonManager::isLoadingChanged, this,
            &ConverterTool::onAddonManagerLoadingChanged);
}

auto ConverterTool::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> ConverterTool *
{
    Q_UNUSED(jsEngine)
    return new ConverterTool(qmlEngine);
}

void ConverterTool::loadData()
{
    if (isLoading() || isDataLoaded()) return;

    qCDebug(gmConverter()) << "Loading all projects ...";

    isLoading(true);

    a_projects = loadLocalProjects();

    loadAddonProjects();

    isLoading(false);
    setIsDataLoaded(true);

    emit projectsChanged();

    m_editor.loadData();
}

void ConverterTool::onProjectsChanged()
{
    if (a_projects.isEmpty())
    {
        currentProject(nullptr);
        return;
    }

    currentProject(a_projects.constFirst());
}

void ConverterTool::onCurrentProjectChanged(const ConverterProject *project)
{
    if (!project || project->categories().isEmpty())
    {
        currentCategory(nullptr);
        return;
    }

    const auto categories = project->categories();
    currentCategory(categories.first());
}

void ConverterTool::onCurrentCategoryChanged(const ConverterCategory *category)
{
    if (!category || category->units().isEmpty())
    {
        fromUnit(nullptr);
        toUnit(nullptr);
        return;
    }

    const auto units = category->units();
    fromUnit(units.first());
    toUnit(units.first());
}

void ConverterTool::onEditorSavedChanged(bool isSaved)
{
    if (!isSaved) return;

    forceReloadData();
}

void ConverterTool::onAddonManagerLoadingChanged(bool isLoading)
{
    if (isLoading) return;

    if (isDataLoaded())
    {
        forceReloadData();
        return;
    }
}

auto ConverterTool::loadLocalProjects() -> QList<ConverterProject *>
{
    const auto dirs = {u":/units"_s,
                       FileUtils::fileInDir(FileUtils::dirFromFolders({".gm-companion", "units"}), QDir::homePath())};
    return loadLocalProjects(dirs, this);
}

auto ConverterTool::loadLocalProjects(const QStringList &paths, QObject *parent) -> QList<ConverterProject *>
{
    QList<ConverterProject *> projects;

    for (const auto &path : paths)
    {
        const QDir dir(path);
        if (!dir.exists()) continue;

        const auto entries = dir.entryList({"*.json", "*.ini"}, QDir::Files);
        for (const auto &entry : entries)
        {
            auto *project = loadLocalProject(FileUtils::fileInDir(entry, path), parent);

            if (project) projects << project;
        }
    }

    return projects;
}

auto ConverterTool::loadLocalProject(const QString &path, QObject *parent) -> ConverterProject *
{
    QFile file(path);

    if (file.open(QIODevice::ReadOnly))
    {
        const auto data = file.readAll();
        file.close();

        return loadProject(data, parent);
    }

    qCWarning(gmConverter()) << "Could not open" << file.fileName();
    return nullptr;
}

auto ConverterTool::loadProject(const QByteArray &data, QObject *parent) -> ConverterProject *
{
    ConverterProjectUpgrader upgrader;
    upgrader.parse(data);
    const auto result = upgrader.run();

    const auto json = QJsonDocument::fromJson(result.isEmpty() ? data : result).object();
    return new ConverterProject(json, parent);
}

void ConverterTool::loadAddonProjects()
{
    if (AddonManager::instance()->isLoading()) return;

    const auto addons = AddonManager::instance()->addons();

    for (const auto *addon : addons)
    {
        if (!addon || !addon->isInstalledAndEnabled()) continue;

        loadAddonProjects(*addon);
    }
}

void ConverterTool::loadAddonProjects(const Addon &addon)
{
    AddonReader reader(addon);
    if (!reader.getFeatures().testFlag(AddonReader::Feature::Units)) return;

    const auto unitsPath = u"/units"_s;
    const auto files = reader.findAllFiles(unitsPath, {u"*.json"_s});
    for (const auto &file : files)
    {
        const auto data = reader.readFile(FileUtils::fileInDir(file, unitsPath));
        a_projects << loadProject(data, this);
    }
}

void ConverterTool::forceReloadData()
{
    setIsDataLoaded(false);
    loadData();
}

auto ConverterTool::convert(const ConverterUnit *fromUnit, const QString &fromValue, const ConverterUnit *toUnit)
    -> QString
{
    if (!fromUnit || !toUnit) return u""_s;

    if (fromUnit->value() == 0 || toUnit->value() == 0) return u""_s;

    const auto factor = fromUnit->value() / toUnit->value();
    const auto value = textToNumber(fromValue);
    return SettingsManager::getLanguage().toString(factor * value, 'g', 6);
}

auto ConverterTool::editor() -> ConverterEditor *
{
    return &m_editor;
}

auto ConverterTool::textToNumber(QStringView text, bool *ok) -> double
{
    return SettingsManager::getLanguage().toDouble(text, ok);
}

#include "addonelementmanager.h"
#include "addons/addon_reader/addonreader.h"
#include "addons/addonmanager.h"
#include "src/common/utils/fileutils.h"
#include "src/common/utils/utils.h"
#include "src/tools/audio/audiosaveload.h"
#include "src/tools/audio/thumbnails/audiothumbnailgenerator.h"

using namespace Qt::Literals::StringLiterals;

AddonElementManager::AddonElementManager(QObject *parent) : AbstractTool(parent)
{
    connect(this, &AddonElementManager::currentAddonIndexChanged, this,
            &AddonElementManager::onCurrentAddonIndexChanged);
    connect(this, &AddonElementManager::currentProjectIndexChanged, this,
            &AddonElementManager::onCurrentProjectIndexChanged);
}

void AddonElementManager::loadData()
{
    if (isDataLoaded() || isLoading()) return;

    isLoading(true);

    connect(AddonManager::instance(), &AddonManager::addonsChanged, this,
            &AddonElementManager::onInstalledAddonsChanged);

    if (!AddonManager::instance()->isLoading())
    {
        onInstalledAddonsChanged();
    }

    setIsDataLoaded(true);
}

void AddonElementManager::onInstalledAddonsChanged()
{
    clearProjects();
    clearAddons();

    foreach (auto *addon, AddonManager::instance()->addons())
    {
        if (addon->isInstalledAndEnabled())
        {
            AddonReader reader(*addon);
            auto features = reader.getFeatures();

            if (features.testFlag(AddonReader::Feature::Audio))
            {
                a_addons.append(addon);
                loadAddonProjects(*addon);
            }
        }
    }

    emit addonsChanged();

    onCurrentAddonIndexChanged(currentAddonIndex());
    isLoading(false);
}

void AddonElementManager::onCurrentAddonIndexChanged(int index)
{
    currentProjectIndex(-1);

    if (!Utils::isInBounds(addons(), index))
    {
        a_currentAddon = nullptr;
        emit currentAddonChanged(nullptr);
        return;
    }

    a_currentAddon = a_addons.at(index);
    emit currentAddonChanged(nullptr);

    a_availableProjects.clear();

    foreach (auto *project, m_projects.value(a_currentAddon->id()))
    {
        a_availableProjects.append(project);
    }

    emit availableProjectsChanged();

    currentProjectIndex(0);
}

void AddonElementManager::onCurrentScenarioChanged(AudioScenario *scenario) const
{
    AudioThumbnailGenerator::generateThumbnails(scenario);
}

void AddonElementManager::onCurrentProjectIndexChanged(int index)
{
    if (!a_currentAddon) return;
    if (!Utils::isInBounds(m_projects.value(a_currentAddon->id()), index)) return;

    a_currentProject = m_projects.value(a_currentAddon->id()).at(index);
    emit currentProjectChanged(a_currentProject);

    if (!a_currentProject || !a_currentProject->currentScenario()) return;

    AudioThumbnailGenerator::generateThumbnails(a_currentProject->currentScenario());
}

void AddonElementManager::loadAddonProjects(const Addon &addon)
{
    AddonReader reader(addon);

    auto projectFiles = reader.findAllFiles(u"/audio"_s, {u"*.audio"_s, u"*.json"_s});

    QList<AudioProject *> projects;
    projects.reserve(projectFiles.count());

    foreach (const auto &file, projectFiles)
    {
        const auto data = reader.readFile(FileUtils::fileInDir(file, u"/audio"_s));
        auto *project = AudioSaveLoad::loadProject(data, this);

        removeUnsupportedElementsFromProject(*project);
        connect(project, &AudioProject::currentScenarioChanged, this, &AddonElementManager::onCurrentScenarioChanged);

        projects << project;
    }

    // delete previous projects
    if (m_projects.contains(addon.id()))
    {
        auto list = m_projects.take(addon.id());

        foreach (auto *project, list)
        {
            if (project) project->deleteLater();
        }
    }

    m_projects[addon.id()] = projects;
}

/// Remove element types that are not supported (yet)
void AddonElementManager::removeUnsupportedElementsFromProject(AudioProject &project)
{
    QList<AudioCategory *> categoriesToDelete;

    foreach (auto *category, project.categories())
    {
        if (!category) continue;

        removeUnsupportedElementsFromCategory(*category);

        if (category->scenarios().isEmpty())
        {
            categoriesToDelete << category;
        }
    }

    foreach (auto *category, categoriesToDelete)
    {
        project.deleteCategory(category);
    }
}

void AddonElementManager::removeUnsupportedElementsFromCategory(AudioCategory &category)
{
    const auto scenarios = category.scenarios();
    QList<AudioScenario *> scenariosToDelete;

    foreach (auto *scenario, scenarios)
    {
        if (!scenario) continue;

        removeUnsupportedElementsFromScenario(*scenario);
        removeEmptySubscenarios(*scenario);

        if (scenario->elements(true).isEmpty())
        {
            scenariosToDelete << scenario;
        }
    }

    foreach (auto *scenario, scenariosToDelete)
    {
        category.deleteScenario(scenario);
    }
}

void AddonElementManager::removeUnsupportedElementsFromScenario(AudioScenario &scenario)
{
    const auto elements = scenario.elements(true);
    QList<AudioElement *> elementsToDelete;

    foreach (auto *element, elements)
    {
        const auto files = element->files();
        int fileIndex = 0;

        foreach (const auto *file, files)
        {
            switch (file->source())
            {
            case AudioFile::Source::Spotify:
            case AudioFile::Source::Web:
            case AudioFile::Source::Youtube:
                fileIndex++;
                break;
            default:
                element->removeFile(fileIndex);
                break;
            }
        }

        if (element->files().isEmpty())
        {
            elementsToDelete << element;
        }
    }

    foreach (auto *element, elementsToDelete)
    {
        scenario.removeElement(element, true);
    }
}

void AddonElementManager::removeEmptySubscenarios(AudioScenario &scenario)
{
    const auto subscenarios = scenario.scenarios();
    QList<AudioScenario *> scenariosToDelete;
    scenariosToDelete.reserve(subscenarios.size());

    foreach (auto *subscenario, subscenarios)
    {
        removeEmptySubscenarios(*subscenario);

        if (subscenario->elements(true).isEmpty())
        {
            scenariosToDelete << subscenario;
        }
    }

    foreach (auto *subscenario, scenariosToDelete)
    {
        scenario.deleteScenario(subscenario);
    }
}

void AddonElementManager::clearAddons()
{
    if (a_addons.isEmpty()) return;

    a_addons.clear();
    emit addonsChanged();
}

void AddonElementManager::clearProjects()
{
    if (a_availableProjects.isEmpty()) return;

    a_currentProject = nullptr;
    emit currentProjectChanged(nullptr);

    a_availableProjects.clear();
    emit availableProjectsChanged();

    foreach (const auto &key, m_projects.keys())
    {
        auto list = m_projects.take(key);

        foreach (auto *project, list)
        {
            if (project) project->deleteLater();
        }
    }
}

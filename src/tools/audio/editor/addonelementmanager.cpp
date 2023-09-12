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
    connect(this, &AddonElementManager::currentIndexChanged, this, &AddonElementManager::onCurrentIndexChanged);
    connect(this, &AddonElementManager::projectsChanged, this, &AddonElementManager::onProjectsChanged);
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
    QList<Addon *> list;

    foreach (auto *addon, AddonManager::instance()->addons())
    {
        if (addon->isInstalledAndEnabled())
        {
            AddonReader reader(*addon);
            auto features = reader.getFeatures();

            if (features.testFlag(AddonReader::Feature::Audio))
            {
                list << addon;
                loadAddonProjects(*addon);
            }
        }
    }

    a_addons = list;
    emit addonsChanged();
    isLoading(false);
}

void AddonElementManager::onCurrentIndexChanged(int index)
{
    if (!Utils::isInBounds(addons(), index)) return;

    const auto *addon = addons().at(index);

    if (!addon) return;

    a_projects = m_projects[addon->id()];
    emit projectsChanged();
}

void AddonElementManager::onCurrentScenarioChanged(AudioScenario *scenario) const
{
    AudioThumbnailGenerator::generateThumbnails(scenario);
}

void AddonElementManager::onProjectsChanged() const
{
    foreach (auto *project, projects())
    {
        if (!project || !project->currentScenario()) continue;

        AudioThumbnailGenerator::generateThumbnails(project->currentScenario());
    }
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

        for (auto *project : list)
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

    for (auto *category : qAsConst(categoriesToDelete))
    {
        project.deleteCategory(category);
    }
}

void AddonElementManager::removeUnsupportedElementsFromCategory(AudioCategory &category)
{
    const auto scenarios = category.scenarios();
    QList<AudioScenario *> scenariosToDelete;

    for (auto *scenario : scenarios)
    {
        if (!scenario) continue;

        removeUnsupportedElementsFromScenario(*scenario);
        removeEmptySubscenarios(*scenario);

        if (scenario->elements(true).isEmpty())
        {
            scenariosToDelete << scenario;
        }
    }

    for (auto *scenario : qAsConst(scenariosToDelete))
    {
        category.deleteScenario(scenario);
    }
}

void AddonElementManager::removeUnsupportedElementsFromScenario(AudioScenario &scenario)
{
    const auto elements = scenario.elements(true);
    QList<AudioElement *> elementsToDelete;

    for (auto *element : elements)
    {
        const auto files = element->files();
        int fileIndex = 0;

        for (const auto *file : files)
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

    for (auto *element : qAsConst(elementsToDelete))
    {
        scenario.removeElement(element, true);
    }
}

void AddonElementManager::removeEmptySubscenarios(AudioScenario &scenario)
{
    const auto subscenarios = scenario.scenarios();
    QList<AudioScenario *> scenariosToDelete;
    scenariosToDelete.reserve(subscenarios.size());

    for (auto *subscenario : subscenarios)
    {
        removeEmptySubscenarios(*subscenario);

        if (subscenario->elements(true).isEmpty())
        {
            scenariosToDelete << subscenario;
        }
    }

    for (auto *subscenario : qAsConst(scenariosToDelete))
    {
        scenario.deleteScenario(subscenario);
    }
}

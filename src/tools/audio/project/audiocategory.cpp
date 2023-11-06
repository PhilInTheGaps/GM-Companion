#include "audiocategory.h"
#include "audioproject.h"
#include "src/common/utils/fileutils.h"
#include "src/common/utils/utils.h"
#include <QJsonArray>
#include <QLoggingCategory>
#include <utility>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioCategory, "gm.audio.project.category")

AudioCategory::AudioCategory(const QString &name, const QString &parentPath, QList<AudioScenario *> scenarios,
                             AudioProject *parent)
    : TreeItem(name, 0, true, parent), a_scenarios(std::move(scenarios)), m_path(parentPath + "/" + name)
{
    this->name(name);

    foreach (auto *scenario, a_scenarios)
    {
        prepareScenario(scenario);
    }

    if (!a_scenarios.isEmpty()) setCurrentScenario(a_scenarios.first());
}

AudioCategory::AudioCategory(const AudioCategory &other)
    : AudioCategory(other.name(), FileUtils::dirFromPath(other.path()), Utils::copyList(other.scenarios()),
                    qobject_cast<AudioProject *>(other.parent()))
{
}

AudioCategory::AudioCategory(const QJsonObject &object, const QString &path, AudioProject *parent)
    : TreeItem(u""_s, 0, true, parent)
{
    name(object["name"_L1].toString());
    m_path = path + "/" + name();

    const auto scenarios = object["scenarios"_L1].toArray();
    a_scenarios.reserve(scenarios.size());

    foreach (const auto &scenarioJson, scenarios)
    {
        auto *scenario = new AudioScenario(scenarioJson.toObject(), m_path, this);
        prepareScenario(scenario);
        a_scenarios.append(scenario);
    }

    if (!a_scenarios.isEmpty()) setCurrentScenario(a_scenarios.first());
}

/**
 * @brief Serialize the category to json.
 */
auto AudioCategory::toJson() const -> QJsonObject
{
    QJsonObject object = {{"name", name()}};
    QJsonArray scenariosJson;

    foreach (const auto *scenario, a_scenarios)
    {
        if (scenario)
        {
            scenariosJson.append(scenario->toJson());
        }
    }

    object.insert(u"scenarios"_s, scenariosJson);
    return object;
}

/**
 * @brief Set the current scenario.
 * Returns false if scenario could not be set.
 */
auto AudioCategory::setCurrentScenario(AudioScenario *scenario) -> bool
{
    if (!scenario || !a_scenarios.contains(scenario))
    {
        m_currentScenario = nullptr;
        emit currentScenarioChanged(nullptr);
        return false;
    }

    qCDebug(gmAudioCategory()) << "Setting current scenario:" << scenario->name() << "...";

    m_currentScenario = scenario;
    emit currentScenarioChanged(scenario);
    return true;
}

auto AudioCategory::containsScenario(const QString &name) const -> bool
{
    return std::any_of(a_scenarios.constBegin(), a_scenarios.constEnd(),
                       [name](const AudioScenario *scenario) { return scenario && scenario->name() == name; });
}

/**
 * @brief Add a scenario to the category.
 * Returns false if the operation failed.
 */
auto AudioCategory::addScenario(AudioScenario *scenario, bool setAsCurrent) -> bool
{
    if (!scenario) return false;

    prepareScenario(scenario);
    a_scenarios.append(scenario);
    emit scenariosChanged();

    if (setAsCurrent)
    {
        return setCurrentScenario(scenario);
    }

    return true;
}

/**
 * @brief Remove a scenario from the category.
 * Returns false if scenario could not be deleted.
 */
auto AudioCategory::deleteScenario(AudioScenario *scenario) -> bool
{
    if (!scenario || !a_scenarios.contains(scenario)) return false;

    if (!a_scenarios.removeOne(scenario)) return false;

    scenario->deleteLater();
    emit scenariosChanged();
    return true;
}

/**
 * @brief Get a list of all elements in the category.
 */
auto AudioCategory::elements() const -> QList<AudioElement *>
{
    QList<AudioElement *> list;

    foreach (auto *scenario, a_scenarios)
    {
        list.append(scenario->elements(true));
    }

    return list;
}

auto AudioCategory::prepareScenario(AudioScenario *scenario) -> void
{
    if (!scenario) return;
    scenario->setParent(this);

    connect(scenario, &AudioScenario::wasEdited, this, &AudioCategory::wasEdited);
    connect(scenario, &AudioScenario::nameChanged, this, &AudioCategory::wasEdited);
    connect(scenario, &AudioScenario::elementsChanged, this, &AudioCategory::wasEdited);
    connect(scenario, &AudioScenario::scenariosChanged, this, &AudioCategory::wasEdited);
}

#include "audiocategory.h"
#include "audioproject.h"
#include "src/common/utils/fileutils.h"
#include "src/common/utils/utils.h"
#include <QJsonArray>
#include <QLoggingCategory>
#include <utility>

Q_LOGGING_CATEGORY(gmAudioCategory, "gm.audio.project.category")

AudioCategory::AudioCategory(const QString &name, const QString &parentPath, QList<AudioScenario *> scenarios,
                             AudioProject *parent)
    : TreeItem(name, 0, true, parent), m_path(parentPath + "/" + name), m_scenarios(std::move(scenarios))
{
    this->name(name);

    for (auto *scenario : qAsConst(m_scenarios))
    {
        prepareScenario(scenario);
    }

    if (!m_scenarios.isEmpty()) setCurrentScenario(m_scenarios.first());
}

AudioCategory::AudioCategory(const AudioCategory &other)
    : AudioCategory(other.name(), FileUtils::dirFromPath(other.path()), Utils::copyList(other.scenarios()),
                    qobject_cast<AudioProject *>(other.parent()))
{
}

AudioCategory::AudioCategory(const QJsonObject &object, const QString &path, AudioProject *parent)
    : TreeItem(QLatin1String(""), 0, true, parent)
{
    name(object[QStringLiteral("name")].toString());
    m_path = path + "/" + name();

    const auto scenarios = object[QStringLiteral("scenarios")].toArray();
    m_scenarios.reserve(scenarios.size());

    for (const auto &scenario : scenarios)
    {
        auto *object = new AudioScenario(scenario.toObject(), m_path, this);
        prepareScenario(object);
        m_scenarios.append(object);
    }

    if (!m_scenarios.isEmpty()) setCurrentScenario(m_scenarios.first());
}

/**
 * @brief Serialize the category to json.
 */
auto AudioCategory::toJson() const -> QJsonObject
{
    QJsonObject object = {{"name", name()}};

    QJsonArray scenariosJson;

    foreach (const auto *scenario, m_scenarios)
    {
        if (scenario)
        {
            scenariosJson.append(scenario->toJson());
        }
    }

    object.insert(QStringLiteral("scenarios"), scenariosJson);
    return object;
}

/**
 * @brief Set the current scenario.
 * Returns false if scenario could not be set.
 */
auto AudioCategory::setCurrentScenario(AudioScenario *scenario) -> bool
{
    if (!scenario || !m_scenarios.contains(scenario))
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
    return std::any_of(m_scenarios.constBegin(), m_scenarios.constEnd(),
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
    m_scenarios.append(scenario);
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
    if (!scenario || !m_scenarios.contains(scenario)) return false;

    if (!m_scenarios.removeOne(scenario)) return false;

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

    foreach (auto *scenario, m_scenarios)
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

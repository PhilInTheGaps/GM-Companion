#include "audiocategory.h"

#include <QJsonArray>
#include <utility>

AudioCategory::AudioCategory(const QString& name, const QString& path, QList<AudioScenario*> scenarios, QObject *parent)
    : QObject(parent), m_name(name), m_path(path + "/" + name), m_scenarios(std::move(scenarios))
{
    for (auto scenario : m_scenarios)
    {
        scenario->setParent(this);
    }

    if (!m_scenarios.isEmpty()) setCurrentScenario(m_scenarios.first());
}

AudioCategory::AudioCategory(QJsonObject object, const QString& path, QObject *parent) : QObject(parent)
{
    m_name = object["name"].toString();
    m_path = path + "/" + m_name;

    for (auto scenario : object["scenarios"].toArray())
    {
        m_scenarios.append(new AudioScenario(scenario.toObject(), m_path, this));
    }

    if (!m_scenarios.isEmpty()) setCurrentScenario(m_scenarios.first());
}

/**
 * @brief Serialize the category to json.
 */
auto AudioCategory::toJson() const -> QJsonObject
{
    QJsonObject object;

    object.insert("name", m_name);

    QJsonArray scenariosJson;

    for (auto scenario : m_scenarios)
    {
        if (scenario) scenariosJson.append(scenario->toJson());
    }

    object.insert("scenarios", scenariosJson);

    return object;
}

/**
 * @brief A list with the names of the scenarios in this category.
 */
auto AudioCategory::scenarioNames() const -> QStringList
{
    QStringList names;

    for (auto *s : m_scenarios)
    {
        names.append(s->name());
    }

    return names;
}

/**
 * @brief Set the current scenario.
 * Returns false if scenario with name could not be found.
 * @param name Name of the scenario
 */
auto AudioCategory::setCurrentScenario(const QString& name) -> bool
{
    qDebug() << "AudioCategory: Setting current scenario:" << name << "...";

    for (auto scenario : m_scenarios)
    {
        if (scenario && scenario->name() == name)
        {
            setCurrentScenario(scenario);
            return true;
        }
    }

    return false;
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
        emit currentScenarioChanged();
        return false;
    }

    qDebug() << "AudioCategory: Setting current scenario:" << scenario->name() << "...";

    m_currentScenario = scenario;
    emit currentScenarioChanged();
    return true;
}

/**
 * @brief Get a list of the current scenario and all subscenarios as QObjects.
 */
QList<QObject *> AudioCategory::currentScenarioModel() const
{
    QList<QObject*> list = { qobject_cast<QObject*>(currentScenario()) };

    for (auto scenario : currentScenario()->scenarios())
    {
        list.push_back(qobject_cast<QObject*>(scenario));
    }

    return list;
}

/**
 * @brief Add a scenario to the category.
 * Returns false if the operation failed.
 */
auto AudioCategory::addScenario(AudioScenario *scenario) -> bool
{
    if (!scenario) return false;

    scenario->setParent(this);
    m_scenarios.append(scenario);
    emit scenariosChanged();
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

    for (auto scenario : m_scenarios)
    {
        list.append(scenario->elements(true));
    }

    return list;
}

#include "audioscenario.h"

#include <QJsonArray>
#include <utility>

AudioScenario::AudioScenario(const QString &name, const QString &path, QList<AudioElement*> musicLists,
                             QList<AudioElement*> soundLists, QList<AudioElement*> radios,
                             QList<AudioScenario*> scenarios, QObject *parent)
    : QObject(parent), m_name(name), m_path(path + "/" + name), m_musicLists(std::move(musicLists)),
      m_soundLists(std::move(soundLists)), m_radios(std::move(radios)), m_scenarios(std::move(scenarios))
{
    for (auto *element : m_musicLists) element->setParent(this);

    for (auto *element : m_soundLists) element->setParent(this);

    for (auto *element : m_radios) element->setParent(this);

    for (auto *scenario : m_scenarios) scenario->setParent(this);
}

AudioScenario::AudioScenario(const QJsonObject& object, const QString& path, QObject *parent)
    : QObject(parent)
{
    m_name = object["name"].toString();
    m_path = path + "/" + m_name;

    for (auto element : object.value("music_elements").toArray())
    {
        m_musicLists.append(new AudioElement(element.toObject(), AudioElement::Type::Music, m_path, this));
    }

    for (auto element : object.value("sound_elements").toArray())
    {
        m_soundLists.append(new AudioElement(element.toObject(), AudioElement::Type::Sound, m_path, this));
    }

    for (auto element : object.value("radio_elements").toArray())
    {
        m_radios.append(new AudioElement(element.toObject(), AudioElement::Type::Radio, m_path, this));
    }

    for (auto scenario : object.value("scenarios").toArray())
    {
        m_scenarios.append(new AudioScenario(scenario.toObject(), m_path, this));
    }
}

/**
 * @brief Serialize the scenario to json.
 */
auto AudioScenario::toJson() const -> QJsonObject
{
    QJsonObject object;
    QJsonArray  musicElementsJson;
    QJsonArray  soundElementsJson;
    QJsonArray  radioElementsJson;

    object.insert("name", m_name);

    for (auto element : elements())
    {
        if (!element) continue;

        switch (element->type())
        {
        case AudioElement::Type::Music:
            musicElementsJson.append(element->toJson());
            break;

        case AudioElement::Type::Sound:
            soundElementsJson.append(element->toJson());
            break;

        case AudioElement::Type::Radio:
            radioElementsJson.append(element->toJson());
            break;

        default:
            break;
        }
    }

    object.insert("music_elements", musicElementsJson);
    object.insert("sound_elements", soundElementsJson);
    object.insert("radio_elements", radioElementsJson);

    QJsonArray scenariosJson;

    for (auto scenario : m_scenarios)
    {
        if (scenario) scenariosJson.append(scenario->toJson());
    }

    object.insert("scenarios", scenariosJson);

    return object;
}

/**
 * @brief Get all elements
 * @return List of pointers to audio elements
 */
auto AudioScenario::elements(bool recursive) const -> QList<AudioElement *>
{
    QList<AudioElement *> list;

    list.append(m_musicLists);
    list.append(m_soundLists);
    list.append(m_radios);

    if (recursive)
    {
        for (auto scenario : m_scenarios)
        {
            list.append(scenario->elements(recursive));
        }
    }

    return list;
}

/**
 * @brief Get all elements
 * @return List of pointers to audio elements
 */
auto AudioScenario::elements(const AudioElement::Type &type, bool recursive) -> QList<AudioElement *>
{
    if (!recursive) return getElementList(type);

    QList<AudioElement *> list;

    list.append(getElementList(type));

    for (auto scenario : m_scenarios)
    {
        list.append(scenario->elements(type, recursive));
    }

    return list;
}

/**
 * @brief Get a pointer to an element, depending on the element type.
 */
auto AudioScenario::element(const QString &name, const AudioElement::Type &type) -> AudioElement *
{
    for (auto element : getElementList(type))
    {
        if (element->name() == name)
        {
            return element;
        }
    }

    return nullptr;
}

/**
 * @brief Get a list of all element names of a type.
 */
auto AudioScenario::elementNames(const AudioElement::Type &type) -> QStringList
{
    QStringList list;

    for (auto element : getElementList(type))
    {
        list.append(element->name());
    }

    return list;
}

/**
 * @brief Move an element
 * @param name Name of the element
 * @param steps Positions to move element by
 * @param type The type of the element
 */
auto AudioScenario::moveElement(const QString &name, int steps, const AudioElement::Type &type) -> bool
{
    int index = getElementList(type).indexOf(element(name, type));

    if ((index + steps < m_musicLists.size()) && (index + steps > -1))
    {
        m_musicLists.move(index, index + steps);
        return true;
    }

    return false;
}

/**
 * @brief Add an element to the scenario;
 */
auto AudioScenario::addElement(AudioElement *element) -> bool
{
    if (!element) return false;

    element->setParent(this);
    getElementList(element->type()).append(element);

    return true;
}

/**
 * @brief Remove an element from the current scenario
 */
auto AudioScenario::removeElement(const QString &name, const AudioElement::Type &type, bool deleteElement) -> bool
{
    for (auto element : getElementList(type))
    {
        if (element->name() == name)
        {
            if (!m_musicLists.removeOne(element))
            {
                return false;
            }

            if (deleteElement) element->deleteLater();

            return true;
        }
    }

    return false;
}

/**
 * @brief Sort all elements alphabetically
 */
void AudioScenario::sortElements()
{
    qDebug() << "Scenario: Sorting elements ...";
    std::sort(m_musicLists.begin(), m_musicLists.end(), AudioElement::compare);
    std::sort(m_soundLists.begin(), m_soundLists.end(), AudioElement::compare);
    std::sort(m_radios.begin(),     m_radios.end(),     AudioElement::compare);
    qDebug() << "Done.";
}

/**
 * @brief Get a pointer to a scenario.
 */
auto AudioScenario::scenario(const QString &name) -> AudioScenario *
{
    for (auto scenario : scenarios())
    {
        if (scenario->name() == name)
        {
            return scenario;
        }
    }

    return nullptr;
}

/**
 * @brief Get a list of all sub scenario names in the scenario.
 */
auto AudioScenario::scenarioNames() const -> QStringList
{
    QStringList list;

    for (auto scenario : scenarios())
    {
        list.append(scenario->name());
    }

    return list;
}

/**
 * @brief Move a scenario
 * @param name Name of the scenario
 * @param steps Positions to move scenario by
 */
auto AudioScenario::moveScenario(const QString &name, int steps) -> bool
{
    int index = m_scenarios.indexOf(scenario(name));

    if ((index + steps < m_scenarios.size()) && (index + steps > -1))
    {
        m_scenarios.move(index, index + steps);
        return true;
    }

    return false;
}

/**
 * @brief Remove a sub scenario from the current scenario
 */
auto AudioScenario::removeScenario(const QString &name) -> bool
{
    for (auto scenario : m_scenarios)
    {
        if (scenario->name() == name)
        {
            if (!m_scenarios.removeOne(scenario))
            {
                return false;
            }

            scenario->deleteLater();
            return true;
        }
    }

    return false;
}

auto AudioScenario::getElementList(const AudioElement::Type &type) -> QList<AudioElement*> &
{
    switch (type)
    {
    case AudioElement::Type::Sound:
        return m_soundLists;
    case AudioElement::Type::Radio:
        return m_radios;
    default:
        return m_musicLists;
    }
}

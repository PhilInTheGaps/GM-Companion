#include "audioscenario.h"
#include "logging.h"
#include <QJsonArray>

QString AudioScenario::filterString = QStringLiteral("");

AudioScenario::AudioScenario(const QString &name, const QString &path, QList<AudioElement*> musicLists,
                             QList<AudioElement*> soundLists, QList<AudioElement*> radios,
                             QList<AudioScenario*> scenarios, QObject *parent)
    : QObject(parent), m_name(name), m_path(path + "/" + name), m_musicLists(std::move(musicLists)),
      m_soundLists(std::move(soundLists)), m_radios(std::move(radios)), m_scenarios(std::move(scenarios))
{
    for (auto *element : m_musicLists) prepareElement(element);

    for (auto *element : m_soundLists) prepareElement(element);

    for (auto *element : m_radios) prepareElement(element);

    for (auto *scenario : m_scenarios) prepareScenario(scenario);

    m_isSubscenario = qobject_cast<AudioScenario*>(parent);
}

AudioScenario::AudioScenario(const QJsonObject& object, const QString& path, QObject *parent)
    : QObject(parent)
{
    m_name = object["name"].toString();
    m_path = path + "/" + m_name;

    for (auto element : object.value("music_elements").toArray())
    {
        auto *object = new AudioElement(element.toObject(), AudioElement::Type::Music, m_path, this);
        prepareElement(object);
        m_musicLists.append(object);
    }

    for (auto element : object.value("sound_elements").toArray())
    {
        auto *object = new AudioElement(element.toObject(), AudioElement::Type::Sound, m_path, this);
        prepareElement(object);
        m_soundLists.append(object);
    }

    for (auto element : object.value("radio_elements").toArray())
    {
        auto *object = new AudioElement(element.toObject(), AudioElement::Type::Radio, m_path, this);
        prepareElement(object);
        m_radios.append(object);
    }

    for (auto scenario : object.value("scenarios").toArray())
    {
        auto *object = new AudioScenario(scenario.toObject(), m_path, this);
        prepareScenario(object);
        m_scenarios.append(object);
    }

    m_isSubscenario = qobject_cast<AudioScenario*>(parent);
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

auto AudioScenario::model() -> QList<QObject *>
{
    QList<QObject*> list;

    if (!elements().isEmpty())
    {
        list.append(qobject_cast<QObject*>(this));
    }

    for (auto *scenario : scenarios())
    {
        list.push_back(qobject_cast<QObject*>(scenario));
    }

    return list;
}

/**
 * @brief Get all elements
 * @return List of pointers to audio elements
 */
auto AudioScenario::elements(bool recursive) const -> QList<AudioElement *>
{
    QList<AudioElement *> list;

    if (filterString.isEmpty())
    {
        list.append(m_musicLists);
        list.append(m_soundLists);
        list.append(m_radios);
    }
    else
    {
        for (const auto& elementList : { m_musicLists, m_soundLists, m_radios })
        {
            for (auto element : elementList)
            {
                if (element->name().contains(filterString, Qt::CaseInsensitive))
                {
                    list.push_back(element);
                }
            }
        }
    }

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
    if (!recursive) return *getElementList(type);

    QList<AudioElement *> list;

    list.append(*getElementList(type));

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
    for (auto element : *getElementList(type))
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

    for (auto *element : *getElementList(type))
    {
        list.append(element->name());
    }

    return list;
}

/**
 * @brief Move an element
 * @param name The element to be moved
 * @param steps Positions to move element by
 */
auto AudioScenario::moveElement(AudioElement *element, int steps) -> bool
{
    if (!element) return false;

    auto list = getElementList(element->type());
    int index = list->indexOf(element);
    int indexNew = index + steps;

    if (Utils::isInBounds(*list, indexNew))
    {
        qDebug() << index << indexNew;
        list->move(index, indexNew);
        emit elementsChanged();
        return true;
    }

    for (auto *scenario : m_scenarios)
    {
        if (scenario->moveElement(element, steps))
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Add an element to the scenario;
 */
auto AudioScenario::addElement(AudioElement *element) -> bool
{
    if (!element) return false;

    prepareElement(element);
    getElementList(element->type())->append(element);

    if (elements().length() == 1)
    {
        emit scenariosChanged();
    }

    emit elementsChanged();

    return true;
}

/**
 * @brief Remove an element
 */
auto AudioScenario::removeElement(AudioElement *element, bool deleteElement) -> bool
{
    if (!element) return false;

    if (element->parent() != this)
    {
        for (auto *sub : scenarios())
        {
            if (sub && element->parent() == sub)
            {
                return sub->removeElement(element, deleteElement);
            }
        }

        qWarning() << "Error: AudioScenario can only remove elements it owns.";
        return false;
    }

    getElementList(element->type())->removeOne(element);

    if (elements().isEmpty())
    {
        emit scenariosChanged();
    }

    if (deleteElement) element->deleteLater();

    releaseElement(element);
    emit elementsChanged();
    return true;
}

/**
 * @brief Sort all elements alphabetically
 */
void AudioScenario::sortElements(bool recursive)
{
    qDebug() << "Scenario: Sorting elements ...";
    std::sort(m_musicLists.begin(), m_musicLists.end(), AudioElement::compare);
    std::sort(m_soundLists.begin(), m_soundLists.end(), AudioElement::compare);
    std::sort(m_radios.begin(),     m_radios.end(),     AudioElement::compare);

    if (recursive)
    {
        for (auto *scenario : m_scenarios)
        {
            scenario->sortElements(recursive);
        }
    }

    emit elementsChanged();
    qDebug() << "Done.";
}

void AudioScenario::refreshElements()
{
    emit elementsChanged();

    for (auto scenario : m_scenarios)
    {
        scenario->refreshElements();
    }
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

auto AudioScenario::addScenario(AudioScenario *scenario) -> bool
{
    if (!scenario) return false;

    prepareScenario(scenario);
    m_scenarios.append(scenario);
    emit scenariosChanged();
    return true;
}

auto AudioScenario::deleteScenario(AudioScenario *scenario) -> bool
{
    if (!scenario) return false;

    if (scenario->parent() != this)
    {
        qWarning() << "Error: AudioScenario can only remove subscenarios it owns.";
        return false;
    }

    m_scenarios.removeOne(scenario);
    scenario->deleteLater();
    emit scenariosChanged();
    return true;
}

/**
 * @brief Move a scenario
 * @param name The scenario to be moved
 * @param steps Positions to move scenario by
 */
auto AudioScenario::moveScenario(AudioScenario *scenario, int steps) -> bool
{
    if (!scenario) return false;

    if (scenario->parent() != this)
    {
        qWarning() << "Error: AudioScenario can only move subscenarios it owns.";
        return false;
    }

    int from = m_scenarios.indexOf(scenario);
    int to = from + steps;

    if (Utils::isInBounds(m_scenarios, to))
    {
        m_scenarios.move(from, to);
        emit scenariosChanged();
        return true;
    }

    return false;
}

/**
 * @brief Get the scenario that is the parent of the element
 */
auto AudioScenario::getScenario(AudioElement *element) -> AudioScenario *
{
    if (!element) return nullptr;

    if (element->parent() == this) return this;

    for (auto *scenario : scenarios())
    {
        auto *result = scenario->getScenario(element);
        if (result) return result;
    }

    return nullptr;
}

auto AudioScenario::getElementList(const AudioElement::Type &type) -> QList<AudioElement*> *
{
    switch (type)
    {
    case AudioElement::Type::Sound:
        return &m_soundLists;
    case AudioElement::Type::Radio:
        return &m_radios;
    default:
        return &m_musicLists;
    }
}

auto AudioScenario::prepareElement(AudioElement *element) -> void
{
    if (!element) return;
    element->setParent(this);

    connect(element, &AudioElement::nameChanged,  this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::iconChanged,  this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::typeChanged,  this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::modeChanged,  this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::filesChanged, this, &AudioScenario::wasEdited);
}

void AudioScenario::releaseElement(AudioElement *element)
{
    if (!element || element->parent() != this) return;

    disconnect(element, &AudioElement::nameChanged,  this, &AudioScenario::wasEdited);
    disconnect(element, &AudioElement::iconChanged,  this, &AudioScenario::wasEdited);
    disconnect(element, &AudioElement::typeChanged,  this, &AudioScenario::wasEdited);
    disconnect(element, &AudioElement::modeChanged,  this, &AudioScenario::wasEdited);
    disconnect(element, &AudioElement::filesChanged, this, &AudioScenario::wasEdited);
}

void AudioScenario::prepareScenario(AudioScenario *scenario)
{
    if (!scenario) return;
    scenario->setParent(this);

    connect(scenario, &AudioScenario::wasEdited, this, &AudioScenario::wasEdited);
    connect(scenario, &AudioScenario::nameChanged, this, &AudioScenario::wasEdited);
    connect(scenario, &AudioScenario::elementsChanged, this, &AudioScenario::wasEdited);
    connect(scenario, &AudioScenario::scenariosChanged, this, &AudioScenario::wasEdited);
}

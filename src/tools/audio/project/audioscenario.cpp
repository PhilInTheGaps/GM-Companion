#include "audioscenario.h"
#include "audiocategory.h"
#include "logging.h"
#include "src/common/utils/fileutils.h"
#include "src/common/utils/utils.h"
#include <QJsonArray>

AudioScenario::AudioScenario(const QString &name, const QString &path, const AudioScenarioElements &elements,
                             bool isSubscenario, QObject *parent)
    : TreeItem(name, path.split(QStringLiteral("/")).length() - 1, true, parent), a_isSubscenario(isSubscenario),
      m_path(path + QStringLiteral("/") + name), m_musicLists(elements.musicLists), m_soundLists(elements.soundLists),
      m_radios(elements.radios), m_scenarios(elements.scenarios)
{
    this->name(name);

    for (auto *element : qAsConst(m_musicLists))
        prepareElement(element);

    for (auto *element : qAsConst(m_soundLists))
        prepareElement(element);

    for (auto *element : qAsConst(m_radios))
        prepareElement(element);

    for (auto *scenario : qAsConst(m_scenarios))
        prepareScenario(scenario);
}

AudioScenario::AudioScenario(const QString &name, const QString &path, const AudioScenarioElements &elements,
                             AudioCategory *parent)
    : AudioScenario(name, path, elements, false, qobject_cast<QObject *>(parent))
{
}

AudioScenario::AudioScenario(const QString &name, const QString &path, const AudioScenarioElements &elements,
                             AudioScenario *parent)
    : AudioScenario(name, path, elements, true, qobject_cast<QObject *>(parent))
{
}

AudioScenario::AudioScenario(const AudioScenario &other)
    : AudioScenario(other.name(), FileUtils::dirFromPath(other.path()),
                    AudioScenarioElements(Utils::copyList(other.m_musicLists), Utils::copyList(other.m_soundLists),
                                          Utils::copyList(other.m_radios), Utils::copyList(other.m_scenarios)),
                    other.isSubscenario(), other.parent())
{
}

AudioScenario::AudioScenario(const QJsonObject &object, const QString &path, bool isSubscenario, QObject *parent)
    : TreeItem(QLatin1String(), path.split(QStringLiteral("/")).length() - 1, true, parent),
      a_isSubscenario(isSubscenario)
{
    name(object[QStringLiteral("name")].toString());
    m_path = path + QStringLiteral("/") + name();

    foreach (const auto &element, object[QStringLiteral("music_elements")].toArray())
    {
        auto *object = new AudioElement(element.toObject(), AudioElement::Type::Music, m_path, this);
        prepareElement(object);
        m_musicLists.append(object);
    }

    foreach (const auto &element, object[QStringLiteral("sound_elements")].toArray())
    {
        auto *object = new AudioElement(element.toObject(), AudioElement::Type::Sound, m_path, this);
        prepareElement(object);
        m_soundLists.append(object);
    }

    foreach (const auto &element, object[QStringLiteral("radio_elements")].toArray())
    {
        auto *object = new AudioElement(element.toObject(), AudioElement::Type::Radio, m_path, this);
        prepareElement(object);
        m_radios.append(object);
    }

    foreach (const auto &scenario, object[QStringLiteral("scenarios")].toArray())
    {
        auto *object = new AudioScenario(scenario.toObject(), m_path, this);
        prepareScenario(object);
        m_scenarios.append(object);
    }
}

AudioScenario::AudioScenario(const QJsonObject &object, const QString &path, AudioCategory *parent)
    : AudioScenario(object, path, false, qobject_cast<QObject *>(parent))
{
}

AudioScenario::AudioScenario(const QJsonObject &object, const QString &path, AudioScenario *parent)
    : AudioScenario(object, path, true, qobject_cast<QObject *>(parent))
{
}

/**
 * @brief Serialize the scenario to json.
 */
auto AudioScenario::toJson() const -> QJsonObject
{
    QJsonObject object{{"name", name()}};
    QJsonArray musicElementsJson;
    QJsonArray soundElementsJson;
    QJsonArray radioElementsJson;

    foreach (const auto *element, elements())
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

    object.insert(QStringLiteral("music_elements"), musicElementsJson);
    object.insert(QStringLiteral("sound_elements"), soundElementsJson);
    object.insert(QStringLiteral("radio_elements"), radioElementsJson);

    QJsonArray scenariosJson;

    foreach (const auto *scenario, m_scenarios)
    {
        if (scenario) scenariosJson.append(scenario->toJson());
    }

    object.insert(QStringLiteral("scenarios"), scenariosJson);

    return object;
}

auto AudioScenario::model() -> QList<QObject *>
{
    QList<QObject *> list;
    list.reserve(scenarios().length() + 1);

    if (!elements().isEmpty())
    {
        list << this;
    }

    foreach (auto *scenario, scenarios())
    {
        list << scenario;
    }

    return list;
}

void AudioScenario::refreshElements()
{
    emit elementsChanged();

    foreach (auto *scenario, m_scenarios)
    {
        scenario->refreshElements();
    }
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
        for (const auto &elementList : {m_musicLists, m_soundLists, m_radios})
        {
            foreach (auto *element, elementList)
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
        for (auto *scenario : m_scenarios)
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
auto AudioScenario::elements(AudioElement::Type type, bool recursive) -> QList<AudioElement *>
{
    if (!recursive) return getElementList(type);

    QList<AudioElement *> list;
    list.append(getElementList(type));

    foreach (auto *scenario, m_scenarios)
    {
        list.append(scenario->elements(type, recursive));
    }

    return list;
}

/**
 * @brief Get a pointer to an element, depending on the element type.
 */
auto AudioScenario::element(const QString &name, AudioElement::Type type) -> AudioElement *
{
    foreach (auto *element, getElementList(type))
    {
        if (element->name() == name)
        {
            return element;
        }
    }

    return nullptr;
}

/**
 * @brief Move an element
 * @param name The element to be moved
 * @param steps Positions to move element by
 */
auto AudioScenario::moveElement(AudioElement *element, int steps) -> bool
{
    if (!element) return false;

    auto &list = getElementList(element->type());
    int index = list.indexOf(element);
    int indexNew = index + steps;

    if (Utils::isInBounds(list, indexNew))
    {
        qDebug() << index << indexNew;
        list.move(index, indexNew);
        emit elementsChanged();
        return true;
    }

    foreach (auto *scenario, m_scenarios)
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
    getElementList(element->type()).append(element);

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
        foreach (auto *sub, scenarios())
        {
            if (sub && element->parent() == sub)
            {
                return sub->removeElement(element, deleteElement);
            }
        }

        qWarning() << "Error: AudioScenario can only remove elements it owns.";
        return false;
    }

    getElementList(element->type()).removeOne(element);

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
    std::sort(m_radios.begin(), m_radios.end(), AudioElement::compare);

    if (recursive)
    {
        foreach (auto *scenario, m_scenarios)
        {
            scenario->sortElements(recursive);
        }
    }

    emit elementsChanged();
    qDebug() << "Done.";
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

    const int from = m_scenarios.indexOf(scenario);
    const int to = from + steps;

    if (Utils::isInBounds(m_scenarios, to))
    {
        m_scenarios.move(from, to);
        emit scenariosChanged();
        return true;
    }

    return false;
}

auto AudioScenario::containsScenario(const QString &name) const -> bool
{
    foreach (const auto *scenario, scenarios())
    {
        if (scenario && scenario->name() == name) return true;
    }

    return false;
}

auto AudioScenario::getElementList(AudioElement::Type type) -> QList<AudioElement *> &
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

auto AudioScenario::prepareElement(AudioElement *element) -> void
{
    if (!element) return;
    element->setParent(this);

    connect(element, &AudioElement::nameChanged, this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::thumbnailChanged, this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::modeChanged, this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::filesChanged, this, &AudioScenario::wasEdited);
}

void AudioScenario::releaseElement(AudioElement *element) const
{
    if (!element || element->parent() != this) return;

    disconnect(element, &AudioElement::nameChanged, this, &AudioScenario::wasEdited);
    disconnect(element, &AudioElement::thumbnailChanged, this, &AudioScenario::wasEdited);
    disconnect(element, &AudioElement::modeChanged, this, &AudioScenario::wasEdited);
    disconnect(element, &AudioElement::filesChanged, this, &AudioScenario::wasEdited);
}

void AudioScenario::prepareScenario(AudioScenario *scenario)
{
    if (!scenario) return;
    scenario->setParent(this);
    scenario->isSubscenario(true);

    connect(scenario, &AudioScenario::wasEdited, this, &AudioScenario::wasEdited);
    connect(scenario, &AudioScenario::nameChanged, this, &AudioScenario::wasEdited);
    connect(scenario, &AudioScenario::elementsChanged, this, &AudioScenario::wasEdited);
    connect(scenario, &AudioScenario::scenariosChanged, this, &AudioScenario::wasEdited);
}

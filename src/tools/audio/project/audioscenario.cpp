#include "audioscenario.h"
#include "audiocategory.h"
#include "src/common/utils/fileutils.h"
#include "src/common/utils/utils.h"
#include <QJsonArray>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioScenario, "gm.audio.project.scenario")

AudioScenario::AudioScenario(const QString &name, const QString &path, const QList<AudioElement *> &elements,
                             const QList<AudioScenario *> &scenarios, bool isSubscenario, QObject *parent)
    : TreeItem(name, static_cast<int>(path.split(u"/"_s).length()) - 1, true, parent), a_scenarios(scenarios),
      a_isSubscenario(isSubscenario), m_path(path + u"/"_s + name), m_elements(elements)
{
    this->name(name);

    connect(this, &AudioScenario::scenariosChanged, this, &AudioScenario::onScenariosChanged);

    foreach (auto *element, m_elements)
        prepareElement(element);

    foreach (auto *scenario, a_scenarios)
        prepareScenario(scenario);

    updateModel();
}

AudioScenario::AudioScenario(const QString &name, const QString &path, const QList<AudioElement *> &elements,
                             const QList<AudioScenario *> &scenarios, AudioCategory *parent)
    : AudioScenario(name, path, elements, scenarios, false, qobject_cast<QObject *>(parent))
{
}

AudioScenario::AudioScenario(const QString &name, const QString &path, const QList<AudioElement *> &elements,
                             const QList<AudioScenario *> &scenarios, AudioScenario *parent)
    : AudioScenario(name, path, elements, scenarios, true, qobject_cast<QObject *>(parent))
{
}

AudioScenario::AudioScenario(const AudioScenario &other)
    : AudioScenario(other.name(), FileUtils::dirFromPath(other.path()), Utils::copyList(other.m_elements),
                    Utils::copyList(other.a_scenarios), other.isSubscenario(), other.parent())
{
}

AudioScenario::AudioScenario(const QJsonObject &object, const QString &path, bool isSubscenario, QObject *parent)
    : TreeItem(u""_s, static_cast<int>(path.split(u"/"_s).length()) - 1, true, parent), a_isSubscenario(isSubscenario)
{
    name(object["name"_L1].toString());
    m_path = path + u"/"_s + name();

    connect(this, &AudioScenario::scenariosChanged, this, &AudioScenario::onScenariosChanged);

    foreach (const auto &elementJson, object["music_elements"_L1].toArray())
    {
        auto *element = new AudioElement(elementJson.toObject(), AudioElement::Type::Music, m_path, this);
        prepareElement(element);
        m_elements.append(element);
    }

    foreach (const auto &elementJson, object["sound_elements"_L1].toArray())
    {
        auto *element = new AudioElement(elementJson.toObject(), AudioElement::Type::Sound, m_path, this);
        prepareElement(element);
        m_elements.append(element);
    }

    foreach (const auto &elementJson, object["radio_elements"_L1].toArray())
    {
        auto *element = new AudioElement(elementJson.toObject(), AudioElement::Type::Radio, m_path, this);
        prepareElement(element);
        m_elements.append(element);
    }

    foreach (const auto &scenarioJson, object["scenarios"_L1].toArray())
    {
        auto *scenario = new AudioScenario(scenarioJson.toObject(), m_path, this);
        prepareScenario(scenario);
        a_scenarios.append(scenario);
    }

    updateModel();
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

    object.insert(u"music_elements"_s, musicElementsJson);
    object.insert(u"sound_elements"_s, soundElementsJson);
    object.insert(u"radio_elements"_s, radioElementsJson);

    QJsonArray scenariosJson;

    foreach (const auto *scenario, a_scenarios)
    {
        if (scenario) scenariosJson.append(scenario->toJson());
    }

    object.insert(u"scenarios"_s, scenariosJson);

    return object;
}

void AudioScenario::refreshElements()
{
    emit elementsChanged();

    foreach (auto *scenario, a_scenarios)
    {
        scenario->refreshElements();
    }
}

void AudioScenario::onScenariosChanged()
{
    updateModel();
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
        if (!recursive)
        {
            return m_elements;
        }

        list.append(m_elements);
    }
    else
    {
        foreach (auto *element, m_elements)
        {
            if (element->name().contains(filterString, Qt::CaseInsensitive))
            {
                list.push_back(element);
            }
        }
    }

    if (recursive)
    {
        foreach (auto *scenario, a_scenarios)
        {
            list.append(scenario->elements(recursive));
        }
    }

    return list;
}

auto AudioScenario::elementsQml() -> QQmlListProperty<AudioElement>
{
    return QQmlListProperty(this, &m_elements);
}

/**
 * @brief Get a pointer to an element, depending on the element type.
 */
auto AudioScenario::element(const QString &name, AudioElement::Type type) -> AudioElement *
{
    foreach (auto *element, m_elements)
    {
        if (element->name() == name && element->type() == type)
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

    auto index = m_elements.indexOf(element);

    if (auto indexNew = index + steps; Utils::isInBounds(m_elements, indexNew))
    {
        qCDebug(gmAudioScenario()) << index << indexNew;
        m_elements.move(index, indexNew);
        emit elementsChanged();
        return true;
    }

    foreach (auto *scenario, a_scenarios)
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
    m_elements.append(element);

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

        qWarning(gmAudioScenario()) << "Error: AudioScenario can only remove elements it owns.";
        return false;
    }

    m_elements.removeOne(element);

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
    qCDebug(gmAudioScenario()) << "Sorting elements ...";
    std::sort(m_elements.begin(), m_elements.end(), AudioElement::compare);

    if (recursive)
    {
        foreach (auto *scenario, a_scenarios)
        {
            scenario->sortElements(recursive);
        }
    }

    emit elementsChanged();
    qCDebug(gmAudioScenario()) << "Done.";
}

auto AudioScenario::addScenario(AudioScenario *scenario) -> bool
{
    if (!scenario) return false;

    prepareScenario(scenario);
    a_scenarios.append(scenario);
    emit scenariosChanged();
    return true;
}

auto AudioScenario::deleteScenario(AudioScenario *scenario) -> bool
{
    if (!scenario) return false;

    if (scenario->parent() != this)
    {
        qCWarning(gmAudioScenario()) << "Error: AudioScenario can only remove subscenarios it owns.";
        return false;
    }

    a_scenarios.removeOne(scenario);
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
        qCWarning(gmAudioScenario()) << "Error: AudioScenario can only move subscenarios it owns.";
        return false;
    }

    const auto from = a_scenarios.indexOf(scenario);

    if (const auto to = from + steps; Utils::isInBounds(a_scenarios, to))
    {
        a_scenarios.move(from, to);
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

auto AudioScenario::prepareElement(AudioElement *element) -> void
{
    if (!element) return;
    element->setParent(this);

    connect(element, &AudioElement::nameChanged, this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::thumbnailChanged, this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::modeChanged, this, &AudioScenario::wasEdited);
    connect(element, &AudioElement::filesChanged, this, &AudioScenario::wasEdited);
}

void AudioScenario::releaseElement(const AudioElement *element) const
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

void AudioScenario::updateModel()
{
    a_model.clear();
    a_model.reserve(scenarios().length() + 1);

    if (!elements().isEmpty())
    {
        a_model << this;
    }

    foreach (auto *scenario, scenarios())
    {
        a_model << scenario;
    }

    emit modelChanged();
}

#include "audioproject.h"
#include <QDebug>

AudioProject::AudioProject(QString name, int version, QList<AudioCategory *>categories, QObject *parent) :
    QObject(parent), m_name(name), m_version(version), m_categories(categories)
{
    qDebug() << "Initializing AudioProject:" << name << "...";
    qDebug() << "   Project version:" << version;
    qDebug() << "   Categories:" << m_categories.size();

    if (m_categories.size() > 0) m_currentCategory = m_categories[0];

    for (auto c : m_categories)
    {
        qDebug() << "       " << c->name();
    }
}

AudioProject::~AudioProject()
{
    for (auto c : m_categories)
    {
        if (c) c->deleteLater();
    }
}

/**
    Get the names of all categories in project
    @return QStringList with category names
 */
QStringList AudioProject::categoryNames()
{
    qDebug() << "AudioProject: Getting category names of project" << m_name << "...";
    QStringList names;

    for (auto *c : m_categories)
    {
        names.append(c->name());
    }

    qDebug() << "Done:" << names;

    return names;
}

void AudioProject::deleteCategory(AudioCategory *category)
{
    if (!category || !m_categories.contains(category)) return;

    m_categories.removeOne(category);

    category->deleteLater();
}

/**
    Set the current category
    @param name Name of the category
 */
void AudioProject::setCurrentCategory(QString name)
{
    qDebug() << "AudioProject: Setting current category:" << name << "...";

    for (auto *c : m_categories)
    {
        if (c->name() == name)
        {
            m_currentCategory = c;
            break;
        }
    }
}

QList<AudioElement *>AudioProject::elements() const
{
    QList<AudioElement *> list;

    qDebug() << m_categories;

    for (auto category : m_categories)
    {
        if (category) list.append(category->elements());
    }

    return list;
}

/**
    Get the names of all scenarios in category
    @return QStringList with scenario names
 */
AudioCategory::~AudioCategory()
{
    for (auto s : m_scenarios)
    {
        if (s) s->deleteLater();
    }
}

QStringList AudioCategory::scenarioNames()
{
    QStringList names;

    for (auto *s : m_scenarios)
    {
        names.append(s->name());
    }

    return names;
}

/**
    Set the current scenario
    @param name Name of the scenario
 */
void AudioCategory::setCurrentScenario(QString name)
{
    qDebug() << "AudioCategory: Setting current scenario:" << name << "...";

    for (auto s : m_scenarios)
    {
        if (s->name() == name)
        {
            m_currentScenario = s;
            break;
        }
    }
}

void AudioCategory::setCurrentScenario(AudioScenario *scenario)
{
    if (!scenario || !m_scenarios.contains(scenario))
    {
        m_currentScenario = scenario;
        return;
    }

    qDebug() << "AudioCategory: Setting current scenario:" << scenario->name() << "...";

    m_currentScenario = scenario;
}

void AudioCategory::deleteScenario(AudioScenario *scenario)
{
    if (!scenario || !m_scenarios.contains(scenario)) return;

    m_scenarios.removeOne(scenario);

    scenario->deleteLater();
}

QList<AudioElement *>AudioCategory::elements() const
{
    QList<AudioElement *> list;

    for (auto scenario : m_scenarios)
    {
        list.append(scenario->elements(true));
    }

    return list;
}

/**
 * @brief Get all elements
 * @return List of pointers to audio elements
 */
AudioScenario::~AudioScenario()
{
    for (auto s : m_scenarios)
    {
        if (s) s->deleteLater();
    }

    for (auto e : elements())
    {
        if (e) e->deleteLater();
    }
}

QList<AudioElement *>AudioScenario::elements(bool recursive)
{
    QList<AudioElement *> list;

    for (AudioElement *e : m_musicLists) list.append(e);

    for (AudioElement *e : m_soundLists) list.append(e);

    for (AudioElement *e : m_radios) list.append(e);

    if (recursive) for (auto scenario : m_scenarios) list.append(scenario->elements());

    return list;
}

/**
 * @brief Get pointer to music list
 * @param element Name of the music list
 * @return Pointer to MusicElement
 */
MusicElement * AudioScenario::musicElement(QString element)
{
    for (auto e : m_musicLists)
    {
        if (e->name() == element) return e;
    }

    return nullptr;
}

/**
 * @brief Get pointer to sound list
 * @param element Name of the sound list
 * @return Pointer to SoundElement
 */
SoundElement * AudioScenario::soundElement(QString element)
{
    for (auto e : m_soundLists)
    {
        if (e->name() == element) return e;
    }

    return nullptr;
}

/**
 * @brief Get pointer to radio
 * @param element Name of the radio
 * @return Pointer to RadioElement
 */
RadioElement * AudioScenario::radioElement(QString element)
{
    for (auto e : m_radios)
    {
        if (e->name() == element) return e;
    }

    return nullptr;
}

AudioScenario * AudioScenario::scenario(QString name)
{
    for (auto e : scenarios())
    {
        if (e->name() == name) return e;
    }

    return nullptr;
}

/**
 * @brief Get a list of all music element names
 * @return List of element names
 */
QStringList AudioScenario::musicElementNames()
{
    QStringList list;

    for (auto e : m_musicLists) list.append(e->name());

    return list;
}

/**
 * @brief Get a list of all sound element names
 * @return List of element names
 */
QStringList AudioScenario::soundElementNames()
{
    QStringList list;

    for (auto e : m_soundLists) list.append(e->name());

    return list;
}

/**
 * @brief Get a list of all radio element names
 * @return List of element names
 */
QStringList AudioScenario::radioElementNames()
{
    QStringList list;

    for (auto e : m_radios) list.append(e->name());

    return list;
}

QStringList AudioScenario::scenarioNames()
{
    QStringList list;

    for (auto e : scenarios()) list.append(e->name());

    return list;
}

/**
 * @brief Helper function for sorting elements
 * @param e1 Element 1
 * @param e2 Element 2
 * @return True if e1 comes alphabetically before e2
 */
bool compareElements(const AudioElement *e1, const AudioElement *e2)
{
    if (e1 && e2) return e1->name() < e2->name();

    return true;
}

/**
 * @brief Sort all elements alphabetically
 */
void AudioScenario::sortElements()
{
    qDebug() << "Scenario: Sorting elements ...";
    std::sort(m_musicLists.begin(), m_musicLists.end(), compareElements);
    std::sort(m_soundLists.begin(), m_soundLists.end(), compareElements);
    std::sort(m_radios.begin(),     m_radios.end(),     compareElements);
    qDebug() << "Done.";
}

/**
 * @brief Move a music element
 * @param name Name of the element
 * @param steps Positions to move element by
 */
void AudioScenario::moveMusicElement(QString name, int steps)
{
    int index = m_musicLists.indexOf(musicElement(name));

    if ((index + steps < m_musicLists.size()) && (index + steps > -1)) m_musicLists.move(index, index + steps);
}

/**
 * @brief Remove a music element from the current scenario
 * @param name Name of the element
 */
void AudioScenario::removeMusicElement(QString name, bool deleteElement)
{
    for (auto e : m_musicLists)
    {
        if (e->name() == name)
        {
            m_musicLists.removeOne(e);

            if (deleteElement) e->deleteLater();
            return;
        }
    }
}

/**
 * @brief Move a sound element
 * @param name Name of the element
 * @param steps Positions to move element by
 */
void AudioScenario::moveSoundElement(QString name, int steps)
{
    int index = m_soundLists.indexOf(soundElement(name));

    if ((index + steps < m_soundLists.size()) && (index + steps > -1)) m_soundLists.move(index, index + steps);
}

/**
 * @brief Remove a sound element from the current scenario
 * @param name Name of the element
 * @param deleteElement True if element should be deleted
 */
void AudioScenario::removeSoundElement(QString name, bool deleteElement)
{
    for (auto e : m_soundLists)
    {
        if (e->name() == name)
        {
            m_soundLists.removeOne(e);

            if (deleteElement) e->deleteLater();
            return;
        }
    }
}

/**
 * @brief Move a radio element
 * @param name Name of the element
 * @param steps Positions to move element by
 */
void AudioScenario::moveRadioElement(QString name, int steps)
{
    int index = m_radios.indexOf(radioElement(name));

    if ((index + steps < m_radios.size()) && (index + steps > -1)) m_radios.move(index, index + steps);
}

/**
 * @brief Remove a radio element from the current scenario
 * @param name Name of the element
 */
void AudioScenario::removeRadioElement(QString name, bool deleteElement)
{
    for (auto e : m_radios)
    {
        if (e->name() == name)
        {
            m_radios.removeOne(e);

            if (deleteElement) e->deleteLater();
            return;
        }
    }
}

void AudioScenario::moveScenario(QString name, int steps)
{
    int index = m_scenarios.indexOf(scenario(name));

    if ((index + steps < m_scenarios.size()) && (index + steps > -1)) m_scenarios.move(index, index + steps);
}

void AudioScenario::removeScenario(QString name)
{
    for (auto s : m_scenarios)
    {
        if (s->name() == name)
        {
            m_scenarios.removeOne(s);
            s->deleteLater();
            return;
        }
    }
}

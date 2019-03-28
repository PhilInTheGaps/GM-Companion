#include "audioproject.h"
#include <QDebug>

AudioProject::AudioProject(QString name, int version, QList<AudioCategory *>categories, QObject *parent) :
    QObject(parent), m_name(name), m_version(version), m_categories(categories)
{
    qDebug() << "Initializing AudioProject:" << name << "...";
    qDebug() << "   Project version:" << version;
    qDebug() << "   Categories:" << m_categories.size();

    if (m_categories.size() > 0) m_currentCategory = m_categories[0];

    for (auto *c : m_categories)
    {
        qDebug() << "       " << c->name();
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

/**
    Get the names of all scenarios in category
    @return QStringList with scenario names
 */
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

/**
 * @brief Get all elements
 * @return List of pointers to audio elements
 */
QList<AudioElement *>AudioScenario::elements()
{
    QList<AudioElement *> list;

    for (AudioElement *e : m_musicLists) list.append(e);

    for (AudioElement *e : m_soundLists) list.append(e);

    for (AudioElement *e : m_radios) list.append(e);

    for (AudioElement *e : m_spotifyElements) list.append(e);

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

/**
 * @brief Get pointer to spotify element
 * @param element Name of the spotify element
 * @return Pointer to SpotifyElement
 */
SpotifyElement * AudioScenario::spotifyElement(QString element)
{
    for (auto e : m_spotifyElements)
    {
        if (e->name() == element) return e;
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

/**
 * @brief Get a list of all spotify element names
 * @return List of element names
 */
QStringList AudioScenario::spotifyElementNames()
{
    QStringList list;

    for (auto e : m_spotifyElements) list.append(e->name());

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
    std::sort(m_musicLists.begin(),      m_musicLists.end(),      compareElements);
    std::sort(m_soundLists.begin(),      m_soundLists.end(),      compareElements);
    std::sort(m_radios.begin(),          m_radios.end(),          compareElements);
    std::sort(m_spotifyElements.begin(), m_spotifyElements.end(), compareElements);
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
void AudioScenario::removeMusicElement(QString name)
{
    for (auto e : m_musicLists)
    {
        if (e->name() == name)
        {
            m_musicLists.removeOne(e);
            e->deleteLater();
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
 */
void AudioScenario::removeSoundElement(QString name)
{
    for (auto e : m_soundLists)
    {
        if (e->name() == name)
        {
            m_soundLists.removeOne(e);
            e->deleteLater();
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
void AudioScenario::removeRadioElement(QString name)
{
    for (auto e : m_radios)
    {
        if (e->name() == name)
        {
            m_radios.removeOne(e);
            e->deleteLater();
            return;
        }
    }
}

/**
 * @brief Move a spotify element
 * @param name Name of the element
 * @param steps Positions to move element by
 */
void AudioScenario::moveSpotifyElement(QString name, int steps)
{
    int index = m_spotifyElements.indexOf(spotifyElement(name));

    if ((index + steps < m_spotifyElements.size()) && (index + steps > -1)) m_spotifyElements.move(index, index + steps);
}

/**
 * @brief Remove a spotify element from the current scenario
 * @param name Name of the element
 */
void AudioScenario::removeSpotifyElement(QString name)
{
    for (auto e : m_spotifyElements)
    {
        if (e->name() == name)
        {
            m_spotifyElements.removeOne(e);
            e->deleteLater();
            return;
        }
    }
}

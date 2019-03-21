#ifndef AUDIOPROJECT_H
#define AUDIOPROJECT_H

#include <QObject>
#include "audioelement.h"

class AudioScenario : public QObject
{
    Q_OBJECT
public:
    AudioScenario(QString name, QList<MusicElement*> musicLists, QList<SoundElement*> soundLists, QList<RadioElement*> radios, QList<SpotifyElement*> spotifyElements) :
        m_name(name), m_musicLists(musicLists), m_soundLists(soundLists), m_radios(radios), m_spotifyElements(spotifyElements) {}
    virtual ~AudioScenario() {}

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }
    QList<AudioElement*> elements();
    void sortElements();
    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }

    MusicElement *musicElement(QString name);
    QStringList musicElementNames();
    QList<MusicElement*> musicElements() const { return m_musicLists; }
    void addMusicElement(MusicElement *element) { m_musicLists.append(element); }
    void removeMusicElement(QString name);
    void moveMusicElement(QString name, int steps);

    SoundElement *soundElement(QString name);
    QStringList soundElementNames();
    QList<SoundElement*> soundElements() const { return m_soundLists; }
    void addSoundElement(SoundElement *element) { m_soundLists.append(element); }
    void removeSoundElement(QString name);
    void moveSoundElement(QString name, int steps);

    RadioElement *radioElement(QString name);
    QStringList radioElementNames();
    QList<RadioElement*> radioElements() const { return m_radios; }
    void addRadioElement(RadioElement *element) { m_radios.append(element); }
    void removeRadioElement(QString name);
    void moveRadioElement(QString name, int steps);

    SpotifyElement *spotifyElement(QString name);
    QList<SpotifyElement*> spotifyElements() const { return m_spotifyElements; }
    QStringList spotifyElementNames();
    void addSpotifyElement(SpotifyElement *element) { m_spotifyElements.append(element); }
    void removeSpotifyElement(QString name);
    void moveSpotifyElement(QString name, int steps);

private:
    bool m_export = true;
    QString m_name;
    QList<MusicElement*> m_musicLists;
    QList<SoundElement*> m_soundLists;
    QList<RadioElement*> m_radios;
    QList<SpotifyElement*> m_spotifyElements;
};

class AudioCategory : public QObject
{
    Q_OBJECT
public:
    AudioCategory(QString name, QList<AudioScenario*> scenarios) : m_name(name), m_scenarios(scenarios) { if (m_scenarios.size() > 0) m_currentScenario = m_scenarios[0]; }
    virtual ~AudioCategory() {}

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }
    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }

    QList<AudioScenario*> scenarios() const { return m_scenarios; }
    QStringList scenarioNames();
    void setCurrentScenario(QString name);
    AudioScenario* currentScenario() const { return m_currentScenario; }
    void addScenario(AudioScenario *scenario) { m_scenarios.append(scenario); }
    int scenarioIndex() const { return m_scenarios.indexOf(m_currentScenario); }

private:
    QString m_name;
    bool m_export = true;
    QList<AudioScenario*> m_scenarios;
    AudioScenario *m_currentScenario;
};

class AudioProject : public QObject
{
    Q_OBJECT
public:
    explicit AudioProject(QString name, int version, QList<AudioCategory*> categories, QObject *parent = nullptr);

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }

    QString oldName() const { return m_oldName; }
    void setOldName(QString name) { m_oldName = name; }

    bool wasRenamed() const { return m_wasRenamed; }
    void setWasRenamed(bool renamed) { m_wasRenamed = renamed; }

    int version() const { return m_version; }
    bool isSaved() const { return m_isSaved; }
    void setSaved(bool saved) { m_isSaved = saved; }
    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }

    QList<AudioCategory*> categories() const { return m_categories; }
    QStringList categoryNames();
    int categoryIndex() const { return m_categories.indexOf(m_currentCategory); }

    void setCurrentCategory(QString name);
    void setCurrentCategory(AudioCategory *category = nullptr) { m_currentCategory = category; }

    AudioCategory* currentCategory() const { return m_currentCategory; }
    void addCategory(AudioCategory* category) { m_categories.append(category); }
private:
    QString m_name, m_oldName;
    int m_version;
    bool m_isSaved, m_wasRenamed = false, m_export = true;

    QList<AudioCategory*> m_categories;
    AudioCategory *m_currentCategory;
};

#endif // AUDIOPROJECT_H

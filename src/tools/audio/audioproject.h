#ifndef AUDIOPROJECT_H
#define AUDIOPROJECT_H

#include <QJsonObject>
#include <QObject>
#include "audioelement.h"

class AudioScenario : public QObject
{
    Q_OBJECT
public:
    AudioScenario(QString name, QString path, QList<AudioElement*> musicLists, QList<AudioElement*> soundLists, QList<AudioElement*> radios, QList<AudioScenario*> scenarios) :
        m_name(name), m_path(path + "/" + name), m_musicLists(musicLists), m_soundLists(soundLists), m_radios(radios), m_scenarios(scenarios) {}
    AudioScenario(QJsonObject object, QString path, QObject *parent = nullptr);
    ~AudioScenario();

    QJsonObject toJson();

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }
    QList<AudioElement*> elements(bool recursive = false);
    void sortElements();
    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }
    QString path() const { return m_path; }

    AudioElement *musicElement(QString name);
    QStringList musicElementNames();
    QList<AudioElement*> musicElements() const { return m_musicLists; }
    void addMusicElement(AudioElement *element) { m_musicLists.append(element); }
    void removeMusicElement(QString name, bool deleteElement = true);
    void moveMusicElement(QString name, int steps);

    AudioElement *soundElement(QString name);
    QStringList soundElementNames();
    QList<AudioElement*> soundElements() const { return m_soundLists; }
    void addSoundElement(AudioElement *element) { m_soundLists.append(element); }
    void removeSoundElement(QString name, bool deleteElement = true);
    void moveSoundElement(QString name, int steps);

    AudioElement *radioElement(QString name);
    QStringList radioElementNames();
    QList<AudioElement*> radioElements() const { return m_radios; }
    void addRadioElement(AudioElement *element) { m_radios.append(element); }
    void removeRadioElement(QString name, bool deleteElement = true);
    void moveRadioElement(QString name, int steps);

    AudioScenario *scenario(QString name);
    QList<AudioScenario*> scenarios() const { return m_scenarios; }
    QStringList scenarioNames();
    void addScenario(AudioScenario *scenario) { m_scenarios.append(scenario); }
    void removeScenario(QString name);
    void moveScenario(QString name, int steps);
    void deleteScenario(QString name);

private:
    bool m_export = true;
    QString m_name;
    QString m_path;
    QList<AudioElement*> m_musicLists;
    QList<AudioElement*> m_soundLists;
    QList<AudioElement*> m_radios;
    QList<AudioScenario*> m_scenarios;
};

class AudioCategory : public QObject
{
    Q_OBJECT
public:
    AudioCategory(QString name, QString path, QList<AudioScenario*> scenarios) :
        m_name(name), m_path(path + "/" + name), m_scenarios(scenarios)
        { if (m_scenarios.size() > 0) m_currentScenario = m_scenarios[0]; }
    AudioCategory(QJsonObject object, QString path, QObject *parent = nullptr);
    ~AudioCategory();

    QJsonObject toJson();

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; }
    void setExport(bool e) { m_export = e; }
    bool isExport() const { return m_export; }
    QString path() const { return m_path; }

    QList<AudioScenario*> scenarios() const { return m_scenarios; }
    QStringList scenarioNames();
    void setCurrentScenario(QString name);
    void setCurrentScenario(AudioScenario *scenario);
    AudioScenario* currentScenario() const { return m_currentScenario; }
    void addScenario(AudioScenario *scenario) { m_scenarios.append(scenario); }
    int scenarioIndex() const { return m_scenarios.indexOf(m_currentScenario); }
    void deleteScenario(AudioScenario *scenario);

    QList<AudioElement *> elements() const;

private:
    QString m_name, m_path;
    bool m_export = true;
    QList<AudioScenario*> m_scenarios;
    AudioScenario *m_currentScenario = nullptr;
};

class AudioProject : public QObject
{
    Q_OBJECT
public:
    AudioProject(QString name, int version, QList<AudioCategory*> categories, QObject *parent = nullptr);
    ~AudioProject();

    AudioProject(QJsonObject object, QObject *parent = nullptr);
    QJsonObject toJson();


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
    void deleteCategory(AudioCategory *category);

    void setCurrentCategory(QString name);
    void setCurrentCategory(AudioCategory *category = nullptr) { m_currentCategory = category; }

    AudioCategory* currentCategory() const { return m_currentCategory; }
    void addCategory(AudioCategory* category) { m_categories.append(category); }

    QList<AudioElement *> elements() const;

private:
    QString m_name, m_oldName;
    int m_version;
    bool m_isSaved, m_wasRenamed = false, m_export = true;

    QList<AudioCategory*> m_categories;
    AudioCategory *m_currentCategory = nullptr;
};

#endif // AUDIOPROJECT_H

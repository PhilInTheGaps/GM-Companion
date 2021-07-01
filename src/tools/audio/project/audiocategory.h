#ifndef AUDIOCATEGORY_H
#define AUDIOCATEGORY_H

#include "audioscenario.h"
#include "utils/utils.h"

class AudioCategory : public TreeItem
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(QList<QObject*> scenarios READ scenarioObjects NOTIFY scenariosChanged)
    Q_PROPERTY(QObject* currentScenario READ currentScenarioObject NOTIFY currentScenarioChanged)
    Q_PROPERTY(int scenarioIndex READ scenarioIndex NOTIFY currentScenarioChanged)

public:
    AudioCategory(const QString& name, const QString& path, QList<AudioScenario*> scenarios, QObject *parent);
    AudioCategory(const QJsonObject &object, const QString& path, QObject *parent);

    QJsonObject toJson() const;

    bool isCheckable() const override { return true; }

    QString path() const override { return m_path; }

    QList<AudioScenario*> scenarios() const { return m_scenarios; }
    QList<QObject*> scenarioObjects() const { return Utils::toQObjectList<AudioScenario*>(m_scenarios); }
    int scenarioIndex() const { return m_scenarios.indexOf(m_currentScenario); }
    QStringList scenarioNames() const;
    void refreshElements() { if (m_currentScenario) m_currentScenario->refreshElements(); }

    Q_INVOKABLE bool setCurrentScenario(AudioScenario *scenario);
    AudioScenario* currentScenario() const { return m_currentScenario; }
    QObject* currentScenarioObject() const { return qobject_cast<QObject*>(currentScenario()); }

    bool addScenario(AudioScenario *scenario, bool setAsCurrent = false);
    bool deleteScenario(AudioScenario *scenario);

    QList<AudioElement *> elements() const;

signals:
    void pathChanged();
    void scenariosChanged();
    void currentScenarioChanged();
    void wasEdited();

private:
    QString m_path;
    QList<AudioScenario*> m_scenarios;
    AudioScenario *m_currentScenario = nullptr;

    void prepareScenario(AudioScenario *scenario);
};

#endif

#ifndef AUDIOCATEGORY_H
#define AUDIOCATEGORY_H

#include "audioscenario.h"

class AudioProject;

class AudioCategory : public TreeItem
{
    Q_OBJECT
public:
    AudioCategory(const QString& name, const QString& path, QList<AudioScenario*> scenarios, AudioProject *parent);
    AudioCategory(const QJsonObject &object, const QString& path, AudioProject *parent);

    [[nodiscard]] auto toJson() const -> QJsonObject;
    [[nodiscard]] auto isCheckable() const -> bool override { return true; }

    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    [[nodiscard]] auto path() const -> QString override { return m_path; }

    Q_PROPERTY(QList<AudioScenario*> scenarios READ scenarios NOTIFY scenariosChanged)
    [[nodiscard]] auto scenarios() const -> QList<AudioScenario*> { return m_scenarios; }

    Q_PROPERTY(AudioScenario* currentScenario READ currentScenario WRITE setCurrentScenario NOTIFY currentScenarioChanged)
    [[nodiscard]] auto currentScenario() const -> AudioScenario* { return m_currentScenario; }
    auto setCurrentScenario(AudioScenario *scenario) -> bool;

    Q_PROPERTY(int scenarioIndex READ scenarioIndex NOTIFY currentScenarioChanged)
    [[nodiscard]] auto scenarioIndex() const -> int { return m_scenarios.indexOf(m_currentScenario); }
    [[nodiscard]] auto containsScenario(const QString &name) const -> bool;

    auto addScenario(AudioScenario *scenario, bool setAsCurrent = false) -> bool;
    auto deleteScenario(AudioScenario *scenario) -> bool;

    [[nodiscard]] auto elements() const -> QList<AudioElement *>;
    void refreshElements() { if (m_currentScenario) m_currentScenario->refreshElements(); }

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

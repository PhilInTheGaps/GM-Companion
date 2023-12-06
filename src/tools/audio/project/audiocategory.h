#pragma once

#include "audioscenario.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QtQml/qqmlregistration.h>

class AudioProject;

class AudioCategory : public TreeItem
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    explicit AudioCategory(const QString &name, const QString &path, QList<AudioScenario *> scenarios,
                           AudioProject *parent);
    explicit AudioCategory(const AudioCategory &other);
    explicit AudioCategory(const QJsonObject &object, const QString &path, AudioProject *parent);
    ~AudioCategory() override = default;

    [[nodiscard]] auto toJson() const -> QJsonObject;
    [[nodiscard]] auto isCheckable() const -> bool override
    {
        return true;
    }

    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    [[nodiscard]] auto path() const -> QString override
    {
        return m_path;
    }

    Q_PROPERTY(
        AudioScenario *currentScenario READ currentScenario WRITE setCurrentScenario NOTIFY currentScenarioChanged)
    [[nodiscard]] auto currentScenario() const -> AudioScenario *
    {
        return m_currentScenario;
    }
    auto setCurrentScenario(AudioScenario *scenario) -> bool;

    Q_PROPERTY(int scenarioIndex READ scenarioIndex NOTIFY currentScenarioChanged)
    [[nodiscard]] auto scenarioIndex() const -> int // needs to be int for qml
    {
        return static_cast<int>(a_scenarios.indexOf(m_currentScenario));
    }
    [[nodiscard]] auto containsScenario(const QString &name) const -> bool;

    auto addScenario(AudioScenario *scenario, bool setAsCurrent = false) -> bool;
    auto deleteScenario(AudioScenario *scenario) -> bool;

    [[nodiscard]] auto elements() const -> QList<AudioElement *>;

    READ_LIST_PROPERTY(AudioScenario, scenarios)

signals:
    void pathChanged();
    void currentScenarioChanged(AudioScenario *scenario);

private:
    QString m_path;
    AudioScenario *m_currentScenario = nullptr;

    void prepareScenario(AudioScenario *scenario);
};

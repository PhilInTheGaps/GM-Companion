#ifndef AUDIOCATEGORY_H
#define AUDIOCATEGORY_H

#include "audioscenario.h"
#include "../../../utils/utils.h"

class AudioCategory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool isMarkedForExport READ isMarkedForExport WRITE setIsMarkedForExport NOTIFY isMarkedForExportChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(QList<QObject*> scenarios READ scenarioObjects NOTIFY scenariosChanged)
    Q_PROPERTY(QObject* currentScenario READ currentScenarioObject NOTIFY currentScenarioChanged)
    Q_PROPERTY(QList<QObject*> currentScenarioModel READ currentScenarioModel NOTIFY currentScenarioChanged)
public:
    AudioCategory(const QString& name, const QString& path, QList<AudioScenario*> scenarios, QObject *parent = nullptr);
    AudioCategory(QJsonObject object, const QString& path, QObject *parent = nullptr);

    QJsonObject toJson() const;

    QString name() const { return m_name; }
    void setName(QString name) { m_name = name; emit nameChanged(); }

    bool isMarkedForExport() const { return m_export; }
    void setIsMarkedForExport(bool e) { m_export = e; emit isMarkedForExportChanged(); }

    QString path() const { return m_path; }

    QList<AudioScenario*> scenarios() const { return m_scenarios; }
    QList<QObject*> scenarioObjects() const { return Utils::toQObjectList<AudioScenario*>(m_scenarios); }
    int scenarioIndex() const { return m_scenarios.indexOf(m_currentScenario); }
    QStringList scenarioNames() const;
    void refreshElements() { if (m_currentScenario) m_currentScenario->refreshElements(); }

    Q_INVOKABLE bool setCurrentScenario(const QString& name);
    bool setCurrentScenario(AudioScenario *scenario);
    AudioScenario* currentScenario() const { return m_currentScenario; }
    QObject* currentScenarioObject() const { return qobject_cast<QObject*>(currentScenario()); }
    QList<QObject*> currentScenarioModel() const;

    bool addScenario(AudioScenario *scenario);
    bool deleteScenario(AudioScenario *scenario);

    QList<AudioElement *> elements() const;

signals:
    void nameChanged();
    void isMarkedForExportChanged();
    void pathChanged();
    void scenariosChanged();
    void currentScenarioChanged();

private:
    QString m_name, m_path;
    bool m_export = true;
    QList<AudioScenario*> m_scenarios;
    AudioScenario *m_currentScenario = nullptr;
};

#endif

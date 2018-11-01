#ifndef AUDIOEXPORTER_H
#define AUDIOEXPORTER_H

#include <QObject>
#include <QStringList>
#include "src/settings/settingsmanager.h"

class AudioExporter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(QStringList scenarios READ scenarios NOTIFY scenariosChanged)
    Q_PROPERTY(QStringList elements READ elements NOTIFY elementsChanged)

    Q_PROPERTY(QString project READ project WRITE setProject)

public:
    explicit AudioExporter(QObject *parent = nullptr);

    QStringList categories() const { return m_categories; }
    QStringList scenarios() const { return m_scenarios; }
    QStringList elements() const { return m_elements;}

    QString project() const { return m_project; }
    void setProject(QString project) { m_project = project; updateCategories(); }
    Q_INVOKABLE void setCategory(int index, bool enabled = true) { m_category = m_categories[index]; updateScenarios(enabled); }
    Q_INVOKABLE void setScenario(int index, bool enabled = true) { m_scenario = m_scenarios[index]; updateElements(enabled); }

    Q_INVOKABLE void setCategoryEnabled(int index, bool enabled = true) { m_exportCategories[index] = enabled; }
    Q_INVOKABLE void setScenarioEnabled(int index, bool enabled = true) { m_exportScenarios[m_categories.indexOf(m_category)][index] = enabled; }
    Q_INVOKABLE void setElementEnabled(int index, bool enabled = true) { m_exportElements[m_categories.indexOf(m_category)][m_scenarios.indexOf(m_scenario)][index] = enabled; }

    Q_INVOKABLE bool isCategoryEnabled(int index) const { return m_exportCategories[index]; }
    Q_INVOKABLE bool isScenarioEnabled(int index) const { return m_exportScenarios[m_categories.indexOf(m_category)][index]; }
    Q_INVOKABLE bool isElementEnabled(int index) const { return m_exportElements[m_categories.indexOf(m_category)][m_scenarios.indexOf(m_scenario)][index]; }

    Q_INVOKABLE QString getDefaultPath();
    Q_INVOKABLE void setPath(QString path) { m_path = path; }
    Q_INVOKABLE void exportFiles();

signals:
    void categoriesChanged();
    void scenariosChanged();
    void elementsChanged();

private:
    SettingsManager sManager;

    QStringList m_categories;
    QStringList m_scenarios;
    QStringList m_elements;

    QString m_project;
    QString m_category;
    QString m_scenario;
    QString m_path = "";

    QList<bool> m_exportCategories;
    QList<QList<bool>> m_exportScenarios;
    QList<QList<QList<bool>>> m_exportElements;

    void updateCategories();
    void updateScenarios(bool enabled = true);
    void updateElements(bool enabled = true);

};

#endif // AUDIOEXPORTER_H

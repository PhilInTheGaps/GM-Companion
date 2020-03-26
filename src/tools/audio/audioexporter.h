#ifndef AUDIOEXPORTER_H
#define AUDIOEXPORTER_H

#include <QObject>
#include <QStringList>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

#include "audioproject.h"
#include "audioelement.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QString path, AudioProject* project):
        m_path(path), m_project(project) {}
    virtual ~Worker() {}

private:
    QString m_path;
    AudioProject* m_project;

    QStringList musicFiles;
    QStringList soundFiles;
    QStringList radioFiles;

    void copyElements(AudioScenario* scenario);

public slots:
    void copyFiles();

signals:
    void copiedFiles();
    void progressChanged(float progress);
};

class AudioExporter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(QStringList scenarios READ scenarios NOTIFY scenariosChanged)
    Q_PROPERTY(QStringList elements READ elements NOTIFY elementsChanged)

    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)

    QThread workerThread;

public:
    explicit AudioExporter(QObject *parent = nullptr);
    ~AudioExporter() {
        workerThread.quit();
        workerThread.wait();
    }

    QStringList categories() const { return m_categoryNames; }
    QStringList scenarios() const { return m_scenarioNames; }
    QStringList elements() const { return m_elementNames; }

    void setProject(AudioProject *project) { m_project = project; updateCategories(); }

    Q_INVOKABLE void setCategory(int index);
    Q_INVOKABLE void setScenario(int index);

    Q_INVOKABLE void setCategoryEnabled(int index, bool enabled = true);
    Q_INVOKABLE void setScenarioEnabled(int index, bool enabled = true);
    Q_INVOKABLE void setElementEnabled(int index, bool enabled = true);

    Q_INVOKABLE bool isCategoryEnabled(int index) const;
    Q_INVOKABLE bool isScenarioEnabled(int index) const;
    Q_INVOKABLE bool isElementEnabled(int index) const;

    Q_INVOKABLE QString getDefaultPath() const { return QString(QDir::homePath() + "/.gm-companion/export"); }
    Q_INVOKABLE void setPath(QString path) { m_path = path; }
    Q_INVOKABLE void exportFiles();

    float progress() const { return m_progress; }

signals:
    void categoriesChanged();
    void scenariosChanged();
    void elementsChanged();
    void progressChanged();
    void startCopying();

private:
    AudioProject* m_project = nullptr;
    AudioCategory* m_category = nullptr;
    AudioScenario* m_scenario = nullptr;

    QStringList m_categoryNames;
    QStringList m_scenarioNames;
    QStringList m_elementNames;

    float m_progress = 0;
    QString m_path = "";

    void updateCategories();
    void updateScenarios();
    void updateElements();

public slots:
    void updateProgress(float progress) { m_progress = progress; if (m_progress >= 1) m_progress = 0; emit progressChanged(); }
};

#endif // AUDIOEXPORTER_H

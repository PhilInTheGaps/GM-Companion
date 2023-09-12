#pragma once

#include "../project/audioproject.h"
#include <QObject>
#include <QPointer>
#include <QQueue>
#include <QThread>
#include <QtQml/qqmlregistration.h>

using namespace Qt::Literals::StringLiterals;

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(const QString &path, AudioProject *project);

public slots:
    void startCopying();

signals:
    void copiedFiles();
    void progressChanged(float progress);

private:
    QString m_path;
    QPointer<AudioProject> m_project;

    QQueue<QString> m_musicFiles;
    QQueue<QString> m_soundFiles;
    QQueue<QString> m_radioFiles;

    int m_fileCount = 0;
    int m_exportCount = 0;

    void copyElements(AudioScenario *scenario);
    void collectFilesToExport();

    auto copyNext() -> bool;
    auto copyNextMusic() -> bool;
    auto copyNextSound() -> bool;
    auto copyNextRadio() -> bool;
    auto copyFile(const QString &filePath, const QString &base, const QString &subfolder) -> bool;
};

class AudioExporter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(AudioProject *model READ model NOTIFY modelChanged)
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)

    QThread workerThread;

public:
    using QObject::QObject;
    ~AudioExporter() override
    {
        workerThread.quit();
        workerThread.wait();
    }
    Q_DISABLE_COPY_MOVE(AudioExporter)

    [[nodiscard]] auto model() const -> AudioProject *
    {
        return m_model;
    }
    void setProject(AudioProject *project);

    Q_INVOKABLE void setPath(const QString &path)
    {
        m_path = path;
    }
    Q_INVOKABLE void exportFiles();

    [[nodiscard]] auto progress() const -> float
    {
        return m_progress;
    }

signals:
    void modelChanged();
    void progressChanged();
    void startCopying();

private:
    AudioProject *m_project = nullptr;
    QPointer<AudioProject> m_model = nullptr;

    float m_progress = 0;
    QString m_path = u""_s;

public slots:
    void updateProgress(float progress);
};

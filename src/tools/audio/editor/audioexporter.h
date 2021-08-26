#ifndef AUDIOEXPORTER_H
#define AUDIOEXPORTER_H

#include <QObject>
#include <QQueue>
#include <QThread>
#include <QPointer>

#include "../project/audioproject.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(const QString &path, AudioProject* project);

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

    void copyElements(AudioScenario* scenario);
    void collectFilesToExport();

    bool copyNext();
    bool copyNextMusic();
    bool copyNextSound();
    bool copyNextRadio();
    bool copyFile(const QString &filePath, const QString &base, const QString &subfolder);
};

class AudioExporter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)

    QThread workerThread;

public:
    explicit AudioExporter(QObject *parent = nullptr) : QObject(parent) {}
    ~AudioExporter() {
        workerThread.quit();
        workerThread.wait();
    }

    QObject* model() const { return m_model; }
    void setProject(AudioProject *project);

    Q_INVOKABLE void setPath(QString path) { m_path = path; }
    Q_INVOKABLE void exportFiles();

    float progress() const { return m_progress; }

signals:
    void modelChanged();
    void progressChanged();
    void startCopying();

private:
    AudioProject* m_project = nullptr;
    QPointer<QObject> m_model = nullptr;

    float m_progress = 0;
    QString m_path = "";

public slots:
    void updateProgress(float progress);
};

#endif // AUDIOEXPORTER_H

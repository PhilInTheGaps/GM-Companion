#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPointer>

#include "common/abstracttool.h"
#include "settings/settingsmanager.h"
#include "addonelementmanager.h"
#include "audioexporter.h"
#include "audioeditorfilebrowser.h"
#include "unsplash/unsplashparser.h"
#include "../audiosaveload.h"
#include "../project/audiofilemodel.h"

#include "thirdparty/propertyhelper/PropertyHelper.h"

class AudioEditor : public AbstractTool
{
    Q_OBJECT
    Q_PROPERTY(QList<AudioProject*> projects READ projects NOTIFY projectsChanged)
    Q_PROPERTY(QObject* currentProject READ currentProject NOTIFY currentProjectChanged)
    Q_PROPERTY(int projectIndex READ projectIndex NOTIFY currentProjectChanged)
    Q_PROPERTY(QObject* currentElement READ currentElement NOTIFY currentElementChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)

    AUTO_PROPERTY(bool, isSaved)

public:
    explicit AudioEditor(QQmlApplicationEngine *engine, QNetworkAccessManager *networkManager, QObject *parent);

    // Project
    Q_INVOKABLE void setCurrentProject(int index);
    Q_INVOKABLE void createProject(const QString &name);
    Q_INVOKABLE void renameProject(const QString &name);
    Q_INVOKABLE void deleteProject();
    Q_INVOKABLE void saveProject();
    QList<AudioProject*> projects() const { return m_projects; }
    QObject* currentProject() const { return qobject_cast<QObject*>(m_currentProject); }
    void setCurrentProject(AudioProject* project);
    int projectIndex() const;

    // Categories
    Q_INVOKABLE void setCurrentCategory(int index);
    Q_INVOKABLE void setCurrentCategory(AudioCategory *category);
    Q_INVOKABLE void createCategory(const QString &name);
    Q_INVOKABLE void renameCategory(const QString &name);
    Q_INVOKABLE void deleteCategory();

    // Scenarios
    Q_INVOKABLE void setCurrentScenario(int index);
    Q_INVOKABLE void setCurrentScenario(AudioScenario *scenario);
    Q_INVOKABLE void createScenario(const QString &name, bool isSubscenario = false);
    Q_INVOKABLE void renameScenario(const QString &name);
    Q_INVOKABLE void deleteScenario();
    Q_INVOKABLE void deleteSubScenario(AudioScenario *subscenario);
    Q_INVOKABLE void moveSubscenario(AudioScenario *subscenario, int steps);

    // Element positions
    Q_INVOKABLE void moveElement(AudioElement *element, int positions);
    Q_INVOKABLE void sortElements();
    Q_INVOKABLE void deleteCurrentElement();
    Q_INVOKABLE void setSubscenario(int index);

    // Create new element
    Q_INVOKABLE void createElement(const QString& name, AudioElement::Type type, int subscenario);

    // Edit element
    Q_INVOKABLE void removeFile(int index, bool findMissing = true);
    Q_INVOKABLE void moveFile(int index, int positions);
    Q_INVOKABLE void removeMissingFiles();
    Q_INVOKABLE void replaceFileFolder(int index, const QString &folder);
    Q_INVOKABLE bool addFile(QStringList path, const QString &filename);
    Q_INVOKABLE bool addUrl(const QString &url, int mode, const QString &title = "");
    Q_INVOKABLE bool addYtUrl(const QString &videoUrl);

    Q_INVOKABLE void findUnsplashImages(const QString &text) { unsplashParser->findImage(text); }
    Q_INVOKABLE void shuffleUnsplashImages() { unsplashParser->shuffle(); }

    // Elements
    QObject* currentElement() const { return qobject_cast<QObject*>(m_currentElement); }

    Q_INVOKABLE void loadElement(QObject* element);
    Q_INVOKABLE QString resourcesPath() const { return SettingsManager::getPath("resources"); }
    Q_INVOKABLE QString basePath() const;

    Q_INVOKABLE void setFileIndex(int index) { m_fileIndex = index; }

    bool isLoading() const { return m_isLoading; }

public slots:
    void loadData() override;

signals:
    void projectsChanged();
    void currentProjectChanged();
    void currentCategoryChanged();
    void currentScenarioChanged();
    void isSavedChanged();
    void isLoadingChanged();

    void currentElementChanged();
    void fileIndexChanged(int index);
    void showInfoBar(const QString& message);

private:
    AddonElementManager *addonElementManager = nullptr;
    AudioExporter *audioExporter = nullptr;
    AudioEditorFileBrowser *fileBrowser = nullptr;
    QQmlApplicationEngine *qmlEngine = nullptr;
    UnsplashParser *unsplashParser = nullptr;
    QNetworkAccessManager *networkManager = nullptr;

    QList<AudioProject*> m_projects;
    AudioProject *m_currentProject = nullptr;

    AudioFileModel *fileModel = nullptr;
    QPointer<AudioElement> m_currentElement;
    bool loadFirstElement(AudioScenario *scenario = nullptr);
    void clearCurrentElement();
    int m_fileIndex{};

    void madeChanges();
    bool addAudioFile(AudioFile *audioFile);

    void setIsLoading(bool isLoading) { m_isLoading = isLoading; emit isLoadingChanged(); }
    bool m_isLoading = true;

    // Helper function
    bool categoryExists() const;
    bool scenarioExists() const;

private slots:
    void addFiles(const QStringList &files);
    void onFoundProjects(const QVector<AudioProject *> &list);
    void onCurrentScenarioChanged();
    void onProjectSavedChanged();

};

#endif // AUDIOEDITOR_H

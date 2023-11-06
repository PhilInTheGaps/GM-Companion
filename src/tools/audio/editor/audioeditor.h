#pragma once

#include "../project/audiofilemodel.h"
#include "addonelementmanager.h"
#include "audioeditorfilebrowser.h"
#include "audioexporter.h"
#include "common/abstracttool.h"
#include "settings/settingsmanager.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include "unsplash/unsplashparser.h"
#include <QPointer>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class AudioEditor : public AbstractTool
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    READ_LIST_PROPERTY(AudioProject, projects)
    Q_PROPERTY(AudioProject *currentProject READ currentProject NOTIFY currentProjectChanged)
    Q_PROPERTY(int projectIndex READ projectIndex NOTIFY currentProjectChanged)
    Q_PROPERTY(AudioElement *currentElement READ currentElement NOTIFY currentElementChanged)

    Q_PROPERTY(AudioExporter *exporter READ exporter CONSTANT)
    Q_PROPERTY(AudioEditorFileBrowser *fileBrowser READ fileBrowser CONSTANT)
    Q_PROPERTY(UnsplashParser *unsplash READ unsplash CONSTANT)
    Q_PROPERTY(AddonElementManager *addons READ addons CONSTANT)
    Q_PROPERTY(AudioFileModel *files READ files CONSTANT)

    AUTO_PROPERTY_VAL2(bool, isSaved, true)

public:
    explicit AudioEditor(const QQmlEngine *engine, QObject *parent = nullptr);

    [[nodiscard]] auto exporter() -> AudioExporter *
    {
        return &m_audioExporter;
    }

    [[nodiscard]] auto fileBrowser() -> AudioEditorFileBrowser *
    {
        return &m_fileBrowser;
    }

    [[nodiscard]] auto unsplash() -> UnsplashParser *
    {
        return &m_unsplashParser;
    }

    [[nodiscard]] auto addons() -> AddonElementManager *
    {
        return &m_addonElementManager;
    }

    [[nodiscard]] auto files() -> AudioFileModel *
    {
        return &m_fileModel;
    }

    // Project
    Q_INVOKABLE void setCurrentProject(int index);
    Q_INVOKABLE void createProject(const QString &name);
    Q_INVOKABLE void createProjectFromTemplate(const AudioProject *other);
    Q_INVOKABLE void renameProject(const QString &name);
    Q_INVOKABLE void deleteProject();
    Q_INVOKABLE void saveProject();
    [[nodiscard]] auto currentProject() const -> AudioProject *
    {
        return m_currentProject;
    }
    void setCurrentProject(AudioProject *project);
    [[nodiscard]] auto projectIndex() const -> int;

    // Categories
    Q_INVOKABLE void setCurrentCategory(int index);
    Q_INVOKABLE void setCurrentCategory(AudioCategory *category);
    Q_INVOKABLE void createCategory(const QString &name);
    Q_INVOKABLE void createCategoryFromTemplate(const AudioCategory *other);
    Q_INVOKABLE void renameCategory(const QString &name);
    Q_INVOKABLE void deleteCategory();

    // Scenarios
    Q_INVOKABLE void setCurrentScenario(int index);
    Q_INVOKABLE void setCurrentScenario(AudioScenario *scenario);
    Q_INVOKABLE void createScenario(const QString &name, bool isSubscenario = false) const;
    Q_INVOKABLE void createScenarioFromTemplate(const AudioScenario *other, bool isSubscenario);
    Q_INVOKABLE void renameScenario(const QString &name) const;
    Q_INVOKABLE void deleteScenario();
    Q_INVOKABLE void deleteSubScenario(AudioScenario *subscenario);
    Q_INVOKABLE void moveSubscenario(AudioScenario *subscenario, int steps) const;

    // Element positions
    Q_INVOKABLE void moveElement(AudioElement *element, int positions) const;
    Q_INVOKABLE void sortElements() const;
    Q_INVOKABLE void deleteCurrentElement();
    Q_INVOKABLE void setSubscenario(int index) const;

    // Create new element
    Q_INVOKABLE void createElement(const QString &name, AudioElement::Type type, int subscenario) const;
    Q_INVOKABLE void createElementFromTemplate(const AudioElement *other, int subscenario) const;

    // Edit element
    Q_INVOKABLE void removeFile(int index, bool findMissing = true);
    Q_INVOKABLE void moveFile(int index, int positions);
    Q_INVOKABLE void removeMissingFiles();
    Q_INVOKABLE void replaceFileFolder(int index, const QString &folder) const;
    Q_INVOKABLE bool addFile(QStringList path, const QString &filename);
    Q_INVOKABLE bool addUrl(const QString &url, int mode, const QString &title = QLatin1String());
    Q_INVOKABLE bool addYtUrl(const QString &videoUrl);

    Q_INVOKABLE void findUnsplashImages(const QString &text)
    {
        m_unsplashParser.findImage(text);
    }
    Q_INVOKABLE void shuffleUnsplashImages()
    {
        m_unsplashParser.shuffle();
    }

    // Elements
    [[nodiscard]] auto currentElement() const -> AudioElement *
    {
        return m_currentElement;
    }

    Q_INVOKABLE void loadElement(QObject *element);
    [[nodiscard]] Q_INVOKABLE QString resourcesPath() const
    {
        return SettingsManager::getPath(QStringLiteral("resources"));
    }
    [[nodiscard]] Q_INVOKABLE QString basePath() const;

    Q_INVOKABLE void setFileIndex(int index)
    {
        m_fileIndex = index;
    }

public slots:
    void loadData() override;

signals:
    void currentProjectChanged();
    void currentCategoryChanged();
    void currentScenarioChanged();

    void currentElementChanged();
    void fileIndexChanged(int index);
    void showInfoBar(const QString &message);

private:
    AddonElementManager m_addonElementManager;
    AudioExporter m_audioExporter;
    AudioEditorFileBrowser m_fileBrowser;
    UnsplashParser m_unsplashParser;
    AudioFileModel m_fileModel;

    AudioProject *m_currentProject = nullptr;
    QPointer<AudioElement> m_currentElement;

    auto loadFirstElement(const AudioScenario *scenario = nullptr) -> bool;
    void clearCurrentElement();
    int m_fileIndex{};

    void madeChanges();
    auto addAudioFile(AudioFile *audioFile) -> bool;

    // Helper functions
    [[nodiscard]] auto categoryExists() const -> bool;
    [[nodiscard]] auto scenarioExists() const -> bool;

private slots:
    void addFiles(const QStringList &files);
    void onFoundProjects(const std::vector<AudioProject *> &projects);
    void onCurrentScenarioChanged() const;
    void onProjectSavedChanged();
};

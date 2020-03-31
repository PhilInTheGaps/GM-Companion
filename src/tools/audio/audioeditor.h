#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "settings/settingsmanager.h"
#include "audiosaveload.h"
#include "addonelementmanager.h"
#include "audioexporter.h"
#include "audioeditorfilebrowser.h"
#include "unsplash/unsplashparser.h"
#include "../lib/qytlib/youtube.h"

class AudioEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList projectNames READ projectNames NOTIFY projectsChanged)
    Q_PROPERTY(QStringList categoryNames READ categoryNames  NOTIFY currentProjectChanged)
    Q_PROPERTY(QStringList scenarioNames READ scenarioNames NOTIFY currentCategoryChanged)
    Q_PROPERTY(QStringList subscenarioNames READ subscenarioNames NOTIFY subscenariosChanged)
    Q_PROPERTY(bool isSaved READ isSaved NOTIFY isSavedChanged)

    Q_PROPERTY(int projectIndex READ projectIndex NOTIFY currentProjectChanged)
    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY currentCategoryChanged)
    Q_PROPERTY(int scenarioIndex READ scenarioIndex NOTIFY currentScenarioChanged)

    Q_PROPERTY(QString name READ name NOTIFY currentElementChanged)
    Q_PROPERTY(int type READ type NOTIFY currentElementChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY currentElementChanged)
    Q_PROPERTY(int mode READ mode NOTIFY currentElementChanged)
    Q_PROPERTY(QString id READ id NOTIFY currentElementChanged)
    Q_PROPERTY(int subscenario READ subscenario NOTIFY currentElementChanged)

public:
    explicit AudioEditor(QQmlApplicationEngine *engine, AudioSaveLoad *audioSaveLoad, QObject *parent);

    AddonElementManager* getAddonElementManager() const { return addonElementManager; }
    AudioExporter* getAudioExporter() const { return audioExporter; }
    AudioEditorFileBrowser* getFileBrowser() const { return fileBrowser; }

    // Project
    Q_INVOKABLE void setCurrentProject(int index);
    Q_INVOKABLE void createProject(QString name);
    Q_INVOKABLE void renameProject(QString name);
    Q_INVOKABLE void deleteProject();
    Q_INVOKABLE void saveProject();
    void setCurrentProject(AudioProject* project);
    QStringList projectNames();
    int projectIndex() const;
    bool isSaved() const { return m_isSaved; }

    // Categories
    Q_INVOKABLE void setCurrentCategory(QString name);
    Q_INVOKABLE void createCategory(QString name);
    Q_INVOKABLE void renameCategory(QString name);
    Q_INVOKABLE void deleteCategory();
    QStringList categoryNames() const { if (!m_currentProject) return {}; else return m_currentProject->categoryNames(); }
    int categoryIndex() const;

    // Scenarios
    Q_INVOKABLE void setCurrentScenario(QString name);
    Q_INVOKABLE void createScenario(QString name, bool subscenario = false);
    Q_INVOKABLE void renameScenario(QString name);
    Q_INVOKABLE void deleteScenario();
    Q_INVOKABLE void deleteSubScenario(QString name);
    QStringList scenarioNames() const;
    int scenarioIndex() const;
    QStringList subscenarioNames() const;

    // Element positions
    Q_INVOKABLE void moveElement(QString element, int type, int positions);
    Q_INVOKABLE void sortElements();
    Q_INVOKABLE void deleteElement(QString element, int type);
    Q_INVOKABLE void setSubscenario(QString element, int type, int index);
    Q_INVOKABLE int getSubscenarioIndex(QString scenario);

    // Create new element
    Q_INVOKABLE void createMusicElement(QString name, bool subscenario, int scenarioIndex);
    Q_INVOKABLE void createSoundElement(QString name, bool subscenario, int scenarioIndex);
    Q_INVOKABLE void createRadioElement(QString name, bool subscenario, int scenarioIndex);

    // Edit element
    Q_INVOKABLE void setName(QString element, int type, QString name);
    Q_INVOKABLE void addFile(QString element, int type, QStringList path, QString filename);
    Q_INVOKABLE void removeFile(QString name, int type, int index, bool findMissing = true);
    Q_INVOKABLE void moveFile(QString name, int type, int index, int positions);
    Q_INVOKABLE void removeMissingFiles(QString name, int type);
    Q_INVOKABLE void replaceFileFolder(QString name, int type, int index, QString folder);
    Q_INVOKABLE void setMode(QString name, int type, int mode);
    Q_INVOKABLE void addUrl(QString name, int type, QString url, int mode, QString title = "");
    Q_INVOKABLE void addYtUrl(QString elementName, int type, QString videoUrl);

    Q_INVOKABLE void findUnsplashImages(QString text) { unsplashParser->findImage(text); }
    Q_INVOKABLE void shuffleUnsplashImages() { unsplashParser->shuffle(); }

    // Elements
    QString name() const { return m_name; }
    QString icon() const { return m_icon; }
    Q_INVOKABLE void setIcon(QString element, int type, QString path);
    int type() const { return m_type; }
    int mode() const { return m_mode; }
    QString id() const { return m_id; }
    int subscenario() { return getSubscenarioIndex(m_subscenario); }
    Q_INVOKABLE void loadElement(QString name, int type, QString subscenario);
    Q_INVOKABLE QString resourcesPath() const { return SettingsManager::getPath("resourcesPath"); }
    Q_INVOKABLE QString basePath(int type);

    Q_INVOKABLE void setFileIndex(int index) { m_fileIndex = index; }

signals:
    void projectsChanged();
    void currentProjectChanged();
    void currentCategoryChanged();
    void currentScenarioChanged();
    void subscenariosChanged();
    void isSavedChanged();

    void currentElementChanged();
    void fileIndexChanged(int index);
    void showInfoBar(QString message);

private:
    SettingsManager *sManager = nullptr;
    AddonElementManager *addonElementManager = nullptr;
    AudioExporter *audioExporter = nullptr;
    AudioEditorFileBrowser *fileBrowser = nullptr;
    QQmlApplicationEngine *qmlEngine = nullptr;
    UnsplashParser *unsplashParser = nullptr;
    QNetworkAccessManager m_networkManager;
    YouTube m_youtube;
    AudioSaveLoad *m_audioSaveLoad = nullptr;

    QList<AudioProject*> m_projects;
    AudioProject *m_currentProject = nullptr;
    bool m_isSaved = true;

    AudioElementModelModel *elementModel = nullptr;
    AudioFileModel *fileModel = nullptr;

    // Elements
    QString m_name, m_subscenario, m_icon, m_id;
    int m_mode, m_type;
    void clearCurrentElement();

    AudioElement* getElement(QString name, int type);
    int m_fileIndex;

    void updateElementModel();
    void madeChanges();

private slots:
    void addFiles(QStringList files);
    void onFoundProjects(QList<AudioProject *>list, bool isEditor);
    void onCurrentScenarioChanged();
    void onExportElements(QList<AudioElement *>elements, bool subscenario, int scenarioIndex);

};

#endif // AUDIOEDITOR_H

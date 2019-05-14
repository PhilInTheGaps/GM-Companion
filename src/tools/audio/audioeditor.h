#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QStringList>
#include "src/settings/settingsmanager.h"
#include "addonelementmanager.h"
#include "audioexporter.h"
#include "audioeditorfilebrowser.h"
#include "src/managers/filemanager.h"
#include "unsplash/unsplashparser.h"

class AudioEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList projectNames READ projectNames NOTIFY projectsChanged)
    Q_PROPERTY(QStringList categoryNames READ categoryNames  NOTIFY currentProjectChanged)
    Q_PROPERTY(QStringList scenarioNames READ scenarioNames NOTIFY currentCategoryChanged)
    Q_PROPERTY(bool isSaved READ isSaved NOTIFY isSavedChanged)

    Q_PROPERTY(int projectIndex READ projectIndex NOTIFY currentProjectChanged)
    Q_PROPERTY(int categoryIndex READ categoryIndex NOTIFY currentCategoryChanged)
    Q_PROPERTY(int scenarioIndex READ scenarioIndex NOTIFY currentScenarioChanged)

    Q_PROPERTY(QString name READ name NOTIFY currentElementChanged)
    Q_PROPERTY(int type READ type NOTIFY currentElementChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY currentElementChanged)
    Q_PROPERTY(QStringList files READ files NOTIFY currentElementChanged)
    Q_PROPERTY(int mode READ mode NOTIFY currentElementChanged)
    Q_PROPERTY(QString url READ url NOTIFY currentElementChanged)
    Q_PROPERTY(bool local READ local NOTIFY currentElementChanged)
    Q_PROPERTY(QString id READ id NOTIFY currentElementChanged)
    Q_PROPERTY(QList<bool> missing READ missing NOTIFY missingChanged)

public:
    explicit AudioEditor(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent);

    AddonElementManager* getAddonElementManager() const { return addonElementManager; }
    AudioExporter* getAudioExporter() const { return audioExporter; }
    AudioEditorFileBrowser* getFileBrowser() const { return fileBrowser; }

    // Project
    Q_INVOKABLE void setCurrentProject(int index);
    Q_INVOKABLE void createProject(QString name);
    Q_INVOKABLE void renameProject(QString name);
    Q_INVOKABLE void saveProject();
    void setCurrentProject(AudioProject* project);
    QStringList projectNames();
    int projectIndex() const;
    bool isSaved() const { return m_isSaved; }

    // Categories
    Q_INVOKABLE void setCurrentCategory(QString name);
    Q_INVOKABLE void createCategory(QString name);
    Q_INVOKABLE void renameCategory(QString name);
    QStringList categoryNames() const { if (!m_currentProject) return {}; else return m_currentProject->categoryNames(); }
    int categoryIndex() const;

    // Scenarios
    Q_INVOKABLE void setCurrentScenario(QString name);
    Q_INVOKABLE void createScenario(QString name);
    Q_INVOKABLE void renameScenario(QString name);
    QStringList scenarioNames() const;
    int scenarioIndex() const;

    // Element positions
    Q_INVOKABLE void moveElement(QString element, int type, int positions);
    Q_INVOKABLE void sortElements();
    Q_INVOKABLE void deleteElement(QString element, int type);

    // Create new element
    Q_INVOKABLE void createMusicElement(QString name);
    Q_INVOKABLE void createSoundElement(QString name);
    Q_INVOKABLE void createRadioElement(QString name);
    Q_INVOKABLE void createSpotifyElement(QString name);

    // Edit element
    Q_INVOKABLE void setName(QString element, int type, QString name);
    Q_INVOKABLE void addFile(QString element, int type, QString path);
    Q_INVOKABLE void removeFile(QString element, int type, int index, bool findMissing = true);
    Q_INVOKABLE void moveFile(QString element, int type, int index, int positions);
    Q_INVOKABLE void removeMissingFiles(QString element, int type);
    Q_INVOKABLE void replaceFileFolder(QString element, int type, int index, QString folder);
    Q_INVOKABLE void setMode(QString element, int type, int mode);
    Q_INVOKABLE void setUrl(QString element, int type, QString url);
    Q_INVOKABLE void setLocal(QString element, int type, bool local);
    Q_INVOKABLE void setId(QString element, int type, QString id);

    Q_INVOKABLE void findUnsplashImages(QString text) { unsplashParser->findImage(text); }
    Q_INVOKABLE void shuffleUnsplashImages() { unsplashParser->shuffle(); }

    // Elements
    QString name() const { return m_name; }
    QString icon() const { return m_icon; }
    Q_INVOKABLE void setIcon(QString element, int type, QString path);
    QStringList files() const { return m_files; }
    int type() const { return m_type; }
    int mode() const { return m_mode; }
    bool local() const { return m_local; }
    QString url() const { return m_url; }
    QString id() const { return m_id; }
    QList<bool> missing() const { return m_missing; }
    Q_INVOKABLE void loadElement(QString name, int type);
    Q_INVOKABLE QString resourcesPath() const { return sManager->getSetting(Setting::resourcesPath); }
    Q_INVOKABLE QString basePath(int type);

    Q_INVOKABLE void setFileIndex(int index) { m_fileIndex = index; }

signals:
    void projectsChanged();
    void currentProjectChanged();
    void currentCategoryChanged();
    void currentScenarioChanged();
    void isSavedChanged();
    void missingChanged();

    void currentElementChanged();
    void fileIndexChanged(int index);
    void showInfoBar(QString message);

private:
    SettingsManager *sManager = nullptr;
    AddonElementManager *addonElementManager = nullptr;
    AudioExporter *audioExporter = nullptr;
    AudioEditorFileBrowser *fileBrowser = nullptr;
    FileManager *fileManager = nullptr;
    QQmlApplicationEngine *qmlEngine = nullptr;
    UnsplashParser *unsplashParser = nullptr;

    QList<AudioProject*> m_projects;
    AudioProject *m_currentProject = nullptr;
    bool m_isSaved = true;

    AudioElementModel *elementModel = nullptr;

    // Elements
    QString m_name, m_icon, m_url, m_id;
    QStringList m_files;
    int m_mode, m_type;
    QList<bool> m_missing;
    bool m_local;

    AudioElement* getElement(QString name, int type);
    int m_fileIndex;

    void updateElementModel();
    void madeChanges();

private slots:
    void foundMissingFiles(QList<bool> missing);
    void addFiles(QStringList files);

};

#endif // AUDIOEDITOR_H

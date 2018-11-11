#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QObject>
#include <QStringList>
#include "src/settings/settingsmanager.h"

class AudioEditor : public QObject
{
    Q_OBJECT
public:
    explicit AudioEditor(QObject *parent = nullptr);

    Q_INVOKABLE void updateProjectList();
    Q_INVOKABLE QStringList getProjectList() const { return m_projectList; }
    Q_INVOKABLE void setCurrentProject(QString project);
    Q_INVOKABLE void createProject(QString project);

    Q_INVOKABLE void updateCategoryList();
    Q_INVOKABLE QStringList getCategoryList() const { return m_categoryList; }
    Q_INVOKABLE void setCurrentCategory(QString category);
    Q_INVOKABLE void createCategory(QString category);

    Q_INVOKABLE void updateScenarioList();
    Q_INVOKABLE QStringList getScenarioList() const { return m_scenarioList; }
    Q_INVOKABLE void setCurrentScenario(QString scenario);
    Q_INVOKABLE void createScenario(QString scenario);

    Q_INVOKABLE void updateElementList();
    Q_INVOKABLE void moveElement(QString element, int type, int positions);
    Q_INVOKABLE void sortElements();

    Q_INVOKABLE void createList(QString listName, int type);
    Q_INVOKABLE QStringList getMusicLists() const { return m_musicLists; }
    Q_INVOKABLE QStringList getSoundLists() const { return m_soundLists; }
    Q_INVOKABLE QStringList getRadios() const { return m_radios; }
    Q_INVOKABLE QStringList getSpotifyPlaylists() const { return m_spotifyPlaylists; }

    Q_INVOKABLE void saveProject();
    Q_INVOKABLE QString getResourcesPath();

    Q_INVOKABLE void setCurrentList(QString list, int type);
    Q_INVOKABLE QStringList getCurrentFileNames() const { return m_currentFileNames; }
    Q_INVOKABLE QStringList getCurrentFilePaths() const { return m_currentFilePaths; }
    Q_INVOKABLE QList<bool> getCurrentFileMissing() const { return m_currentFileMissing; }
    Q_INVOKABLE QString getCurrentBasePath() const { return m_currentBasePath; }
    Q_INVOKABLE QString getCurrentListIcon() const { return m_currentListIcon; }
    Q_INVOKABLE int getCurrentListMode() const { return m_currentListMode; }
    Q_INVOKABLE void addFile(QString name, QString path);
    Q_INVOKABLE void addFiles(QStringList names, QStringList paths, QList<bool> missing);
    Q_INVOKABLE void removeFile(int index);
    Q_INVOKABLE void saveList(int type);
    Q_INVOKABLE void deleteList(QString list, int type);
    Q_INVOKABLE void moveFile(int index, int positions);
    Q_INVOKABLE void setCurrentListMode(int mode) { m_currentListMode = mode; }
    Q_INVOKABLE void setCurrentListIcon(QString icon) { m_currentListIcon = icon; }
    Q_INVOKABLE void removeMissingFiles();
    Q_INVOKABLE void replaceMissingFolder(int index, QString folder, int type);
    Q_INVOKABLE int getListIndex() const { return m_lastListIndex > m_currentFileNames.size() ? 0 : m_lastListIndex; }

    Q_INVOKABLE bool getLocal() const { return m_local; }
    Q_INVOKABLE QString getURL() const { return m_url; }
    Q_INVOKABLE void setURL(QString url);
    Q_INVOKABLE void setLocal(bool local) { m_local = local; }

    Q_INVOKABLE QString getSpotifyID() const { return m_spotifyID; }
    Q_INVOKABLE void setSpotifyID(QString id) { m_spotifyID = id; }

signals:
    void projectListChanged();
    void categoryListChanged();
    void scenarioListChanged();
    void elementListChanged();

    void listChanged();
    void urlChanged();
    void spotifyIDChanged();

private:
    SettingsManager *sManager;

    QStringList m_projectList;
    QString m_currentProject;

    QStringList m_categoryList;
    QString m_currentCategory;

    QStringList m_scenarioList;
    QString m_currentScenario;

    QStringList m_musicLists;
    QStringList m_soundLists;
    QStringList m_radios;
    QStringList m_spotifyPlaylists;

    QString m_currentElement;

    QString m_currentList;
    QStringList m_currentFileNames;
    QStringList m_currentFilePaths;
    QList<bool> m_currentFileMissing;
    int m_currentListMode;
    QString m_currentListIcon;
    QString m_currentBasePath;
    int m_lastListIndex = 0;

    bool m_local;
    QString m_url;

    QString m_spotifyID;
};

#endif // AUDIOEDITOR_H

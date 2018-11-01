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
    Q_INVOKABLE QStringList getProjectList();
    Q_INVOKABLE void setCurrentProject(QString project);
    Q_INVOKABLE void createProject(QString project);

    Q_INVOKABLE void updateCategoryList();
    Q_INVOKABLE QStringList getCategoryList();
    Q_INVOKABLE void setCurrentCategory(QString category);
    Q_INVOKABLE void createCategory(QString category);

    Q_INVOKABLE void updateScenarioList();
    Q_INVOKABLE QStringList getScenarioList();
    Q_INVOKABLE void setCurrentScenario(QString scenario);
    Q_INVOKABLE void createScenario(QString scenario);

    Q_INVOKABLE void updateElementList();
    Q_INVOKABLE void moveElement(QString element, int type, int positions);
    Q_INVOKABLE void sortElements();

    Q_INVOKABLE void createList(QString listName, int type);
    Q_INVOKABLE QStringList getMusicLists();
    Q_INVOKABLE QStringList getSoundLists();
    Q_INVOKABLE QStringList getRadios();
    Q_INVOKABLE QStringList getSpotifyPlaylists() const { return m_spotifyPlaylists; }

    Q_INVOKABLE void saveProject();
    Q_INVOKABLE QString getResourcesPath();

    Q_INVOKABLE void setCurrentList(QString list, int type);
    Q_INVOKABLE QStringList getCurrentFileNames();
    Q_INVOKABLE QStringList getCurrentFilePaths();
    Q_INVOKABLE QList<bool> getCurrentFileMissing() const { return l_currentFileMissing; }
    Q_INVOKABLE QString getCurrentBasePath() const { return l_currentBasePath; }
    Q_INVOKABLE QString getCurrentListIcon();
    Q_INVOKABLE int getCurrentListMode();
    Q_INVOKABLE void addFile(QString name, QString path);
    Q_INVOKABLE void addFiles(QStringList names, QStringList paths, QList<bool> missing);
    Q_INVOKABLE void removeFile(int index);
    Q_INVOKABLE void saveList(int type);
    Q_INVOKABLE void deleteList(QString list, int type);
    Q_INVOKABLE void moveFile(int index, int positions);
    Q_INVOKABLE void setCurrentListMode(int mode);
    Q_INVOKABLE void setCurrentListIcon(QString icon);
    Q_INVOKABLE void removeMissingFiles();
    Q_INVOKABLE void replaceMissingFolder(int index, QString folder, int type);
    Q_INVOKABLE int getListIndex() const { return l_lastListIndex > l_currentFileNames.size() ? 0 : l_lastListIndex; }

    Q_INVOKABLE bool getLocal();
    Q_INVOKABLE QString getURL();
    Q_INVOKABLE void setURL(QString url);
    Q_INVOKABLE void setLocal(bool local);

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

    QStringList l_projectList;
    QString l_currentProject;

    QStringList l_categoryList;
    QString l_currentCategory;

    QStringList l_scenarioList;
    QString l_currentScenario;

    QStringList l_musicLists;
    QStringList l_soundLists;
    QStringList l_radios;
    QStringList m_spotifyPlaylists;

    QString l_currentElement;

    QString l_currentList;
    QStringList l_currentFileNames;
    QStringList l_currentFilePaths;
    QList<bool> l_currentFileMissing;
    int l_currentListMode;
    QString l_currentListIcon;
    QString l_currentBasePath;
    int l_lastListIndex = 0;

    bool l_local;
    QString l_url;

    QString m_spotifyID;
};

#endif // AUDIOEDITOR_H

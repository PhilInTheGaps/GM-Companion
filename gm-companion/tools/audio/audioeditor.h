#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QObject>
#include <QStringList>
#include "gm-companion/settings/settingsmanager.h"

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

    Q_INVOKABLE void createList(QString listName, int type);
    Q_INVOKABLE QStringList getMusicLists();
    Q_INVOKABLE QStringList getSoundLists();
    Q_INVOKABLE QStringList getRadios();

    Q_INVOKABLE void saveProject();
    Q_INVOKABLE QString getResourcesPath();

    Q_INVOKABLE void setCurrentList(QString list, int type);
    Q_INVOKABLE QStringList getCurrentFileNames();
    Q_INVOKABLE QStringList getCurrentFilePaths();
    Q_INVOKABLE QString getCurrentListIcon();
    Q_INVOKABLE int getCurrentListMode();
    Q_INVOKABLE void addFile(QString name, QString path);
    Q_INVOKABLE void addFiles(QStringList names, QStringList paths);
    Q_INVOKABLE void removeFile(int index);
    Q_INVOKABLE void saveList(int type);
    Q_INVOKABLE void deleteList(QString list, int type);
    Q_INVOKABLE void moveFile(int index, int positions);
    Q_INVOKABLE void setCurrentListMode(int mode);
    Q_INVOKABLE void setCurrentListIcon(QString icon);
    Q_INVOKABLE void removeMissingFiles(int type);

    Q_INVOKABLE bool getLocal();
    Q_INVOKABLE QString getURL();
    Q_INVOKABLE void setURL(QString url);
    Q_INVOKABLE void setLocal(bool local);

signals:
    void projectListChanged();
    void categoryListChanged();
    void scenarioListChanged();
    void elementListChanged();

    void listChanged();
    void urlChanged();

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

    QString l_currentElement;

    QString l_currentList;
    QStringList l_currentFileNames;
    QStringList l_currentFilePaths;
    int l_currentListMode;
    QString l_currentListIcon;

    bool l_local;
    QString l_url;
};

#endif // AUDIOEDITOR_H

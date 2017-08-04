#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QWidget>
#include <QTreeWidget>
#include <QListWidget>

#include "managers/settingsmanager.h"

namespace Ui {
class AudioEditor;
}

class AudioEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AudioEditor(QWidget *parent = 0);
    ~AudioEditor();

private slots:
    void on_treeWidget_music_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_newProject_clicked();
    void on_pushButton_openProject_clicked();
    void on_pushButton_newCategory_clicked();

    void on_treeWidget_categories_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_categories_currentItemChanged(QTreeWidgetItem *current);

    void on_pushButton_deleteSelected_clicked();

    void on_pushButton_newScenario_clicked();
    void on_pushButton_newMusicList_clicked();
    void on_pushButton_newSoundList_clicked();
    void on_pushButton_newRadio_clicked();

    void on_pushButton_saveElement_clicked();

    void on_listWidget_musicList_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_soundList_itemDoubleClicked(QListWidgetItem *item);
    void on_treeWidget_sound_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_radio_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_addAllFilesFromMusicFolder_clicked();

    void on_pushButton_addAllFilesFromSoundFolder_clicked();

    void on_pushButton_convertFolderToMusicList_clicked();

private:
    Ui::AudioEditor *ui;

    SettingsManager *settingsManager;

    bool isProjectOpen = false;

    bool filesAreLoaded = false;

    QString projectName;
    QString currentCategory;
    QString currentScenario;
    QString currentElement;

    void save();

    void getProjects();

    void loadProject();
    void loadCategories();
    void loadScenarios(QTreeWidgetItem *catItem);
    void loadMusicLists(QTreeWidgetItem *scenItem, QString category);
    void loadSoundLists(QTreeWidgetItem *scenItem, QString category);
    void loadRadios(QTreeWidgetItem *scenItem, QString category);

    void loadFolderContentsToTreeView(QTreeWidget *treeWidget, QString baseFolder);

    void addSubFoldersToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder);

    void addFilesToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder);
};

#endif // AUDIOEDITOR_H

#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QMediaPlayer>

#include "gm-companion/settings/settingsmanager.h"

//namespace Ui {
//class AudioEditor;
//}

class AudioEditor : public QObject
{
    Q_OBJECT

public:
    explicit AudioEditor(QObject *parent = 0);
    ~AudioEditor();

private slots:
//    void on_treeWidget_music_itemDoubleClicked(QTreeWidgetItem *item, int column);

//    void on_pushButton_newProject_clicked();
//    void on_pushButton_openProject_clicked();

//    void on_treeWidget_categories_itemDoubleClicked(QTreeWidgetItem *item, int column);
//    void on_treeWidget_categories_currentItemChanged(QTreeWidgetItem *current);

//    void on_pushButton_deleteSelected_clicked();

//    void on_listWidget_musicList_itemDoubleClicked(QListWidgetItem *item);
//    void on_listWidget_soundList_itemDoubleClicked(QListWidgetItem *item);
//    void on_treeWidget_sound_itemDoubleClicked(QTreeWidgetItem *item, int column);
//    void on_treeWidget_radio_itemDoubleClicked(QTreeWidgetItem *item, int column);

//    void on_pushButton_addAllFilesFromMusicFolder_clicked();
//    void on_pushButton_addAllFilesFromSoundFolder_clicked();
//    void on_pushButton_convertFolderToMusicList_clicked();

//    void on_pushButton_playPause_toggled(bool checked);

//    void on_treeWidget_music_currentItemChanged(QTreeWidgetItem *current);
//    void on_treeWidget_sound_currentItemChanged(QTreeWidgetItem *current);
//    void on_treeWidget_music_itemExpanded(QTreeWidgetItem *item);
//    void on_treeWidget_sound_itemExpanded(QTreeWidgetItem *item);

//    void on_verticalSlider_volume_valueChanged(int value);
//    void on_horizontalSlider_progress_sliderMoved(int position);
//    void previewPlayer_positionChanged(qint64 position);

//    void on_toolButton_musicIcon_clicked();
//    void on_toolButton_soundIcon_clicked();
//    void on_toolButton_radioIcon_clicked();

//    void on_pushButton_newElement_clicked();

//    void on_pushButton_detectMissingMusic_clicked();
//    void on_pushButton_detectMissingSounds_clicked();

//    void on_pushButton_addToMusicList_clicked();
//    void on_pushButton_addToSoundList_clicked();

//    void on_pushButton_removeSoundFile_clicked();
//    void on_pushButton_removeFileMusic_clicked();

//    void on_pushButton_up_clicked();
//    void on_pushButton_down_clicked();

//    void on_pushButton_save_clicked();
//    void on_pushButton_openProjectFolder_clicked();

//    void on_pushButton_saveCategory_clicked();

//    void on_pushButton_saveScenario_clicked();

//    void on_pushButton_saveMusicList_clicked();

//    void on_pushButton_saveSoundList_clicked();

//    void on_pushButton_saveRadio_clicked();

private:
//    Ui::AudioEditor *ui;

//    SettingsManager *settingsManager;

//    QMediaPlayer *previewPlayer;
//    void playPreview(QString path);

//    bool isProjectOpen = false;
//    bool filesAreLoaded = false;

//    QString projectName;
//    QString currentCategory;
//    QString currentScenario;
//    QString currentElement;

//    void save();
//    void saveCategoryOrder();
//    void saveScenarioOrder(QTreeWidgetItem *categoryItem);
//    void saveElement();
//    void saveElementOrder();

//    void getProjects();
//    void loadProject();
//    void loadCategories();
//    void loadScenarios(QTreeWidgetItem *catItem);
//    void loadElement(QTreeWidgetItem *scenItem, QString category, QString element, int type);
//    void loadElements(QTreeWidgetItem *scenItem, QString category);

//    void moveElement(QTreeWidgetItem *item, int steps);

//    void addFileToList(QTreeWidgetItem *item, int column, int type);
//    void addAllFilesFromFolder(QTreeWidgetItem *item, int type);

//    void addNewElement(int type);
//    void addNewCategory();
//    void addNewScenario();

//    void markMissingFiles(QList<int> indices, QListWidget *widget);
//    QList<int> getMissingFiles(int type);

//    // Icon File Browser
//    QString setIconPath(QString windowTitle);
//    void chooseIcon(int type);
//    QString lastPath;

//    // File Browser
//    void loadFolderContentsToTreeView(QTreeWidget *treeWidget, QString baseFolder);
//    void addSubFoldersToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder, bool topLevel = false, QTreeWidget *tree = NULL);
//    void addFilesToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder, bool topLevel = false, QTreeWidget *tree = NULL);
};

#endif // AUDIOEDITOR_H

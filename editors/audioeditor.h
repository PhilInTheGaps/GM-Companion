#ifndef AUDIOEDITOR_H
#define AUDIOEDITOR_H

#include <QWidget>
#include <QTreeWidget>

#include "managers/dbmanager.h"
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

    void on_pushButton_save_clicked();

    void on_pushButton_newCategory_clicked();

    void on_pushButton_openProject_clicked();

    void on_treeWidget_categories_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_deleteSelected_clicked();

    void on_pushButton_newScenario_clicked();

private:
    Ui::AudioEditor *ui;

    SettingsManager *settingsManager;

    bool isProjectOpen;

    QString projectName;
    QString currentCategory;
    QString currentScenario;
    QString currentElement;

    void save();

    void getProjects();

    void loadProject();

    void loadCategories();

    void loadScenarios(QTreeWidgetItem *catItem);

    void loadFolderContentsToTreeView(QTreeWidget *treeWidget, QString baseFolder);

    void addSubFoldersToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder);

    void addFilesToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder);
};

#endif // AUDIOEDITOR_H

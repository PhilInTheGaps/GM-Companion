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
    void on_pushButton_newCategory_clicked();

    void on_pushButton_newScenario_clicked();

    void on_treeWidget_music_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::AudioEditor *ui;

    SettingsManager *settingsManager;

    void loadFolderContentsToTreeView(QTreeWidget *treeWidget, QString baseFolder);

    void addSubFoldersToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder);

    void addFilesToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder);
};

#endif // AUDIOEDITOR_H

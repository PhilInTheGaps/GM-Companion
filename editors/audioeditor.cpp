#include "audioeditor.h"
#include "ui_audioeditor.h"
#include "functions.h"

#include <QDir>
#include <QStyle>
#include <QDebug>
#include <QSettings>

AudioEditor::AudioEditor(QWidget *parent) : QWidget(parent), ui(new Ui::AudioEditor)
{
    ui->setupUi(this);

    settingsManager = new SettingsManager;

    isProjectOpen = false;

    loadFolderContentsToTreeView(ui->treeWidget_music, settingsManager->getSetting(musicPath));
}

AudioEditor::~AudioEditor()
{
    delete ui;
}

// Adds a child item for every subfolder in the directory
void AudioEditor::addSubFoldersToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder)
{
    for (QString folder : getFolders(baseFolder))
    {
        if (!folder.contains("."))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(0);
            item->setText(0, folder);
            item->setToolTip(0, folder);
            item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));

            baseItem->addChild(item);

            // If this directory has even more subfolders, adds those as well
            addSubFoldersToTreeItem(item, baseFolder+"/"+folder);

            // Adds all file in this directory as well
            addFilesToTreeItem(item, baseFolder+"/"+folder);
        }
    }
}

// Adds a child item for every file inside the folder
void AudioEditor::addFilesToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder)
{
    for (QString file : getFiles(baseFolder))
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(1);
        item->setText(0, file);
        item->setToolTip(0, file);
        item->setIcon(0, style()->standardIcon(QStyle::SP_FileIcon));

        baseItem->addChild(item);
    }
}

// Load a folder structure to a tree view
void AudioEditor::loadFolderContentsToTreeView(QTreeWidget *treeWidget, QString baseFolder)
{
    // Making sure it is empty
    treeWidget->clear();

    // Adding top level items
    for (QString folder : getFolders(baseFolder))
    {
        if (!folder.contains("."))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(0);
            item->setText(0, folder);
            item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
            item->setToolTip(0, folder);

            treeWidget->addTopLevelItem(item);

            // Add subfolder items
            addSubFoldersToTreeItem(item, baseFolder+"/"+folder);

            // Add files
            addFilesToTreeItem(item, baseFolder+"/"+folder);
        }
    }
}

// When an item in the music explorer is double clicked
void AudioEditor::on_treeWidget_music_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->type() == 1)
        qDebug() << "File was double clicked!";
}

// Create a new project
void AudioEditor::on_pushButton_newProject_clicked()
{
    projectName = ui->lineEdit_project->text();

    ui->lineEdit_project->clear();

    save();
}

// Save project
void AudioEditor::save()
{
    if (projectName != NULL)
    {
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        settings.setValue("ProjectName", projectName);
    }
}

// Save Project
void AudioEditor::on_pushButton_save_clicked()
{
    save();
}

// Add a category
void AudioEditor::on_pushButton_newCategory_clicked()
{
    if (ui->lineEdit_category->text() != NULL && projectName != NULL)
    {
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray("Categories");
        settings.endArray();

        settings.beginWriteArray("Categories");



        settings.setArrayIndex(size);

        settings.setValue("name", ui->lineEdit_category->text());

        settings.endArray();
    }

    ui->lineEdit_category->clear();
}

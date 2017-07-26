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

    getProjects();
}

AudioEditor::~AudioEditor()
{
    delete ui;
}

// Reads all project files in the project directory and adds them to the combo box
void AudioEditor::getProjects()
{
    ui->comboBox_projects->clear();

    for (QString project : getFiles(settingsManager->getSetting(audioPath)))
    {
        if (project.contains(".ini"))
        {
            ui->comboBox_projects->addItem(cleanText(project));
        }
    }
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
    qDebug() << "Loading music files...";

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

    getProjects();

    loadProject();
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

        // Add to Tree View
        QTreeWidgetItem *catItem = new QTreeWidgetItem(0);
        catItem->setText(0, ui->lineEdit_category->text());
        catItem->setToolTip(0, ui->lineEdit_category->text());
        catItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogStart));

        ui->treeWidget_categories->addTopLevelItem(catItem);
    }

    ui->lineEdit_category->clear();
}

// Load the project to the editor
void AudioEditor::loadProject()
{
    qDebug() << "Loading project" << projectName << "...";

    isProjectOpen = true;

    ui->label_pleaseCreateOrLoadAProject->hide();

    // Generating music files tree view
    if (!filesAreLoaded)
    {
        loadFolderContentsToTreeView(ui->treeWidget_music, settingsManager->getSetting(musicPath));
        filesAreLoaded = true;
    }

    // Loading Categories
    loadCategories();
}

// Loads all categories and their sub-elements
void AudioEditor::loadCategories()
{
    qDebug() << "Loading Categories...";

    // Only execute if a project is loaded
    if (isProjectOpen)
    {
        // Making sure the tree widget is empty
        ui->treeWidget_categories->clear();

        // Setting up QSettings
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int catCount = settings.beginReadArray("Categories");

        // Loading categories
        for (int i = 0; i<catCount; i++)
        {
            settings.setArrayIndex(i);

            QString category = settings.value("name").toString();

            QTreeWidgetItem *catItem = new QTreeWidgetItem(0);
            catItem->setText(0, category);
            catItem->setToolTip(0, category);
            catItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogStart));

            // load scenarios
            loadScenarios(catItem);

            // add to tree
            ui->treeWidget_categories->addTopLevelItem(catItem);
        }

        settings.endArray();
    }
}

// Add Scenarios of category
void AudioEditor::loadScenarios(QTreeWidgetItem *catItem)
{
    // Setting up QSettings
    QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

    // Loading Scenarios
    int scenCount = settings.beginReadArray(catItem->text(0) + "_Scenarios");

    for (int i = 0; i<scenCount; i++)
    {
        settings.setArrayIndex(i);

        QString scenario = settings.value("name").toString();

        QTreeWidgetItem *scenItem = new QTreeWidgetItem(1);
        scenItem->setText(0, scenario);
        scenItem->setToolTip(0, scenario);
        scenItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));

        catItem->addChild(scenItem);

        // Add music lists, sound lists and radios
        loadMusicLists(scenItem, catItem->text(0));
        loadSoundLists(scenItem, catItem->text(0));
        loadRadios(scenItem, catItem->text(0));

    }

    settings.endArray();
}

// Add music lists of scenario
void AudioEditor::loadMusicLists(QTreeWidgetItem *scenItem, QString category)
{
    // Setting up QSettings
    QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

    // Loading Music Lists
    int count = settings.beginReadArray(category + "_" + scenItem->text(0) + "_MusicLists");

    for (int i = 0; i<count; i++)
    {
        settings.setArrayIndex(i);

        QString list = settings.value("name").toString();

        QTreeWidgetItem *item = new QTreeWidgetItem(2);
        item->setText(0, list);
        item->setToolTip(0, list);
        item->setIcon(0, style()->standardIcon(QStyle::SP_MediaVolume));

        scenItem->addChild(item);
    }

    settings.endArray();
}

// Add sound lists
void AudioEditor::loadSoundLists(QTreeWidgetItem *scenItem, QString category)
{
    // Setting up QSettings
    QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

    // Loading Music Lists
    int count = settings.beginReadArray(category + "_" + scenItem->text(0) + "_SoundLists");

    for (int i = 0; i<count; i++)
    {
        settings.setArrayIndex(i);

        QString list = settings.value("name").toString();

        QTreeWidgetItem *item = new QTreeWidgetItem(3);
        item->setText(0, list);
        item->setToolTip(0, list);
        item->setIcon(0, style()->standardIcon(QStyle::SP_DriveCDIcon));

        scenItem->addChild(item);
    }

    settings.endArray();
}

// Add radios
void AudioEditor::loadRadios(QTreeWidgetItem *scenItem, QString category)
{
    // Setting up QSettings
    QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

    // Loading Music Lists
    int count = settings.beginReadArray(category + "_" + scenItem->text(0) + "_Radios");

    for (int i = 0; i<count; i++)
    {
        settings.setArrayIndex(i);

        QString list = settings.value("name").toString();

        QTreeWidgetItem *item = new QTreeWidgetItem(4);
        item->setText(0, list);
        item->setToolTip(0, list);
        item->setIcon(0, style()->standardIcon(QStyle::SP_MessageBoxInformation));

        scenItem->addChild(item);
    }

    settings.endArray();
}

// Open project selected in combo box
void AudioEditor::on_pushButton_openProject_clicked()
{
    // Setting project name
    projectName = ui->comboBox_projects->currentText();

    // Loading project
    loadProject();
}

// When an item in the project content tree is double clicked
void AudioEditor::on_treeWidget_categories_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    int itemType = item->type();

    switch (itemType) {
    case 0:

        currentCategory = item->text(column);

        break;
    default:
        break;
    }
}

// Delete Category, Scenario or Element
void AudioEditor::on_pushButton_deleteSelected_clicked()
{
    // Only execute if a project is loaded
    if (isProjectOpen)
    {
        int type = ui->treeWidget_categories->currentItem()->type();

        QString typeString;

        switch (type) {
        case 0:
            typeString = "Categories";
            break;
        case 1:
            typeString = "Scenarios";
            break;
        case 2:
            typeString = "Elements";
            break;
        default:
            break;
        }

        // Setting up QSettings
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int catCount = settings.beginReadArray(typeString);

        // Loading categories
        QStringList categories;

        for (int i = 0; i<catCount; i++)
        {
            settings.setArrayIndex(i);

            QString category = settings.value("name").toString();

            qDebug() << category;

            if (category != ui->treeWidget_categories->currentItem()->text(0))
            {
                categories.push_back(category);
            }
        }
        settings.endArray();

        // Add only not deleted stuff
        settings.beginWriteArray(typeString);

        for (int i = 0; i < categories.size(); i++)
        {
            settings.setArrayIndex(i);

            settings.setValue("name", categories.at(i));
        }
        settings.endArray();

        // Re-loading categories
        loadCategories();
    }
}

// Add a new scenario
void AudioEditor::on_pushButton_newScenario_clicked()
{
    if (isProjectOpen && ui->lineEdit_scenario->text() != NULL)
    {
        QTreeWidgetItem *item = ui->treeWidget_categories->currentItem();

        while (item->type() != 0)
        {
            item = ui->treeWidget_categories->itemAbove(item);
        }

        QString category = item->text(0);

        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray(category + "_Scenarios");
        settings.endArray();

        settings.beginWriteArray(category + "_Scenarios");
        settings.setArrayIndex(size);

        settings.setValue("name", ui->lineEdit_scenario->text());
        settings.endArray();

        loadCategories();
    }
}

// Create a new music list
void AudioEditor::on_pushButton_newMusicList_clicked()
{
    if (isProjectOpen && ui->treeWidget_categories->currentItem()->type() == 1 && ui->lineEdit_musicList->text() != NULL)
    {
        QString scenario = ui->treeWidget_categories->currentItem()->text(0);

        QTreeWidgetItem *item = ui->treeWidget_categories->currentItem();

        while (item->type() != 0)
        {
            item = ui->treeWidget_categories->itemAbove(item);
        }

        QString category = item->text(0);

        QString arrayName = category + "_" + scenario + "_MusicLists";

        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray(arrayName);
        settings.endArray();

        settings.beginWriteArray(arrayName);
        settings.setArrayIndex(size);

        settings.setValue("name", ui->lineEdit_musicList->text());
        settings.endArray();

        loadCategories();

    }
}

// Create a new Sound List
void AudioEditor::on_pushButton_newSoundList_clicked()
{
    if (isProjectOpen && ui->treeWidget_categories->currentItem()->type() == 1 && ui->lineEdit_soundList->text() != NULL)
    {
        QString scenario = ui->treeWidget_categories->currentItem()->text(0);

        QTreeWidgetItem *item = ui->treeWidget_categories->currentItem();

        while (item->type() != 0)
        {
            item = ui->treeWidget_categories->itemAbove(item);
        }

        QString category = item->text(0);

        QString arrayName = category + "_" + scenario + "_SoundLists";

        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray(arrayName);
        settings.endArray();

        settings.beginWriteArray(arrayName);
        settings.setArrayIndex(size);

        settings.setValue("name", ui->lineEdit_soundList->text());
        settings.endArray();

        loadCategories();

    }
}

// Create a new Radio
void AudioEditor::on_pushButton_newRadio_clicked()
{
    if (isProjectOpen && ui->treeWidget_categories->currentItem()->type() == 1 && ui->lineEdit_radio->text() != NULL)
    {
        QString scenario = ui->treeWidget_categories->currentItem()->text(0);

        QTreeWidgetItem *item = ui->treeWidget_categories->currentItem();

        while (item->type() != 0)
        {
            item = ui->treeWidget_categories->itemAbove(item);
        }

        QString category = item->text(0);

        QString arrayName = category + "_" + scenario + "_Radios";

        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray(arrayName);
        settings.endArray();

        settings.beginWriteArray(arrayName);
        settings.setArrayIndex(size);

        settings.setValue("name", ui->lineEdit_radio->text());
        settings.endArray();

        loadCategories();

    }
}

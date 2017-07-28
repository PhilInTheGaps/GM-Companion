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

    ui->progressBar_loading->setHidden(true);

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

        QString path = baseFolder+"/"+file;
        path = path.replace(settingsManager->getSetting(musicPath), "");
        path = path.replace(settingsManager->getSetting(soundPath), "");
        item->setWhatsThis(0, path);

        baseItem->addChild(item);
    }
}

// Load a folder structure to a tree view
void AudioEditor::loadFolderContentsToTreeView(QTreeWidget *treeWidget, QString baseFolder)
{
    qDebug() << "Loading files...";

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
    if (!projectName.isNull())
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

    // Show loading bar
    ui->progressBar_loading->setHidden(false);

    isProjectOpen = true;

    ui->label_pleaseCreateOrLoadAProject->hide();

    // Generating music files tree view
    if (!filesAreLoaded)
    {
        loadFolderContentsToTreeView(ui->treeWidget_music, settingsManager->getSetting(musicPath));
        loadFolderContentsToTreeView(ui->treeWidget_sound, settingsManager->getSetting(soundPath));
        filesAreLoaded = true;
    }

    // Loading Categories
    loadCategories();

    // Hide loading bar
    ui->progressBar_loading->setHidden(true);
}

// Loads all categories and their sub-elements
void AudioEditor::loadCategories()
{
    qDebug() << "Loading Categories...";

    // Only execute if a project is loaded
    if (isProjectOpen)
    {
        // Making sure the tree widget is empty
        qDebug() << "Clearing tree view";
        ui->treeWidget_categories->clear();

        // Setting up QSettings
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int catCount = settings.beginReadArray("Categories");

        // Loading categories
        for (int i = 0; i<catCount; i++)
        {
            settings.setArrayIndex(i);

            QString category = settings.value("name").toString();

            qDebug() << "Loading category: " + category +" ...";

            QTreeWidgetItem *catItem = new QTreeWidgetItem(0);
            catItem->setText(0, category);
            catItem->setToolTip(0, category);
            catItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogStart));

            // load scenarios
            loadScenarios(catItem);

            // add to tree
            ui->treeWidget_categories->addTopLevelItem(catItem);

            // Expand item
            catItem->setExpanded(true);
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
        qDebug() << "   Loading scenario: " + scenario +" ...";

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
        qDebug() << "       Loading music list: " + list +" ...";

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
        qDebug() << "       Loading sound list: " + list +" ...";

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
        qDebug() << "       Loading radio: " + list +" ...";

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

        QString arrayName;

        switch (type) {
        case 0:
            arrayName = "Categories";
            break;
        case 1:
            arrayName = currentCategory + "_Scenarios";
            break;
        case 2:
            arrayName = currentCategory + "_" + currentScenario + "_MusicLists";
            break;
        case 3:
            arrayName = currentCategory + "_" + currentScenario + "_SoundLists";
            break;
        case 4:
            arrayName = currentCategory + "_" + currentScenario + "_Radios";
            break;
        default:
            break;
        }

        // Setting up QSettings
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int arraySize = settings.beginReadArray(arrayName);

        // Loading categories
        QStringList elements;

        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            QString element = settings.value("name").toString();

            qDebug() << element;

            if (element != ui->treeWidget_categories->currentItem()->text(0))
            {
                elements.push_back(element);
            }
        }
        settings.endArray();

        // Add only not deleted stuff
        settings.beginWriteArray(arrayName);

        for (int i = 0; i < elements.size(); i++)
        {
            settings.setArrayIndex(i);

            settings.setValue("name", elements.at(i));
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
    if (isProjectOpen && !currentScenario.isNull() && !ui->lineEdit_musicList->text().isNull())
    {
        qDebug() << "Adding new music list ...";

        QString arrayName = currentCategory + "_" + currentScenario + "_MusicLists";
        qDebug() << "Adding music list to: " + arrayName;

        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

        int size = settings.beginReadArray(arrayName);
        settings.endArray();
        qDebug() << "Music list position: " + size;

        qDebug() << "Writing array ...";
        settings.beginWriteArray(arrayName);
        settings.setArrayIndex(size);

        settings.setValue("name", ui->lineEdit_musicList->text());
        settings.endArray();
        qDebug() << "Added music list: " + ui->lineEdit_musicList->text();

        loadCategories();
    }
}

// Create a new Sound List
void AudioEditor::on_pushButton_newSoundList_clicked()
{
    if (isProjectOpen && !currentScenario.isNull() && !ui->lineEdit_soundList->text().isNull())
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
    if (isProjectOpen && !currentScenario.isNull() && !ui->lineEdit_radio->text().isNull())
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

// When another element is selected
void AudioEditor::on_treeWidget_categories_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (ui->treeWidget_categories->currentColumn() >= 0)
    {
        int type = current->type();

        // Change stacked widget index based on the item type
        // 0 is the default "No element selected" screen
        ui->stackedWidget_editElement->setCurrentIndex(type + 1);

        // Load stuff
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);
        int arraySize;

        QString description;

        QTreeWidgetItem *item = current;

        switch (type) {
        // Category
        case 0:
            currentCategory = current->text(0);

            arraySize = settings.beginReadArray("Categories");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->textEdit_categoryDescription->setText(description);
                }
            }

            break;

        // Scenario
        case 1:
            // Get current category
            while (item->type() != 0) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentCategory = item->text(0);

            currentScenario = current->text(0);

            arraySize = settings.beginReadArray(currentCategory+"_Scenarios");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->textEdit_scenarioDescription->setText(description);
                }
            }

            break;

        // Music List
        case 2:
            // Get scenario
            while (item->type() != 1) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentScenario = item->text(0);

            // Get category
            while (item->type() != 0) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentCategory = item->text(0);

            currentElement = current->text(0);

            arraySize = settings.beginReadArray(currentCategory+"_"+currentScenario+"_MusicLists");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->lineEdit_musicListDescription->setText(description);
                    ui->checkBox_musicRandom->setChecked(settings.value("random", false).toBool());

                    // Songs
                    ui->listWidget_musicList->clear();

                    int songs = settings.beginReadArray("songs");
                    for (int i = 0; i<songs; i++)
                    {
                        settings.setArrayIndex(i);
                        QString songName = settings.value("name", "UNKNOWN NAME").toString();
                        QString songPath = settings.value("path", "UNKNOWN PATH").toString();

                        QListWidgetItem *item = new QListWidgetItem(songName);
                        item->setWhatsThis(songPath);

                        ui->listWidget_musicList->addItem(item);
                    }
                    settings.endArray();
                }
            }
            settings.endArray();
            break;

        // Sound List
        case 3:
            // Get scenario
            while (item->type() != 1) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentScenario = item->text(0);

            // Get category
            while (item->type() != 0) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentCategory = item->text(0);

            currentElement = current->text(0);

            arraySize = settings.beginReadArray(currentCategory+"_"+currentScenario+"_SoundLists");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->lineEdit_soundListDescription->setText(description);

                    ui->radioButton_soundListRandom->setChecked(settings.value("random", true).toBool());
                    ui->radioButton_soundListLoop->setChecked(settings.value("loop", false).toBool());
                    ui->radioButton_soundListSequential->setChecked(settings.value("sequential", false).toBool());

                    // Sounds
                    ui->listWidget_soundList->clear();

                    int sounds = settings.beginReadArray("sounds");
                    for (int i = 0; i<sounds; i++)
                    {
                        settings.setArrayIndex(i);
                        QString soundName = settings.value("name", "UNKNOWN NAME").toString();
                        QString soundPath = settings.value("path", "UNKNOWN PATH").toString();

                        QListWidgetItem *item = new QListWidgetItem(soundName);
                        item->setWhatsThis(soundPath);

                        ui->listWidget_soundList->addItem(item);
                    }
                    settings.endArray();
                }
            }
            settings.endArray();
            break;

        // Radios
        case 4:
            // Get scenario
            while (item->type() != 1) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentScenario = item->text(0);

            // Get category
            while (item->type() != 0) {
                item = ui->treeWidget_categories->itemAbove(item);
            }
            currentCategory = item->text(0);

            currentElement = current->text(0);

            arraySize = settings.beginReadArray(currentCategory+"_"+currentScenario+"_Radios");

            for (int i = 0; i<arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();

                    ui->lineEdit_soundListDescription->setText(description);

                    ui->lineEdit_radioURL->setText(settings.value("URL").toString());
                }
            }

            break;

        default:

            ui->stackedWidget_editElement->setCurrentIndex(0);

            break;
        }
    }
}

// Save the element
void AudioEditor::on_pushButton_saveElement_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget_categories->currentItem();
    int type = item->type();
    QString name = item->text(0);

    int arraySize;
    QString description;

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+projectName+".ini", QSettings::IniFormat);

    switch (type) {

    // Category
    case 0:
        qDebug() << "Saving category ...";

        description = ui->textEdit_categoryDescription->toPlainText();

        arraySize = settings.beginReadArray("Categories");
        settings.endArray();

        arraySize = settings.beginReadArray("Categories");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
            }
        }

        settings.endArray();
        break;

    // Scenario
    case 1:
        qDebug() << "Saving scenario ...";

        description = ui->textEdit_scenarioDescription->toPlainText();

        arraySize = settings.beginReadArray(currentCategory + "_Scenarios");
        settings.endArray();

        arraySize = settings.beginReadArray(currentCategory + "_Scenarios");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
            }
        }

        settings.endArray();
        break;

    // Music List
    case 2:
        qDebug() << "Saving music list ...";

        description = ui->lineEdit_musicListDescription->text();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_MusicLists");
        settings.endArray();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_MusicLists");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
                settings.setValue("random", ui->checkBox_musicRandom->isChecked());

                // Save songs
                settings.beginWriteArray("songs");
                for (int i = 0; i<ui->listWidget_musicList->count(); i++)
                {
                    settings.setArrayIndex(i);
                    settings.setValue("name", ui->listWidget_musicList->item(i)->text());
                    settings.setValue("path", ui->listWidget_musicList->item(i)->whatsThis());
                }
                settings.endArray();
            }
        }

        settings.endArray();
        break;

    // Sound List
    case 3:
        qDebug() << "Saving sound list ...";

        description = ui->lineEdit_soundListDescription->text();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_SoundLists");
        settings.endArray();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_SoundLists");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
                settings.setValue("random", ui->radioButton_soundListRandom->isChecked());
                settings.setValue("loop", ui->radioButton_soundListLoop->isChecked());
                settings.setValue("sequential", ui->radioButton_soundListSequential->isChecked());

                // Save sounds
                settings.beginWriteArray("sounds");
                for (int i = 0; i<ui->listWidget_soundList->count(); i++)
                {
                    settings.setArrayIndex(i);
                    settings.setValue("name", ui->listWidget_soundList->item(i)->text());
                    settings.setValue("path", ui->listWidget_soundList->item(i)->whatsThis());
                }
                settings.endArray();
            }
        }

        settings.endArray();
        break;

    // Radio
    case 4:
        qDebug() << "Saving radio ...";

        description = ui->textEdit_radioDescription->toPlainText();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_Radios");
        settings.endArray();

        arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_Radios");
        for (int i = 0; i<arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);
                settings.setValue("URL", ui->lineEdit_radioURL->text());
            }
        }

        settings.endArray();
        break;

    // Everything else, should that actually be possible
    default:
        qDebug() << "ERROR: Something went wrong saving the current element.";
        break;
    }

}

// Remove a song from the music list
void AudioEditor::on_listWidget_musicList_itemDoubleClicked(QListWidgetItem *item)
{
    delete item;
}

// Remove a sound from the sound list
void AudioEditor::on_listWidget_soundList_itemDoubleClicked(QListWidgetItem *item)
{
    delete item;
}

// When an item in the music explorer is double clicked
void AudioEditor::on_treeWidget_music_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->type() == 1)
    {
        qDebug() << "Adding song: " + item->text(0);

        QListWidgetItem *listItem = new QListWidgetItem(item->text(0));
        listItem->setWhatsThis(item->whatsThis(0));

        ui->listWidget_musicList->addItem(listItem);
    }
}

// Add a sound to the sound list
void AudioEditor::on_treeWidget_sound_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->type() == 1)
    {
        qDebug() << "Adding sound: " + item->text(0);

        QListWidgetItem *listItem = new QListWidgetItem(item->text(0));
        listItem->setWhatsThis(item->whatsThis(0));

        ui->listWidget_soundList->addItem(listItem);
    }
}

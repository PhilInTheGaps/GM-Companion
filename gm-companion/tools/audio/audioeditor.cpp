#include "audioeditor.h"
#include "ui_audioeditor.h"
#include "gm-companion/functions.h"

#include <QDir>
#include <QStyle>
#include <QDebug>
#include <QSettings>
#include <QStringList>
#include <QHeaderView>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>

AudioEditor::AudioEditor(QWidget *parent) : QWidget(parent), ui(new Ui::AudioEditor)
{
    ui->setupUi(this);

    previewPlayer = new QMediaPlayer;
    connect(previewPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(previewPlayer_positionChanged(qint64)));

    settingsManager = new SettingsManager;

    isProjectOpen = false;

    getProjects();
}

AudioEditor::~AudioEditor()
{
    delete ui;
}

// Reads all project files in the project directory and adds them to the combo
// box
void AudioEditor::getProjects()
{
    ui->comboBox_projects->clear();

    for (QString project : getFiles(settingsManager->getSetting(audioPath)))
    {
        if (project.contains(".ini") && (project != "desktop.ini")) ui->comboBox_projects->addItem(cleanText(project));
    }
}

// Open project selected in combo box
void AudioEditor::on_pushButton_openProject_clicked()
{
    // Setting project name
    projectName = ui->comboBox_projects->currentText();

    // Loading project
    loadProject();
}

// Load the project to the editor
void AudioEditor::loadProject()
{
    qDebug() << "Loading project" << projectName << "...";

    isProjectOpen = true;

    // Generating music files tree view
    if (!filesAreLoaded)
    {
        ui->treeWidget_music->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        loadFolderContentsToTreeView(ui->treeWidget_music, settingsManager->getSetting(musicPath));

        ui->treeWidget_sound->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        loadFolderContentsToTreeView(ui->treeWidget_sound, settingsManager->getSetting(soundPath));

        ui->treeWidget_radio->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        loadFolderContentsToTreeView(ui->treeWidget_radio, settingsManager->getSetting(radioPath));

        filesAreLoaded = true;
    }

    // Loading Categories
    loadCategories();
}

// Load a folder structure to a tree view
void AudioEditor::loadFolderContentsToTreeView(QTreeWidget *treeWidget, QString baseFolder)
{
    qDebug() << "Loading files...";

    // Making sure it is empty
    treeWidget->clear();

    // Adding top level folders
    addSubFoldersToTreeItem(NULL, baseFolder, true, treeWidget);

    // Adding top level files
    addFilesToTreeItem(NULL, baseFolder, true, treeWidget);
}

// Adds a child item for every subfolder in the directory
void AudioEditor::addSubFoldersToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder, bool topLevel, QTreeWidget *tree)
{
    for (QString folder : getFolders(baseFolder))
    {
        if (!folder.contains("."))
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(0);
            item->setText(0, folder);
            item->setToolTip(0, folder);
            item->setWhatsThis(0, baseFolder + "/" + folder);
            item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
            item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

            if (topLevel) tree->addTopLevelItem(item);
            else baseItem->addChild(item);
        }
    }
}

// Adds a child item for every file inside the folder
void AudioEditor::addFilesToTreeItem(QTreeWidgetItem *baseItem, QString baseFolder, bool topLevel, QTreeWidget *tree)
{
    QStringList fileFormats = { ".mp3", ".wav", ".ogg", ".flac", ".m3u", ".pls" };

    for (QString file : getFiles(baseFolder))
    {
        // Check if file is supported
        bool isFile = false;

        for (QString fileType : fileFormats)
        {
            if (file.endsWith(fileType, Qt::CaseInsensitive)) isFile = true;
        }

        if (isFile)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(1);
            item->setText(0, file);
            item->setToolTip(0, file);
            item->setIcon(0, style()->standardIcon(QStyle::SP_FileIcon));

            QString path = baseFolder + "/" + file;
            path = path.replace(settingsManager->getSetting(musicPath), "");
            path = path.replace(settingsManager->getSetting(soundPath), "");
            path = path.replace(settingsManager->getSetting(radioPath), "");
            item->setWhatsThis(0, path);

            if (topLevel) tree->addTopLevelItem(item);
            else baseItem->addChild(item);
        }
    }
}

// When a music item is expanded, load folder contents
void AudioEditor::on_treeWidget_music_itemExpanded(QTreeWidgetItem *item)
{
    if ((item->type() == 0) && (item->childCount() == 0))
    {
        addSubFoldersToTreeItem(item, item->whatsThis(0));
        addFilesToTreeItem(item, item->whatsThis(0));
    }
}

// When a sound item is expanded, load folder contents
void AudioEditor::on_treeWidget_sound_itemExpanded(QTreeWidgetItem *item)
{
    if ((item->type() == 0) && (item->childCount() == 0))
    {
        addSubFoldersToTreeItem(item, item->whatsThis(0));
        addFilesToTreeItem(item, item->whatsThis(0));
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
        QSettings settings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);
        settings.setValue("ProjectName", projectName);
    }
}

// Add a category
void AudioEditor::addNewCategory()
{
    if ((ui->lineEdit_elementName->text() != NULL) && (projectName != NULL))
    {
        QSettings settings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);

        int size = settings.beginReadArray("Categories");
        settings.endArray();

        settings.beginWriteArray("Categories");
        settings.setArrayIndex(size);

        settings.setValue("name", ui->lineEdit_elementName->text());

        settings.endArray();

        // Add to Tree View
        QTreeWidgetItem *catItem = new QTreeWidgetItem(0);
        catItem->setText(0, ui->lineEdit_elementName->text());
        catItem->setToolTip(0, ui->lineEdit_elementName->text());
        catItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogStart));

        ui->treeWidget_categories->addTopLevelItem(catItem);
    }

    ui->lineEdit_elementName->clear();
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
        ui->treeWidget_categories->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

        // Setting up QSettings
        QSettings settings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);

        QStringList categories;
        int count = settings.beginReadArray("Categories_Order");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);
            categories.append(settings.value("name").toString());
        }

        settings.endArray();

        count = settings.beginReadArray("Categories");

        // Get all categories
        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);
            QString category = settings.value("name").toString();
            qDebug() << "Loading category: " + category + " ...";

            QTreeWidgetItem *catItem = new QTreeWidgetItem(0);
            catItem->setText(0, category);
            catItem->setToolTip(0, category);
            catItem->setIcon(0, style()->standardIcon(QStyle::SP_FileDialogStart));
            catItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

            // add to tree
            int index = categories.indexOf(category);

            if ((index > i) || (index < 0)) index = i;

            ui->treeWidget_categories->insertTopLevelItem(index, catItem);

            // load scenarios
            loadScenarios(catItem);

            // Expand item if it is the current category
            if (category == currentCategory) catItem->setExpanded(true);
        }
        settings.endArray();
    }
}

// Add Scenarios of category
void AudioEditor::loadScenarios(QTreeWidgetItem *catItem)
{
    // Setting up QSettings
    QSettings *settings = new QSettings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);

    QStringList scenarios;
    int scenCount = settings->beginReadArray(catItem->text(0) + "_Scenarios_Order");

    for (int i = 0; i < scenCount; i++)
    {
        settings->setArrayIndex(i);
        scenarios.append(settings->value("name").toString());
    }

    settings->endArray();

    scenCount = settings->beginReadArray(catItem->text(0) + "_Scenarios");

    // Loading Scenarios
    for (int i = 0; i < scenCount; i++)
    {
        settings->setArrayIndex(i);

        QString scenario = settings->value("name").toString();
        qDebug() << "   Loading scenario: " + scenario + " ...";

        QTreeWidgetItem *scenItem = new QTreeWidgetItem(1);
        scenItem->setText(0, scenario);
        scenItem->setToolTip(0, scenario);
        scenItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
        scenItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

        // add to tree
        int index = scenarios.indexOf(scenario);

        if ((index > i) || (index < 0)) index = i;

        catItem->insertChild(index, scenItem);

        // Expand Item if it is the current Scenario
        if ((catItem->text(0) == currentCategory) && (scenario == currentScenario)) scenItem->setExpanded(true);

        // Add music lists, sound lists and radios
        loadElements(scenItem, catItem->text(0));
    }

    settings->endArray();
}

void AudioEditor::loadElement(QTreeWidgetItem *scenItem, QString category, QString element, int type)
{
    QSettings settings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);

    QString suffix;
    QIcon   icon;

    switch (type) {
    case 2: // Music
        suffix = "_MusicLists";
        icon   = QIcon(":/icons/media/music.png");
        break;

    case 3: // Sound
        suffix = "_SoundLists";
        icon   = QIcon(":/icons/media/sound.png");
        break;

    case 4: // Radio
        suffix = "_Radios";
        icon   = QIcon(":/icons/media/radio.png");
        break;

    default:
        break;
    }

    int count = settings.beginReadArray(category + "_" + scenItem->text(0) + suffix);

    QString list;

    // Loading Elements
    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);
        list = settings.value("name").toString();

        if (list == element)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(type);
            item->setText(0, list);
            item->setToolTip(0, list);
            item->setIcon(0, icon);

            scenItem->addChild(item);

            if ((scenItem->text(0) == currentScenario) &&
                (scenItem->parent()->text(0) == currentCategory) &&
                (list == currentElement)) ui->treeWidget_categories->setCurrentItem(item);
        }
    }

    settings.endArray();
}

// Add all Elements to Scenario
void AudioEditor::loadElements(QTreeWidgetItem *scenItem, QString category)
{
    QSettings *settings = new QSettings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);

    int count = settings->beginReadArray(category + "_" + scenItem->text(0) + "_Order");

    QString name;
    QString type;

    for (int i = 0; i < count; i++)
    {
        settings->setArrayIndex(i);

        name = settings->value("name").toString();
        type = settings->value("type").toString();

        if (type == "MusicLists") loadElement(scenItem, category, name, 2);
        else if (type == "SoundLists") loadElement(scenItem, category, name, 3);
        else if (type == "Radios") loadElement(scenItem, category, name, 4);
    }

    settings->endArray();
}

// When an item in the project content tree is double clicked
void AudioEditor::on_treeWidget_categories_itemDoubleClicked(
    QTreeWidgetItem *item, int column)
{
    int itemType = item->type();

    switch (itemType) {
    case 0:
        currentCategory = item->text(column);
        break;

    case 1:
        currentScenario = item->text(column);
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
        QSettings settings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);

        int arraySize = settings.beginReadArray(arrayName);

        // Loading categories
        QStringList elements;

        for (int i = 0; i < arraySize; i++)
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

        // Switch back to "No Element selected"
        ui->stackedWidget_editElement->setCurrentIndex(0);
    }
}

// Add a new scenario
void AudioEditor::addNewScenario()
{
    QString scenario = ui->lineEdit_elementName->text();

    if (isProjectOpen && (scenario != NULL) && (currentCategory != NULL))
    {
        QSettings settings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);

        int size = settings.beginReadArray(currentCategory + "_Scenarios");
        settings.endArray();

        settings.beginWriteArray(currentCategory + "_Scenarios");
        settings.setArrayIndex(size);

        settings.setValue("name", scenario);
        currentScenario = scenario;

        settings.endArray();

        loadCategories();
    }
}

// Add a new Element (Music List, Sound List or Radio)
void AudioEditor::addNewElement(int type)
{
    QString element, arrayName, typeString;

    switch (type) {
    case 0:
        element = ui->lineEdit_elementName->text();
        ui->lineEdit_elementName->clear();
        arrayName  = currentCategory + "_" + currentScenario + "_MusicLists";
        typeString = "MusicLists";
        break;

    case 1:
        element = ui->lineEdit_elementName->text();
        ui->lineEdit_elementName->clear();
        arrayName  = currentCategory + "_" + currentScenario + "_SoundLists";
        typeString = "SoundLists";
        break;

    case 2:
        element = ui->lineEdit_elementName->text();
        ui->lineEdit_elementName->clear();
        arrayName  = currentCategory + "_" + currentScenario + "_Radios";
        typeString = "Radios";
        break;

    default:
        break;
    }


    if (isProjectOpen && !currentCategory.isNull() && !currentScenario.isNull() && !element.isNull())
    {
        QSettings settings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);

        // Save Element in Scenario
        int size = settings.beginReadArray(arrayName);
        settings.endArray();

        settings.beginWriteArray(arrayName);
        settings.setArrayIndex(size);

        settings.setValue("name", element);
        currentElement = element;
        settings.endArray();

        // Save Element Order
        size = settings.beginReadArray(currentCategory + "_" + currentScenario + "_Order");
        settings.endArray();

        settings.beginWriteArray(currentCategory + "_" + currentScenario + "_Order");
        settings.setArrayIndex(size);
        settings.setValue("name", element);
        settings.setValue("type", typeString);
        settings.endArray();


        loadCategories();
    }
}

// When another element is selected
void AudioEditor::on_treeWidget_categories_currentItemChanged(
    QTreeWidgetItem *current)
{
    if (ui->treeWidget_categories->currentColumn() >= 0)
    {
        int type = current->type();

        // Change stacked widget index based on the item type
        // 0 is the default "No element selected" screen
        ui->stackedWidget_editElement->setCurrentIndex(type + 1);

        // Load stuff
        QSettings settings(settingsManager->getSetting(audioPath) + "/" + projectName + ".ini", QSettings::IniFormat);
        int arraySize;

        QString description;
        QString iconPath;

        QTreeWidgetItem *item = current;

        switch (type) {
        // Category
        case 0:
            currentCategory = current->text(0);

            arraySize = settings.beginReadArray("Categories");

            for (int i = 0; i < arraySize; i++)
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

            arraySize = settings.beginReadArray(currentCategory + "_Scenarios");

            for (int i = 0; i < arraySize; i++)
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

            arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_MusicLists");

            for (int i = 0; i < arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();
                    ui->lineEdit_musicListDescription->setText(description);

                    // Icon
                    iconPath = settings.value("icon", "").toString();
                    ui->lineEdit_musicIcon->setText(iconPath);

                    if (!iconPath.isNull())
                    {
                        QString resPath = settingsManager->getSetting(
                            resourcesPath);
                        ui->label_musicIcon->setPixmap(QPixmap(resPath + "/" + iconPath).scaledToWidth(ui->label_musicIcon->width()));
                    }

                    // Get Playback Mode of Music List
                    ui->radioButton_musicListRandomOrder->setChecked(settings.value("randomPlayback", false).toBool());
                    ui->radioButton_musicListShuffleList->setChecked(settings.value("randomPlaylist", true).toBool());
                    ui->radioButton_musicListLoopList->setChecked(settings.value("loop", false).toBool());
                    ui->radioButton_musicListSequential->setChecked(settings.value("sequential", false).toBool());

                    // Songs
                    ui->listWidget_musicList->clear();

                    int songs = settings.beginReadArray("songs");

                    for (int i = 0; i < songs; i++)
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

            ui->tabWidget->setCurrentIndex(0);
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

            arraySize = settings.beginReadArray(currentCategory + "_" + currentScenario + "_SoundLists");

            for (int i = 0; i < arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();
                    ui->lineEdit_soundListDescription->setText(description);

                    // Icon
                    iconPath = settings.value("icon", "").toString();
                    ui->lineEdit_soundIcon->setText(iconPath);

                    if (!iconPath.isNull())
                    {
                        QString resPath = settingsManager->getSetting(
                            resourcesPath);
                        ui->label_soundIcon->setPixmap(QPixmap(resPath + "/" + iconPath).scaledToWidth(ui->label_soundIcon->width()));
                    }

                    ui->radioButton_soundListRandom->setChecked(settings.value("random", true).toBool());
                    ui->radioButton_soundListLoop->setChecked(settings.value("loop", false).toBool());
                    ui->radioButton_soundListSequential->setChecked(settings.value("sequential", false).toBool());

                    // Sounds
                    ui->listWidget_soundList->clear();

                    int sounds = settings.beginReadArray("sounds");

                    for (int i = 0; i < sounds; i++)
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

            ui->tabWidget->setCurrentIndex(1);
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

            arraySize = settings.beginReadArray(
                currentCategory + "_" + currentScenario + "_Radios");

            for (int i = 0; i < arraySize; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name").toString() == current->text(0))
                {
                    description = settings.value("description", "").toString();
                    ui->textEdit_radioDescription->setText(description);

                    // Icon
                    iconPath = settings.value("icon", "").toString();
                    ui->lineEdit_radioIcon->setText(iconPath);

                    if (!iconPath.isNull())
                    {
                        QString resPath = settingsManager->getSetting(
                            resourcesPath);
                        ui->label_radioIcon->setPixmap(QPixmap(resPath + "/" + iconPath).scaledToWidth(ui->label_radioIcon->width()));
                    }

                    ui->lineEdit_radioURL->setText(settings.value("URL").toString());
                }
            }

            ui->tabWidget->setCurrentIndex(2);
            break;

        default:

            ui->stackedWidget_editElement->setCurrentIndex(0);

            break;
        }
    }
}

// Save category order
void AudioEditor::saveCategoryOrder()
{
    QString   projectPath = settingsManager->getSetting(audioPath) + "/" + projectName + ".ini";
    QSettings settings(projectPath, QSettings::IniFormat);
    QTreeWidget *tree = ui->treeWidget_categories;

    int count = tree->topLevelItemCount();

    settings.beginWriteArray("Categories_Order");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("name", tree->topLevelItem(i)->text(0));
        saveScenarioOrder(tree->topLevelItem(i));
    }

    settings.endArray();
}

void AudioEditor::saveScenarioOrder(QTreeWidgetItem *categoryItem)
{
    QString   projectPath = settingsManager->getSetting(audioPath) + "/" + projectName + ".ini";
    QSettings settings(projectPath, QSettings::IniFormat);

    int count = categoryItem->childCount();

    settings.beginWriteArray(categoryItem->text(0) + "_Scenarios_Order");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("name", categoryItem->child(i)->text(0));
    }

    settings.endArray();
}

// Save order of elements
void AudioEditor::saveElementOrder()
{
    QTreeWidget *tree = ui->treeWidget_categories;
    int topLevelCount = tree->topLevelItemCount();

    if (topLevelCount > 0)
    {
        QString   projectPath = settingsManager->getSetting(audioPath) + "/" + projectName + ".ini";
        QSettings settings(projectPath, QSettings::IniFormat);

        for (int i = 0; i < topLevelCount; i++)
        {
            QTreeWidgetItem *topLevelItem = tree->topLevelItem(i);
            int scenarioCount             = topLevelItem->childCount();

            for (int j = 0; j < scenarioCount; j++)
            {
                QTreeWidgetItem *scenarioItem = topLevelItem->child(j);
                int elementCount              = scenarioItem->childCount();

                settings.beginWriteArray(topLevelItem->text(0) + "_" + scenarioItem->text(0) + "_Order");

                for (int k = 0; k < elementCount; k++)
                {
                    QTreeWidgetItem *elementItem = scenarioItem->child(k);
                    settings.setArrayIndex(k);

                    settings.setValue("name", elementItem->text(0));

                    switch (elementItem->type()) {
                    case 2: // Music List
                        settings.setValue("type", "MusicLists");
                        break;

                    case 3: // Sound List
                        settings.setValue("type", "SoundLists");
                        break;

                    case 4: // Radio
                        settings.setValue("type", "Radios");
                        break;

                    default:
                        break;
                    }
                }

                settings.endArray();
            }
        }
    }
}

// Save the element
void AudioEditor::saveElement()
{
    QTreeWidgetItem *item = ui->treeWidget_categories->currentItem();

    if (item != NULL)
    {
        QString   name        = item->text(0);
        QString   projectPath = settingsManager->getSetting(audioPath) + "/" + projectName + ".ini";
        QSettings settings(projectPath, QSettings::IniFormat);

        QString description;
        QString debugMessage;
        QString arrayName;
        QString contentName;
        QLineEdit   *iconLine;
        QListWidget *listWidget;

        switch (item->type()) {
        case 0: // Category
            debugMessage = "Saving category ...";
            description  = ui->textEdit_categoryDescription->toPlainText();
            arrayName    = "Categories";
            break;

        case 1: // Scenario
            debugMessage = "Saving scenario ...";
            description  = ui->textEdit_scenarioDescription->toPlainText();
            arrayName    = currentCategory + "_Scenarios";
            break;

        case 2: // Music List
            debugMessage = "Saving music list ...";
            description  = description = ui->lineEdit_musicListDescription->text();
            arrayName    = currentCategory + "_" + currentScenario + "_MusicLists";
            iconLine     = ui->lineEdit_musicIcon;
            listWidget   = ui->listWidget_musicList;
            contentName  = "songs";
            break;

        case 3: // Sound List
            debugMessage = "Saving sound list ...";
            description  = description = ui->lineEdit_soundListDescription->text();
            arrayName    = currentCategory + "_" + currentScenario + "_SoundLists";
            iconLine     = ui->lineEdit_soundIcon;
            listWidget   = ui->listWidget_soundList;
            contentName  = "sounds";
            break;

        case 4: // Radio
            debugMessage = "Saving radio ...";
            description  = description = ui->textEdit_radioDescription->toPlainText();
            arrayName    = currentCategory + "_" + currentScenario + "_Radios";
            iconLine     = ui->lineEdit_radioIcon;
            break;

        default:
            break;
        }

        qDebug() << debugMessage;
        int arraySize = settings.beginReadArray(arrayName);

        for (int i = 0; i < arraySize; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == name)
            {
                settings.setValue("description", description);

                // Lists
                if (item->type() >= 2)
                {
                    settings.setValue("icon", iconLine->text());

                    // Music specific
                    if (item->type() == 2)
                    {
                        // Save Playback Mode of Music List
                        settings.setValue("randomPlayback", ui->radioButton_musicListRandomOrder->isChecked());
                        settings.setValue("randomPlaylist", ui->radioButton_musicListShuffleList->isChecked());
                        settings.setValue("loop",           ui->radioButton_musicListLoopList->isChecked());
                        settings.setValue("sequential",     ui->radioButton_musicListSequential->isChecked());
                    }

                    // Sound specific
                    if (item->type() == 3)
                    {
                        settings.setValue("random",     ui->radioButton_soundListRandom->isChecked());
                        settings.setValue("loop",       ui->radioButton_soundListLoop->isChecked());
                        settings.setValue("sequential", ui->radioButton_soundListSequential->isChecked());
                    }

                    // Radio specific
                    if (item->type() == 4)
                    {
                        settings.setValue("URL", ui->lineEdit_radioURL->text());
                    }

                    // Save songs or sounds
                    if ((item->type() == 2) || (item->type() == 3))
                    {
                        settings.beginWriteArray(contentName);
                        QString contentName, contentPath;

                        for (int i = 0; i < listWidget->count(); i++)
                        {
                            settings.setArrayIndex(i);

                            contentName = listWidget->item(i)->text();
                            contentPath = listWidget->item(i)->whatsThis();

                            settings.setValue("name", contentName);
                            settings.setValue("path", contentPath);

                            qDebug().noquote() << " Name:" << contentName;
                            qDebug().noquote() << " Path:" << contentPath;
                        }
                        settings.endArray();
                    }
                }
            }
        }

        settings.endArray();

        saveElementOrder();
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

// Add a file to music or sound list
void AudioEditor::addFileToList(QTreeWidgetItem *item, int column, int type)
{
    if (item != NULL)
    {
        QString debugMessage;
        QListWidget *listWidget;

        if (type == 0)
        {
            debugMessage = "Adding song: ";
            listWidget   = ui->listWidget_musicList;
        }
        else
        {
            debugMessage = "Adding sound: ";
            listWidget   = ui->listWidget_soundList;
        }

        if (item->type() == 1)
        {
            qDebug() << debugMessage + item->text(column);

            QListWidgetItem *listItem = new QListWidgetItem(item->text(column));
            listItem->setWhatsThis(item->whatsThis(column));

            listWidget->addItem(listItem);

            saveElement();
        }
    }
}

// When an item in the music explorer is double clicked
void AudioEditor::on_treeWidget_music_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    addFileToList(item, column, 0);
}

// Add a sound to the sound list
void AudioEditor::on_treeWidget_sound_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    addFileToList(item, column, 1);
}

// Set a radio playlist
void AudioEditor::on_treeWidget_radio_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->lineEdit_radioURL->setText(item->whatsThis(column));
}

// Add all files from a folder to the music or sound list
void AudioEditor::addAllFilesFromFolder(QTreeWidgetItem *item, int type)
{
    if (item != NULL)
    {
        QString debugMessage;
        Setting setting;
        QListWidget *listWidget;

        if (type == 0)
        {
            debugMessage = "Adding all music files from folder: ";
            setting      = Setting::musicPath;
            listWidget   = ui->listWidget_musicList;
        }
        else if (type == 1)
        {
            debugMessage = "Adding all sound files from folder: ";
            setting      = Setting::soundPath;
            listWidget   = ui->listWidget_soundList;
        }

        if (item->type() == 0)
        {
            qDebug() << debugMessage + item->text(0);

            QString path = item->whatsThis(0);

            for (QString file : getFiles(path))
            {
                // If file is an audio file
                if (file.contains(".mp3") || file.contains(".wav") ||
                    file.contains(".ogg") || file.contains(".flac"))
                {
                    QString relativePath =
                        path.replace(settingsManager->getSetting(setting), "");
                    relativePath += "/" + file;

                    QListWidgetItem *listItem = new QListWidgetItem(file);
                    listItem->setWhatsThis(relativePath);

                    listWidget->addItem(listItem);
                }
            }

            saveElement();
        }
    }
}

// Add all music files from the selected folder
void AudioEditor::on_pushButton_addAllFilesFromMusicFolder_clicked()
{
    addAllFilesFromFolder(ui->treeWidget_music->currentItem(), 0);
}

void AudioEditor::on_pushButton_addAllFilesFromSoundFolder_clicked()
{
    addAllFilesFromFolder(ui->treeWidget_sound->currentItem(), 1);
}

// Create a new music list with all the songs from the selected folder
void AudioEditor::on_pushButton_convertFolderToMusicList_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget_music->currentItem();

    if ((item->type() == 0) && (item != NULL))
    {
        QString folderName = item->text(0);
        QString path       = item->whatsThis(0);

        qDebug() << "Creating music list from folder: " + folderName;

        // Set the name of the music list
        ui->lineEdit_elementName->setText(folderName);

        // Create new list
        addNewElement(0);

        // Select the new music list
        for (int i = 0; i < ui->treeWidget_categories->topLevelItemCount(); i++)
        {
            // Get the correct category item
            QTreeWidgetItem *catItem = ui->treeWidget_categories->topLevelItem(i);

            if (catItem->text(0) == currentCategory)
            {
                for (int j = 0; j < catItem->childCount(); j++)
                {
                    // Get the scenario item
                    QTreeWidgetItem *scenItem = catItem->child(j);

                    if (scenItem->text(0) == currentScenario)
                    {
                        for (int m = 0; m < scenItem->childCount(); m++)
                        {
                            // Get the music list item
                            QTreeWidgetItem *listItem = scenItem->child(m);

                            if (listItem->text(0) == folderName)
                            {
                                // Expand the scenario item
                                scenItem->setExpanded(true);

                                // Set list Item as curren item
                                ui->treeWidget_categories->setCurrentItem(listItem);
                            }
                        }
                    }
                }
            }
        }

        for (QString file : getFiles(path))
        {
            if (file.contains(".mp3") || file.contains(".wav") ||
                file.contains(".ogg") || file.contains(".flac"))
            {
                QString relativePath = path.replace(settingsManager->getSetting(musicPath), "");
                relativePath += "/" + file;

                QListWidgetItem *listItem = new QListWidgetItem(file);
                listItem->setWhatsThis(relativePath);

                ui->listWidget_musicList->addItem(listItem);
            }
        }

        saveElement();
    }
}

// Start preview
void AudioEditor::playPreview(QString path)
{
    qDebug().noquote() << "Previewing file:" << path;

    if (ui->checkBox_autoplay->isChecked() && QFile(path).exists())
    {
        ui->pushButton_playPause->setEnabled(true);
        ui->pushButton_playPause->setChecked(false);
        on_pushButton_playPause_toggled(false);

        previewPlayer->setMedia(QUrl::fromLocalFile(path));
        previewPlayer->setVolume(ui->verticalSlider_volume->value());
        previewPlayer->play();
    }
}

// Mark all missing files red, all others transparent
void AudioEditor::markMissingFiles(QList<int>indices, QListWidget *widget)
{
    for (int i = 0; i < widget->count(); i++)
    {
        if (indices.contains(i))
        {
            widget->item(i)->setBackgroundColor(Qt::red);
        }
        else
        {
            widget->item(i)->setBackgroundColor(Qt::transparent);
        }
    }
}

// Returns a integer list with all list widget item indices that represent a
// missing file
QList<int>AudioEditor::getMissingFiles(int type)
{
    QList<int>   list;
    QListWidget *widget;
    Setting s;

    if (type == 0)
    {
        widget = ui->listWidget_musicList;
        s      = Setting::musicPath;
    }
    else if (type == 1)
    {
        widget = ui->listWidget_soundList;
        s      = Setting::soundPath;
    }

    for (int i = 0; i < widget->count(); i++)
    {
        if (!QFile(settingsManager->getSetting(s) + "/" + widget->item(i)->whatsThis()).exists())
        {
            list.append(i);
        }
    }

    return list;
}

// Opens a File Chooser to set a file path
QString AudioEditor::setIconPath(QString windowTitle)
{
    QString path;
    QFileDialog *fileDialog = new QFileDialog;

    if (QDir(lastPath).exists() && !lastPath.isNull())
    {
        fileDialog->setDirectory(lastPath);
    }
    else
    {
        fileDialog->setDirectory(settingsManager->getSetting(Setting::resourcesPath));
    }
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle(windowTitle);

    if (fileDialog->exec() == QDialog::Accepted)
    {
        QStringList paths = fileDialog->selectedFiles();
        path = paths.at(0);
    }

    lastPath = fileDialog->directory().absolutePath();
    return path;
}

// Choose the music list, sound list or radio icon
void AudioEditor::chooseIcon(int type)
{
    QString resPath = settingsManager->getSetting(resourcesPath);
    QString windowName;
    QLineEdit *lineEdit;
    QLabel    *label;

    switch (type) {
    case 0: // Music
        windowName = "Set Music Icon";
        lineEdit   = ui->lineEdit_musicIcon;
        label      = ui->label_musicIcon;
        break;

    case 1: // Sounds
        windowName = "Set Sound Icon";
        lineEdit   = ui->lineEdit_soundIcon;
        label      = ui->label_soundIcon;
        break;

    case 2: // Radio
        windowName = "Set Radio Icon";
        lineEdit   = ui->lineEdit_radioIcon;
        label      = ui->label_radioIcon;
        break;

    default:
        break;
    }

    QString path = setIconPath(windowName).replace(resPath, "");

    lineEdit->setText(path);
    lineEdit->setToolTip(path);
    label->setPixmap(QPixmap(resPath + "/" + path).scaledToWidth(label->width()));
}

void AudioEditor::on_pushButton_playPause_toggled(bool checked)
{
    if (checked)
    {
        ui->pushButton_playPause->setIcon(QIcon(":/icons/media/play.png"));
        ui->pushButton_playPause->setText("Play");
        previewPlayer->pause();
    }
    else
    {
        ui->pushButton_playPause->setIcon(QIcon(":/icons/media/pause.png"));
        ui->pushButton_playPause->setText("Pause");
        previewPlayer->play();
    }
}

void AudioEditor::on_treeWidget_music_currentItemChanged(QTreeWidgetItem *current)
{
    if (current->type() == 1)
    {
        playPreview(settingsManager->getSetting(musicPath) + current->whatsThis(0));
    }
}

void AudioEditor::on_treeWidget_sound_currentItemChanged(QTreeWidgetItem *current)
{
    if (current->type() == 1)
    {
        playPreview(settingsManager->getSetting(soundPath) + current->whatsThis(0));
    }
}

void AudioEditor::on_verticalSlider_volume_valueChanged(int value)
{
    previewPlayer->setVolume(value);
}

void AudioEditor::on_horizontalSlider_progress_sliderMoved(int position)
{
    previewPlayer->setPosition(position);
}

void AudioEditor::previewPlayer_positionChanged(qint64 position)
{
    ui->horizontalSlider_progress->setMaximum(previewPlayer->duration());
    ui->horizontalSlider_progress->setValue(position);
}

// Add new Element
void AudioEditor::on_pushButton_newElement_clicked()
{
    switch (ui->comboBox_elementType->currentIndex()) {
    case 0: // Category
        addNewCategory();
        break;

    case 1: // Scenario
        addNewScenario();
        break;

    case 2: // Music
        addNewElement(0);
        break;

    case 3: // Sounds
        addNewElement(1);
        break;

    case 4: // Radio
        addNewElement(2);
        break;

    default:
        break;
    }
}

// Show which music files are missing
void AudioEditor::on_pushButton_detectMissingMusic_clicked()
{
    markMissingFiles(getMissingFiles(0), ui->listWidget_musicList);
}

// Show which sound files are missing
void AudioEditor::on_pushButton_detectMissingSounds_clicked()
{
    markMissingFiles(getMissingFiles(1), ui->listWidget_soundList);
}

// Choose Music Icon
void AudioEditor::on_toolButton_musicIcon_clicked()
{
    chooseIcon(0);
}

// Choose Sound Icon
void AudioEditor::on_toolButton_soundIcon_clicked()
{
    chooseIcon(1);
}

// Choose Radio Icon
void AudioEditor::on_toolButton_radioIcon_clicked()
{
    chooseIcon(2);
}

// Add music file to list
void AudioEditor::on_pushButton_addToMusicList_clicked()
{
    addFileToList(ui->treeWidget_music->currentItem(), 0, 0);
}

// Add sound file to list
void AudioEditor::on_pushButton_addToSoundList_clicked()
{
    addFileToList(ui->treeWidget_sound->currentItem(), 0, 1);
}

// Remove music file from list
void AudioEditor::on_pushButton_removeFileMusic_clicked()
{
    if (ui->listWidget_musicList->currentItem() != NULL) delete ui->listWidget_musicList->currentItem();
}

// Remove sound file from list
void AudioEditor::on_pushButton_removeSoundFile_clicked()
{
    if (ui->listWidget_soundList->currentItem() != NULL) delete ui->listWidget_soundList->currentItem();
}

// Move selected item
void AudioEditor::moveElement(QTreeWidgetItem *item, int steps)
{
    if (item != NULL)
    {
        if (item->type() > 0) // Scenarios or Lists
        {
            QTreeWidgetItem *parentItem = item->parent();
            int index                   = parentItem->indexOfChild(item);

            if ((index + steps > -1) && (index + steps < parentItem->childCount()))
            {
                QTreeWidgetItem *child = parentItem->takeChild(index);
                parentItem->insertChild(index + steps, child);

                ui->treeWidget_categories->setCurrentItem(item);
            }
        }
        else if (item->type() == 0) // Top Level Item = Category
        {
            int index = ui->treeWidget_categories->indexOfTopLevelItem(item);

            if ((index + steps > -1) && (index + steps < ui->treeWidget_categories->topLevelItemCount()))
            {
                QTreeWidgetItem *temp = ui->treeWidget_categories->takeTopLevelItem(index);
                ui->treeWidget_categories->insertTopLevelItem(index + steps, temp);

                ui->treeWidget_categories->setCurrentItem(item);
            }
        }
    }
}

// Move selected item up
void AudioEditor::on_pushButton_up_clicked()
{
    moveElement(ui->treeWidget_categories->currentItem(), -1);
}

// Move selected item down
void AudioEditor::on_pushButton_down_clicked()
{
    moveElement(ui->treeWidget_categories->currentItem(), 1);
}

void AudioEditor::on_pushButton_save_clicked()
{
    saveCategoryOrder();
    saveElement();
}

void AudioEditor::on_pushButton_openProjectFolder_clicked()
{
    QDesktopServices::openUrl(settingsManager->getSetting(Setting::audioPath));
}

void AudioEditor::on_pushButton_saveCategory_clicked()
{
    saveElement();
}

void AudioEditor::on_pushButton_saveScenario_clicked()
{
    saveElement();
}

void AudioEditor::on_pushButton_saveMusicList_clicked()
{
    saveElement();
}

void AudioEditor::on_pushButton_saveSoundList_clicked()
{
    saveElement();
}

void AudioEditor::on_pushButton_saveRadio_clicked()
{
    saveElement();
}

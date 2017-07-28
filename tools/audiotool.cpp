#include "audiotool.h"
#include "ui_audiotool.h"
#include "functions.h"
#include "flowlayout.h"
#include "editors/audioeditor.h"

#include <QDebug>
#include <QScrollArea>
#include <QToolButton>
#include <QVBoxLayout>

AudioTool::AudioTool(SettingsManager *sManager, QWidget *parent) : QWidget(parent), ui(new Ui::AudioTool)
{
    ui->setupUi(this);

    settingsManager = sManager;

    // Music
    musicPlayer = new QMediaPlayer;
    connect(musicPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(updateMetaData()));

    musicPlaylist = new QMediaPlaylist;
    musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);

    signalMapperMusic = new QSignalMapper;
    connect(signalMapperMusic, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));

    // Sound
    signalMapperSound = new QSignalMapper;
    connect(signalMapperSound, SIGNAL(mapped(QString)), this, SLOT(playSound(QString)));

    // Display all available projects in the combo box
    getProjects();

    // Set a layout for the element frame
    FlowLayout *elementLayout = new FlowLayout;
    ui->frame_elements->setLayout(elementLayout);

    // Load default project
    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    settings.beginGroup("AudioTool");
    for (int i = 0; i<ui->comboBox_projects->count(); i++)
    {
        if (ui->comboBox_projects->itemText(i) == settings.value("defaultProject").toString())
        {
            ui->comboBox_projects->setCurrentIndex(i);
            loadProject(ui->comboBox_projects->itemText(i));
        }
    }
}

AudioTool::~AudioTool()
{
    delete ui;
}

// Load a project
void AudioTool::loadProject(QString project)
{
    qDebug() << "Loading project: " + project + " ...";

    ui->listWidget_categories->clear();

    currentProject = project;

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+project, QSettings::IniFormat);

    // Load categories
    int categories = settings.beginReadArray("Categories");

    for (int i = 0; i<categories; i++)
    {
        settings.setArrayIndex(i);

        QString categoryName = settings.value("name").toString();
        QString categoryDescription = settings.value("description").toString();

        QListWidgetItem *categoryItem = new QListWidgetItem(categoryName);
        categoryItem->setToolTip(categoryDescription);

        ui->listWidget_categories->addItem(categoryItem);
    }
    settings.endArray();
}

// Get all available projects
void AudioTool::getProjects()
{
    ui->comboBox_projects->clear();

    QString projectsFolder = settingsManager->getSetting(audioPath);

    for (QString project : getFiles(projectsFolder))
    {
        if (project.contains(".ini") && !project.contains("desktop.ini"))
        {
            ui->comboBox_projects->addItem(project);
        }
    }
}

// Play Music
void AudioTool::playMusic(QString musicList)
{
    QString category = ui->listWidget_categories->currentItem()->text();
    QString scenario = ui->listWidget_scenarios->currentItem()->text();

    ui->label_element->setText(musicList);

    // Clear Playlist
    musicPlaylist->clear();
    ui->comboBox_music->clear();
    ui->listWidget_songs->clear();

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);
    int musicLists = settings.beginReadArray(category+"_"+scenario+"_MusicLists");

    bool random;

    for (int i = 0; i<musicLists; i++)
    {
        settings.setArrayIndex(i);

        if (settings.value("name").toString() == musicList)
        {
            random = settings.value("random", false).toBool();

            int songCount = settings.beginReadArray("songs");

            for (int j = 0; j<songCount; j++)
            {
                settings.setArrayIndex(j);

                QString name = settings.value("name").toString();
                QString path = settings.value("path").toString();

                if (QFile(path).exists())
                {
                    ui->comboBox_music->addItem(cleanText(name));

                    QListWidgetItem *listItem = new QListWidgetItem(name);
                    listItem->setToolTip(path);
                    ui->listWidget_songs->addItem(listItem);

                    musicPlaylist->addMedia(QUrl::fromLocalFile(settingsManager->getSetting(musicPath)+path));
                }
            }

            settings.endArray();
        }
    }

    settings.endArray();

    // Set playlist, volume and play
    musicPlayer->setPlaylist(musicPlaylist);
    musicPlayer->setVolume(ui->horizontalSlider_music->value());
    musicPlayer->play();

    // If random mode is active, start with a random song
    if (random){
        musicPlaylist->setPlaybackMode(QMediaPlaylist::Random);
        musicPlaylist->next();
    }
}

// Play Sounds
void AudioTool::playSound(QString soundList)
{
    // Check if sound player already exists in list
    bool checked = false;

    for (QMediaPlayer *sPlayer : soundPlayerList)
    {
        if (sPlayer->objectName() == soundList)
        {
            checked = true;

            sPlayer->stop();

            soundPlayerList.removeOne(sPlayer);

            delete sPlayer;
        }
    }

    if (!checked)
    {
        QString category = ui->listWidget_categories->currentItem()->text();
        QString scenario = ui->listWidget_scenarios->currentItem()->text();

        QMediaPlayer *player = new QMediaPlayer;
        QMediaPlaylist *playlist = new QMediaPlaylist;

        player->setPlaylist(playlist);
        soundPlayerList.push_back(player);

        player->setObjectName(soundList);

        bool random;
        bool loop;
        bool sequential;

        // Read properties
        QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);
        int lists = settings.beginReadArray(category+"_"+scenario+"_SoundLists");

        for (int i = 0; i<lists; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name").toString() == soundList)
            {
                random = settings.value("random", true).toBool();
                loop = settings.value("loop", false).toBool();
                sequential = settings.value("sequential", false).toBool();

                int sounds = settings.beginReadArray("sounds");

                for (int j = 0; j<sounds; j++)
                {
                    settings.setArrayIndex(j);

                    QString path = settingsManager->getSetting(soundPath) + settings.value("path").toString();

                    if (QFile(path).exists())
                    {
                        playlist->addMedia(QUrl::fromLocalFile(path));
                    }

                }

                settings.endArray();
            }
        }

        settings.endArray();

        // Set playback mode
        if (random) {
            playlist->setPlaybackMode(QMediaPlaylist::Random);
            playlist->next();
        }

        else if (loop) {
            playlist->setPlaybackMode(QMediaPlaylist::Loop);
        }

        else if (sequential) {
            playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        }

        player->setVolume(ui->horizontalSlider_sound->value());
        player->play();
    }

}

// Display the metadata of the currently playing song
void AudioTool::updateMetaData()
{
    ui->comboBox_music->setCurrentIndex(musicPlaylist->currentIndex());
}

// Getting the groups in the category and creating the frame for it
QList<QFrame*> AudioTool::createGroupFrame(QString folder, QFrame *frame)
{
    QList<QFrame*> groupFrames;

    QSettings settings(settingsManager->getSetting(Setting::musicPath)+"/"+folder+"/groups.ini", QSettings::IniFormat);
    int groupsSize = settings.beginReadArray("Groups");

    for (int i = 0; i<groupsSize; i++){
        settings.setArrayIndex(i);

        // Getting the group name
        QString group = settings.value("group").toString();

        // Actually creating the frame and it's layout
        QFrame* f = new QFrame;
        f->setStyleSheet("background: none;");
        f->setToolTip(group);

        FlowLayout* fLayout = new FlowLayout;
        f->setLayout(fLayout);

        // Creating a group title label
        QLabel* l = new QLabel(group);
        l->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        l->setStyleSheet("background: none;");

        // Adding the label and the frame to the "main frame" ;)
        frame->layout()->addWidget(l);
        frame->layout()->addWidget(f);

        // Adding the frame to the frame list
        groupFrames.push_back(f);
    }

    settings.endArray();

    return groupFrames;
}

// Generate the category list
void AudioTool::generateCategoryList(QStringList categories)
{
    ui->listWidget_categories->clear();

    for (QString cat : categories)
    {
        if (!cat.contains("."))
        {
            ui->listWidget_categories->addItem(cleanText(cat));
            ui->listWidget_categories->item(ui->listWidget_categories->count()-1)->setToolTip(cat);
        }
    }
}

// Generate the scenario list
void AudioTool::generateScenarioList(QString category)
{
    ui->listWidget_scenarios->clear();

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);

    // Load scenarios
    int scenarios = settings.beginReadArray(category+"_Scenarios");

    for (int i = 0; i<scenarios; i++)
    {
        settings.setArrayIndex(i);

        QString scenarioName = settings.value("name").toString();
        QString scenarioDescription = settings.value("description").toString();

        QListWidgetItem *scenarioItem = new QListWidgetItem(scenarioName);
        scenarioItem->setToolTip(scenarioDescription);

        ui->listWidget_scenarios->addItem(scenarioItem);
    }
    settings.endArray();
}

// Generate the buttons for all the elements of a scenario
void AudioTool::generateElementButtons(QString scenario)
{
    QString category = ui->listWidget_categories->currentItem()->text();

    qDebug() << "Removing old Elements...";
    qDeleteAll(ui->frame_elements->children());

    QVBoxLayout *vlayout = new QVBoxLayout;
    ui->frame_elements->setLayout(vlayout);

    FlowLayout *musicLayout = new FlowLayout;
    vlayout->addLayout(musicLayout);

    FlowLayout *soundLayout = new FlowLayout;
    vlayout->addLayout(soundLayout);

    FlowLayout *radioLayout = new FlowLayout;
    vlayout->addLayout(radioLayout);

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);

    // Music Lists
    qDebug() << "Adding music lists ...";
    int musicLists = settings.beginReadArray(category+"_"+scenario+"_MusicLists");

    for (int i = 0; i<musicLists; i++)
    {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        QString description = settings.value("description").toString();

        QPushButton *button = new QPushButton(name);
        button->setToolTip(description);
        button->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

        connect(button, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
        signalMapperMusic->setMapping(button, name);

        musicLayout->addWidget(button);
    }
    settings.endArray();

    // Sound Lists
    qDebug() << "Adding sound lists ...";
    int soundLists = settings.beginReadArray(category+"_"+scenario+"_SoundLists");

    for (int i = 0; i<soundLists; i++)
    {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        QString description = settings.value("description").toString();

        QPushButton *button = new QPushButton(name);
        button->setToolTip(description);
        button->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));
        button->setCheckable(true);

        connect(button, SIGNAL(clicked(bool)), signalMapperSound, SLOT(map()));
        signalMapperSound->setMapping(button, name);

        soundLayout->addWidget(button);
    }
    settings.endArray();

    // Radios
    qDebug() << "Adding radios ...";
    int radios = settings.beginReadArray(category+"_"+scenario+"_Radios");

    for (int i = 0; i<radios; i++)
    {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        QString description = settings.value("description").toString();

        QPushButton *button = new QPushButton(name);
        button->setToolTip(description);
        button->setIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));

//        connect(button, SIGNAL(clicked()), signalMapperSounds, SLOT(map()));
//        signalMapperMusic->setMapping(button, name);

        radioLayout->addWidget(button);
    }
    settings.endArray();
}

// Generate all the buttons and tabs in the "Small Buttons" page
void AudioTool::generateSmallButtons(QStringList categories)
{
    qDebug() << tr("Generating Small Music Buttons...");

    for (QString folder : categories){
        if (!folder.contains(".")){

            // Creating the scroll area and setting a frame as it's widget
            // The scroll area is the widget that becomes a category tab, the frame contains all the group-frames
            QScrollArea *scrollArea = new QScrollArea;
            QFrame *frame = new QFrame;
            QVBoxLayout* frameLayout = new QVBoxLayout;

            frame->setLayout(frameLayout);
            frameLayout->setAlignment(Qt::AlignTop);

            ui->tabWidget_Small->addTab(scrollArea, cleanText(folder));
            scrollArea->setWidget(frame);
            scrollArea->setWidgetResizable(true);

            // Checking if a background image for that category exists and adding it to the frame via a stylesheet
            if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png);"
                                     "background-position: center top;}");
            }
            else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg);"
                                     "background-position: center top;}");
            }

            // Creating all the group frames
            QList<QFrame*> groupFrames = createGroupFrame(folder, frame);

            // Generating musicButtons
            QString path = settingsManager->getSetting(Setting::musicPath)+"/"+folder;
            QStringList musicFolders = getFolders(path);

            // Label and frame for buttons without a specified group
            QLabel* allLabel = new QLabel("General");
            allLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
            allLabel->setStyleSheet("background: none;");
            frame->layout()->addWidget(allLabel);

            QFrame* allFrame = new QFrame;
            allFrame->setStyleSheet("background: none;");
            allFrame->setToolTip("General");

            FlowLayout *flowLayoutMusic = new FlowLayout;
            allFrame->setLayout(flowLayoutMusic);
            frame->layout()->addWidget(allFrame);

            // Creating the buttons
            for (QString s : musicFolders){
                if (!s.contains(".")){
                    QToolButton* b = new QToolButton;

                    // Connect button with music signal mapper
                    connect(b, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
                    signalMapperMusic->setMapping(b, settingsManager->getSetting(Setting::musicPath)+"/"+folder+"/"+s);

                    // Setting basic button properties
                    b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                    b->setIconSize(QSize(64,64));
                    b->setMinimumWidth(150);
                    b->setMinimumHeight(90);
                    b->setStyleSheet("background-color: #222222;");

                    // Setting the button font
                    QFont font;
                    font.setPixelSize(15);
                    b->setFont(font);

                    // Setting the button text and if it is too long, splitting it in to multiple lines
                    QString title = cleanText(s);

                    if (title.length()>20){
                        int i = title.indexOf(" ", 14);
                        title.remove(i, 1);
                        title.insert(i, "\n");
                    }

                    b->setText(title);
                    b->setToolTip(cleanText(s));

                    // Setting the button icon
                    if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png").exists()){
                        QPixmap pixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png");
                        QIcon icon(pixmap);
                        b->setIcon(icon);
                    }
                    else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg").exists()){
                        QPixmap pixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg");
                        QIcon icon(pixmap);
                        b->setIcon(icon);
                    }

                    // Checking if the button belongs in a group
                    QSettings settings(settingsManager->getSetting(Setting::musicPath)+"/"+folder+"/groups.ini", QSettings::IniFormat);

                    bool inGroup = false;
                    for (int gIndex = 0; gIndex<groupFrames.size(); gIndex++){
                        int size = settings.beginReadArray(groupFrames.at(gIndex)->toolTip());

                        for (int j = 0; j<size; j++){
                            settings.setArrayIndex(j);
                            QString value = settings.value("folder").toString();

                            if (s == value){
                                inGroup = true;

                                QFrame* gFrame = groupFrames.at(gIndex);
                                gFrame->layout()->addWidget(b);

                                break;
                            }
                        }

                        settings.endArray();
                    }

                    if (!inGroup)
                        flowLayoutMusic->addWidget(b);
                }
            }
        }
    }
}

// Switching to list view
void AudioTool::on_radioButton_List_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// Switching to large button view
void AudioTool::on_radioButton_Large_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Switching to small button view
void AudioTool::on_radioButton_Small_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


//}else{
//    if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png").exists()){
////                            b->setStyleSheet(getStyleSheet(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png"));
//    }
//    else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg").exists()){
////                            b->setStyleSheet(getStyleSheet(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg"));
//    }
//    else if (QFile(QApplication::applicationDirPath()+"/resources/button.png").exists()){
////                            b->setStyleSheet(getStyleSheet(QApplication::applicationDirPath()+"/resources/button.png"));
//    }
//    else{
//        b->setStyleSheet("QToolButton {"
//                         "min-width: 152; "
//                         "min-height: 152; "
//                         "padding: 1px; "
//                         "border-radius: 2px; "
//                         "background-color: #222222;"
//                         "background-position: center center;}"
//                         "QToolButton QWidget{"
//                         "color: white"
//                         "} ");
//    }

//    b->setFixedSize(152, 152);
//}

// Play
void AudioTool::on_pushButton_play_clicked()
{
    musicPlayer->play();
}

// Pause
void AudioTool::on_pushButton_pause_clicked()
{
    musicPlayer->pause();
}

// Replay
void AudioTool::on_pushButton_replay_clicked()
{
    musicPlayer->setPosition(0);
}

// Next
void AudioTool::on_pushButton_next_clicked()
{
    musicPlaylist->next();
}

// Adjust music volume
void AudioTool::on_horizontalSlider_music_valueChanged(int value)
{
    musicPlayer->setVolume(value);
}

void AudioTool::on_horizontalSlider_sound_valueChanged(int value)
{
    for (QMediaPlayer *player : soundPlayerList)
    {
        player->setVolume(value);
    }
}

void AudioTool::on_comboBox_music_currentIndexChanged(int index)
{
    if (index != musicPlaylist->currentIndex())
    {
        musicPlaylist->setCurrentIndex(index);
    }
}

// When a category is selected in list view
void AudioTool::on_listWidget_categories_currentRowChanged(int currentRow)
{
    if (currentRow > -1)
    {
        generateScenarioList(ui->listWidget_categories->currentItem()->text());

        ui->textEdit_categoryDescription->setText(ui->listWidget_categories->currentItem()->toolTip());
        ui->label_category->setText(ui->listWidget_categories->currentItem()->text());
    }
}

// When a scenario is selected
void AudioTool::on_listWidget_scenarios_currentRowChanged(int currentRow)
{
    if (currentRow > -1)
    {
        generateElementButtons(ui->listWidget_scenarios->currentItem()->text());

        ui->textEdit_scenarioDescription->setText(ui->listWidget_scenarios->currentItem()->toolTip());
        ui->label_scenario->setText(ui->listWidget_scenarios->currentItem()->text());
    }

}

// Open Audio Editor
void AudioTool::on_pushButton_openEditor_clicked()
{
    AudioEditor *audioEditor = new AudioEditor;
    audioEditor->showMaximized();
}

// Check or uncheck set project as default
void AudioTool::on_checkBox_setProjectAsDefault_toggled(bool checked)
{
    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    settings.beginGroup("AudioTool");

    if (checked)
    {
        settings.setValue("defaultProject", ui->comboBox_projects->currentText());
    }

    else
    {
        if (ui->comboBox_projects->currentText() == settings.value("defaultProject", "").toString())
        {
            settings.setValue("defaultProject", "");
        }
    }

    settings.endGroup();
}

// When load project is clicked
void AudioTool::on_pushButton_loadProject_clicked()
{
    loadProject(ui->comboBox_projects->currentText());
}

// When the project combobox changes
void AudioTool::on_comboBox_projects_currentTextChanged(const QString &arg1)
{
    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    settings.beginGroup("AudioTool");

    if (arg1 == settings.value("defaultProject", "").toString())
    {
        ui->checkBox_setProjectAsDefault->setChecked(true);
    }

    else
    {
        ui->checkBox_setProjectAsDefault->setChecked(false);
    }

    settings.endGroup();
}

void AudioTool::on_listWidget_songs_currentRowChanged(int currentRow)
{
    if (currentRow >= 0)
    {
        ui->comboBox_music->setCurrentIndex(currentRow);
    }
}

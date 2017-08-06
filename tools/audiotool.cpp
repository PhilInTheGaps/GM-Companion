#include "audiotool.h"
#include "ui_audiotool.h"
#include "functions.h"
#include "ui/flowlayout.h"
#include "editors/audioeditor.h"

#include <QDebug>
#include <QScrollArea>
#include <QToolButton>
#include <QVBoxLayout>

// Only Windows relevant
#ifdef _WIN32
#include <QtWinExtras>
#endif

AudioTool::AudioTool(SettingsManager *sManager, QWidget *parent) : QWidget(parent), ui(new Ui::AudioTool)
{
    qDebug() << "Loading AudioTool ...";

    ui->setupUi(this);

    settingsManager = sManager;

    // Music
    musicPlayer = new QMediaPlayer;
    connect(musicPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(updateMetaData()));

    musicPlaylist = new QMediaPlaylist;
    musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);

    signalMapperMusic = new QSignalMapper;
    connect(signalMapperMusic, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));

    // Radio
    radioPlayer = new QMediaPlayer;

    signalMapperRadio = new QSignalMapper;
    connect(signalMapperRadio, SIGNAL(mapped(QString)), this, SLOT(playRadio(QString)));

    // Sound
    signalMapperSound = new QSignalMapper;
    connect(signalMapperSound, SIGNAL(mapped(QString)), this, SLOT(playSound(QString)));

    // Display all available projects in the combo box
    getProjects();

    // Load default project
    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    settings.beginGroup("AudioTool");
    for (int i = 0; i<ui->comboBox_projects->count(); i++)
    {
        if (ui->comboBox_projects->itemText(i) == settings.value("defaultProject").toString())
        {
            qDebug() << "Loading default project: " + ui->comboBox_projects->itemText(i) + " ...";
            ui->comboBox_projects->setCurrentIndex(i);
            loadProject(ui->comboBox_projects->itemText(i));
        }
    }

    // Only Windows relevant
    #ifdef _WIN32
    // Creates thumbnail toolbar
    qDebug() << "Adding thumbnail toolbar ...";

    QWinThumbnailToolBar* thumbnailToolBar = new QWinThumbnailToolBar(this);
    thumbnailToolBar->setWindow(parent->windowHandle());

    QWinThumbnailToolButton *playToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    playToolButton->setEnabled(true);
    playToolButton->setToolTip(tr("Music: Play"));
    playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playToolButton, SIGNAL(clicked()), this, SLOT(on_pushButton_play_clicked()));

    QWinThumbnailToolButton *pauseToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    pauseToolButton->setEnabled(true);
    pauseToolButton->setToolTip(tr("Music: Pause"));
    pauseToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    connect(pauseToolButton, SIGNAL(clicked()), this, SLOT(on_pushButton_pause_clicked()));

    QWinThumbnailToolButton *nextToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    nextToolButton->setEnabled(true);
    nextToolButton->setToolTip(tr("Music: Next"));
    nextToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(nextToolButton, SIGNAL(clicked()), this, SLOT(on_pushButton_next_clicked()));

    thumbnailToolBar->addButton(playToolButton);
    thumbnailToolBar->addButton(pauseToolButton);
    thumbnailToolBar->addButton(nextToolButton);
    #endif
}

AudioTool::~AudioTool()
{
    delete ui;
}

// Get all available projects
void AudioTool::getProjects()
{
    qDebug() << "Getting available projects ...";

    ui->comboBox_projects->clear();

    QString projectsFolder = settingsManager->getSetting(audioPath);

    for (QString project : getFiles(projectsFolder))
    {
        if (project.contains(".ini") && !project.contains("desktop.ini"))
        {
            ui->comboBox_projects->addItem(cleanText(project));
        }
    }
}

// Load a project
void AudioTool::loadProject(QString project)
{
    qDebug() << "Loading project: " + project + " ...";

    ui->listWidget_categories->clear();

    for (int i = 0; i<ui->tabWidget_Small->count(); i++)
    {
        ui->tabWidget_Small->removeTab(i);
        ui->tabWidget_Large->removeTab(i);
    }

    project = cleanText(project)+".ini";
    currentProject = project;

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+project, QSettings::IniFormat);

    // Load categories
    int categories = settings.beginReadArray("Categories");

    for (int i = 0; i<categories; i++)
    {
        settings.setArrayIndex(i);

        QString categoryName = settings.value("name").toString();
        QString categoryDescription = settings.value("description").toString();

        // List View
        QListWidgetItem *categoryItem = new QListWidgetItem(categoryName);
        categoryItem->setToolTip(categoryDescription);

        ui->listWidget_categories->addItem(categoryItem);

        // Small Buttons
        QFrame *sFrame = new QFrame;
        QVBoxLayout *sFrameLayout = new QVBoxLayout;
        sFrameLayout->setAlignment(Qt::AlignTop);
        sFrame->setLayout(sFrameLayout);

        ui->tabWidget_Small->addTab(sFrame, categoryName);

        // Large Buttons
        QFrame *lFrame = new QFrame;
        QVBoxLayout *lFrameLayout = new QVBoxLayout;
        lFrameLayout->setAlignment(Qt::AlignTop);
        lFrame->setLayout(lFrameLayout);

        ui->tabWidget_Large->addTab(lFrame, categoryName);

        // Both Button Views
        generateScenarioFrames(categoryName, sFrame, lFrame);
    }
    settings.endArray();
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
void AudioTool::generateListViewElementButtons(QString scenario)
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
        signalMapperMusic->setMapping(button, name+";"+category+";"+scenario);

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
        signalMapperSound->setMapping(button, name+";"+category+";"+scenario);

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

        connect(button, SIGNAL(clicked()), signalMapperRadio, SLOT(map()));
        signalMapperRadio->setMapping(button, name+";"+category+";"+scenario);

        radioLayout->addWidget(button);
    }
    settings.endArray();
}

// Generate Scenarios for Button View
void AudioTool::generateScenarioFrames(QString category, QFrame *sFrame, QFrame *lFrame)
{
    qDebug() << "Generating scenario frames for category: " + category + " ...";

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);

    // Load scenarios
    int scenarios = settings.beginReadArray(category+"_Scenarios");

    for (int i = 0; i<scenarios; i++)
    {
        settings.setArrayIndex(i);

        QString scenarioName = settings.value("name").toString();
        QString scenarioDescription = settings.value("description").toString();

        // Small Buttons
        sFrame->layout()->addWidget(new QLabel(scenarioName));

        QFrame *sSubFrame = new QFrame;
        FlowLayout *sSubFrameLayout = new FlowLayout;
        sSubFrame->setLayout(sSubFrameLayout);
        sSubFrame->setToolTip(scenarioDescription);

        sFrame->layout()->addWidget(sSubFrame);

        // Large Buttons
        lFrame->layout()->addWidget(new QLabel(scenarioName));

        QFrame *lSubFrame = new QFrame;
        FlowLayout *lSubFrameLayout = new FlowLayout;
        lSubFrame->setLayout(lSubFrameLayout);
        lSubFrame->setToolTip(scenarioDescription);

        lFrame->layout()->addWidget(lSubFrame);

        // Both
        generateElementButtons(category, scenarioName, sSubFrame, lSubFrame);
    }
    settings.endArray();
}

// Generate Elements for Button View
void AudioTool::generateElementButtons(QString category, QString scenario, QFrame *sFrame, QFrame *lFrame)
{
    qDebug() << "Generating element buttons for scenario: " + scenario + " in category: " + category + " ...";

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);

    // Music Lists
    qDebug() << "   Generating music lists ...";
    int musicLists = settings.beginReadArray(category+"_"+scenario+"_MusicLists");

    for (int i = 0; i<musicLists; i++)
    {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        QString description = settings.value("description").toString();

        // Small Buttons
        QToolButton *sb = new QToolButton;
        sb->setText(name);
        sb->setToolTip(description);
        sb->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

        connect(sb, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
        signalMapperMusic->setMapping(sb, name+";"+category+";"+scenario);

        sb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        sb->setIconSize(QSize(64,64));
        sb->setMinimumWidth(150);
        sb->setMinimumHeight(90);
        sb->setStyleSheet("background-color: #222222;");

//        if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"".png").exists()){
//            QPixmap pixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png");
//            QIcon icon(pixmap);
//            b->setIcon(icon);
//        }
//        else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg").exists()){
//            QPixmap pixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg");
//            QIcon icon(pixmap);
//            b->setIcon(icon);
//        }

        sFrame->layout()->addWidget(sb);

        // Large Buttons
        QToolButton *lb = new QToolButton;
        lb->setText(name);
        lb->setToolTip(description);
        lb->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

        connect(lb, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
        signalMapperMusic->setMapping(lb, name+";"+category+";"+scenario);

        lb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        lb->setIconSize(QSize(128,128));
        lb->setMinimumWidth(150);
        lb->setMinimumHeight(150);
        lb->setStyleSheet("background-color: #222222;");

        lFrame->layout()->addWidget(lb);
    }
    settings.endArray();

    // Sound Lists
    qDebug() << "   Generating sound lists ...";
    int soundLists = settings.beginReadArray(category+"_"+scenario+"_SoundLists");

    for (int i = 0; i<soundLists; i++)
    {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        QString description = settings.value("description").toString();

        // Small Buttons
        QToolButton* sb = new QToolButton;
        sb->setText(name);
        sb->setToolTip(description);
        sb->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));

        connect(sb, SIGNAL(clicked()), signalMapperSound, SLOT(map()));
        signalMapperSound->setMapping(sb, name+";"+category+";"+scenario);

        sb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        sb->setIconSize(QSize(64,64));
        sb->setMinimumWidth(150);
        sb->setMinimumHeight(90);
        sb->setStyleSheet("background-color: #222222;");
        sb->setCheckable(true);

        sFrame->layout()->addWidget(sb);

        // Large Buttons
        QToolButton* lb = new QToolButton;
        lb->setText(name);
        lb->setToolTip(description);
        lb->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));

        connect(lb, SIGNAL(clicked()), signalMapperSound, SLOT(map()));
        signalMapperSound->setMapping(lb, name+";"+category+";"+scenario);

        lb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        lb->setIconSize(QSize(128,128));
        lb->setMinimumWidth(150);
        lb->setMinimumHeight(150);
        lb->setStyleSheet("background-color: #222222;");
        lb->setCheckable(true);

        lFrame->layout()->addWidget(lb);
    }
    settings.endArray();

    // Radios
    qDebug() << "   Generating radios ...";
    int radios = settings.beginReadArray(category+"_"+scenario+"_Radios");

    for (int i = 0; i<radios; i++)
    {
        settings.setArrayIndex(i);

        QString name = settings.value("name").toString();
        QString description = settings.value("description").toString();

        // Small Buttons
        QToolButton* sb = new QToolButton;
        sb->setText(name);
        sb->setToolTip(description);
        sb->setIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));

        connect(sb, SIGNAL(clicked()), signalMapperRadio, SLOT(map()));
        signalMapperRadio->setMapping(sb, name+";"+category+";"+scenario);

        sb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        sb->setIconSize(QSize(64,64));
        sb->setMinimumWidth(150);
        sb->setMinimumHeight(90);
        sb->setStyleSheet("background-color: #222222;");

        sFrame->layout()->addWidget(sb);

        // Large Buttons
        QToolButton* lb = new QToolButton;
        lb->setText(name);
        lb->setToolTip(description);
        lb->setIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));

        connect(lb, SIGNAL(clicked()), signalMapperRadio, SLOT(map()));
        signalMapperRadio->setMapping(lb, name+";"+category+";"+scenario);

        lb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        lb->setIconSize(QSize(128,128));
        lb->setMinimumWidth(150);
        lb->setMinimumHeight(150);
        lb->setStyleSheet("background-color: #222222;");

        lFrame->layout()->addWidget(lb);

    }
    settings.endArray();
}

// Play Music
void AudioTool::playMusic(QString arg)
{
    QStringList args = arg.split(";");

    QString musicList = args.at(0);
    QString category = args.at(1);
    QString scenario = args.at(2);

    qDebug() << "Playing music list: " + musicList + " ...";

    radioPlayer->stop();

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

            qDebug() << "This element contains the following songs:";

            for (int j = 0; j<songCount; j++)
            {
                settings.setArrayIndex(j);

                QString name = settings.value("name").toString();
                QString path = settings.value("path").toString();

                path = settingsManager->getSetting(musicPath)+path;

                qDebug() << "   " + name;
                qDebug() << "   " + path;

                if (QFile(path).exists())
                {
                    ui->comboBox_music->addItem(cleanText(name));

                    QListWidgetItem *listItem = new QListWidgetItem(name);
                    listItem->setToolTip(path);
                    ui->listWidget_songs->addItem(listItem);

                    musicPlaylist->addMedia(QUrl::fromLocalFile(path));
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

// Display the metadata of the currently playing song
void AudioTool::updateMetaData()
{
    ui->comboBox_music->setCurrentIndex(musicPlaylist->currentIndex());

    // TODO
}

// Play Sounds
void AudioTool::playSound(QString arg)
{
    QStringList args = arg.split(";");

    QString soundList = args.at(0);
    QString category = args.at(1);
    QString scenario = args.at(2);

    // Check if sound player already exists in list
    bool checked = false;

    for (QMediaPlayer *sPlayer : soundPlayerList)
    {
        if (sPlayer->objectName() == soundList)
        {
            qDebug() << "Stopping sound list: " + soundList + " ...";

            checked = true;

            sPlayer->stop();

            soundPlayerList.removeOne(sPlayer);

            delete sPlayer;
        }
    }

    if (!checked)
    {
        qDebug() << "Playing sound list: " + soundList + " ...";

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

// Play Radio
void AudioTool::playRadio(QString arg)
{
    QStringList args = arg.split(";");

    QString radio = args.at(0);
    QString category = args.at(1);
    QString scenario = args.at(2);

    qDebug() << "Playing radio: " + radio + " ...";

    musicPlayer->stop();

    ui->listWidget_songs->clear();
    ui->comboBox_music->clear();

    ui->label_element->setText(radio);

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);
    int radios = settings.beginReadArray(category+"_"+scenario+"_Radios");

    QString url;

    for (int i = 0; i<radios; i++)
    {
        settings.setArrayIndex(i);

        if (settings.value("name").toString() == radio)
        {
            url = settings.value("url").toString();
        }
    }

    qDebug() << "URL: " +url;

    settings.endArray();

    if (url.contains(".m3u"))
    {
        QMediaPlaylist *radioPlaylist = new QMediaPlaylist;
        radioPlayer->setPlaylist(radioPlaylist);

        if (QFile(settingsManager->getSetting(radioPath)+url).exists())
            radioPlaylist->load(QUrl::fromLocalFile(settingsManager->getSetting(radioPath)+url));
        else
            radioPlaylist->load(QUrl(url));
    }
    else
    {
        radioPlayer->setMedia(QUrl(url));
    }

    radioPlayer->setVolume(ui->horizontalSlider_music->value());
    radioPlayer->play();
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
    radioPlayer->play();
}

// Pause
void AudioTool::on_pushButton_pause_clicked()
{
    radioPlayer->pause();
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
    radioPlayer->setVolume(value);
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
        generateListViewElementButtons(ui->listWidget_scenarios->currentItem()->text());

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

// When a song is selected in the song list view, play it
void AudioTool::on_listWidget_songs_currentRowChanged(int currentRow)
{
    if (currentRow >= 0)
    {
        // Changing the combo box index causes the music player to change to the song
        ui->comboBox_music->setCurrentIndex(currentRow);
    }
}

// Update Projects List
void AudioTool::on_pushButton_updateProjects_clicked()
{
    getProjects();
}

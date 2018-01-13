#include "audiotool.h"
#include "ui_audiotool.h"
#include "gm-companion/functions.h"
#include "gm-companion/ui/flowlayout.h"
#include "gm-companion/editors/audioeditor.h"

#include <QDebug>
#include <QScrollArea>
#include <QToolButton>
#include <QVBoxLayout>
#include <QMediaMetaData>
#include <QDesktopServices>
#include <QDir>
#include <QApplication>
#include <QScreen>

// Only Windows relevant
#ifdef _WIN32
#include <QtWinExtras>
#endif

AudioTool::AudioTool(SettingsManager *sManager, QWidget *parent) : QWidget(parent), ui(new Ui::AudioTool)
{
    qDebug().noquote() << "Loading AudioTool ...";

    ui->setupUi(this);

    settingsManager = sManager;

    // Music
    musicPlayer = new QMediaPlayer;
    connect(musicPlayer, SIGNAL(metaDataChanged()), this, SLOT(updateMetaData())); //metaDataAvailableChanged

    musicPlaylist = new QMediaPlaylist;
    musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);

    signalMapperMusic = new QSignalMapper;
    connect(signalMapperMusic, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));

    // Radio
    radioPlayer = new QMediaPlayer;
    signalMapperRadio = new QSignalMapper;
    connect(signalMapperRadio, SIGNAL(mapped(QString)), this, SLOT(playRadio(QString)));
    radioActive = false;

    // Sound
    signalMapperSound = new QSignalMapper;
    connect(signalMapperSound, SIGNAL(mapped(QString)), this, SLOT(playSound(QString)));

    // Categories
    signalMapperCategories = new QSignalMapper;
    connect(signalMapperCategories, SIGNAL(mapped(QString)), this, SLOT(changeCategory(QString)));

    // Display all available projects in the combo box
    getProjects();

    // Load default project
    QSettings settings(QDir::homePath()+"/.gm-companion/settings.ini", QSettings::IniFormat);
    settings.beginGroup("AudioTool");
    for (int i = 0; i<ui->comboBox_projects->count(); i++)
    {
        if (ui->comboBox_projects->itemText(i) == settings.value("defaultProject").toString())
        {
            qDebug().noquote() << "Loading default project: " + ui->comboBox_projects->itemText(i) + " ...";
            ui->comboBox_projects->setCurrentIndex(i);
            loadProject(ui->comboBox_projects->itemText(i));
        }
    }

    // Only Windows relevant
    #ifdef _WIN32
    // Creates thumbnail toolbar
    qDebug().noquote() << "Adding thumbnail toolbar ...";

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
    qDebug().noquote() << "Getting available projects ...";

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
    qDebug().noquote() << "Loading project: " + project + " ...";

    // Clean old category stuff
    qDeleteAll(ui->categoryBar->children());
    qDeleteAll(ui->scrollAreaWidgetContents->children());
    ui->listWidget_scenarios->clear();

    project = cleanText(project)+".ini";
    currentProject = project;

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+project, QSettings::IniFormat);

    QHBoxLayout *categoryLayout = new QHBoxLayout;
    ui->categoryBar->setLayout(categoryLayout);

    // Load categories
    int categories = settings.beginReadArray("Categories");

    for (int i = 0; i<categories; i++)
    {
        settings.setArrayIndex(i);

        QString categoryName = settings.value("name").toString();
        QString categoryDescription = settings.value("description").toString();

        // Category Buttons
        QPushButton *categoryButton = new QPushButton(categoryName);
        categoryButton->setToolTip(categoryName + ": " + categoryDescription);
        categoryButton->setMinimumHeight(25);

        QFont f;
        f.setPointSize(12);
        f.setFamily("Helvetica");

        categoryButton->setFont(f);
        ui->categoryBar->layout()->addWidget(categoryButton);

        categoryButton->setStyleSheet("QPushButton{color: #eff0f1; background-color: #31363b; border-width: 1px; border-color: #76797C; "
                                      "border-style: solid; padding: 5px; border-radius: 2px; outline: none;}");

        connect(categoryButton, SIGNAL(clicked()), signalMapperCategories, SLOT(map()));
        signalMapperCategories->setMapping(categoryButton, categoryName);
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
void AudioTool::generateElementButtons(QString scenario)
{
    QString category = currentCategory;

    qDebug().noquote() << "Removing old Elements...";
    qDeleteAll(ui->scrollAreaWidgetContents->children());

    // Create and set Layouts for Music, Sounds and Radios
    QVBoxLayout *vlayout = new QVBoxLayout;
    ui->scrollAreaWidgetContents->setLayout(vlayout);

    FlowLayout *musicLayout = new FlowLayout;
    vlayout->addLayout(musicLayout);

    FlowLayout *soundLayout = new FlowLayout;
    vlayout->addLayout(soundLayout);

    FlowLayout *radioLayout = new FlowLayout;
    vlayout->addLayout(radioLayout);

    vlayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));


    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);

    // Calculate a good button size based on the screen resolution
    int screenWidth = QApplication::primaryScreen()->availableGeometry().width();
    int buttonWidth = screenWidth / 10;
    int buttonHeight = screenWidth / 10;

    QStringList listNames = {"MusicLists", "SoundLists", "Radios"};
    qDebug().noquote() << "Adding Elements ...";
    for (int i = 0; i<listNames.size(); i++)
    {
        int size = settings.beginReadArray(category+"_"+scenario+"_"+listNames.at(i));

        for (int j = 0; j<size; j++)
        {
            settings.setArrayIndex(j);

            QString name = settings.value("name").toString();
//            QString description = settings.value("description").toString();

            // Create Button
            QWidget* bWidget = new QWidget;
            QVBoxLayout* bLayout = new QVBoxLayout;
            QPushButton *button = new QPushButton;
            QLabel* bLabel = new QLabel(name);

            bWidget->setMaximumWidth(buttonWidth+10);
            bWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

            bLabel->setToolTip(name);
            bLabel->setAlignment(Qt::AlignHCenter);
            bLabel->setWordWrap(true);

            QFont f;
            f.setPointSize(10);
            f.setFamily("Helvetica");
            bLabel->setFont(f);

            button->setToolTip(name);
            button->setMinimumSize(buttonWidth, buttonHeight);
            button->setMaximumSize(buttonWidth, buttonHeight);
            button->setIconSize(QSize(buttonWidth-10, buttonHeight-10));

            bWidget->setLayout(bLayout);
            bLayout->addWidget(button);
            bLayout->addWidget(bLabel);

            switch (i){
            case 0: // Music
                button->setIcon(QIcon(":/resources/mediaIcons/music_image.png"));

                connect(button, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
                signalMapperMusic->setMapping(button, name+";"+category+";"+scenario);

                musicLayout->addWidget(bWidget);
                break;
            case 1: // Sound
                button->setIcon(QIcon(":/resources/mediaIcons/sound_image.png"));
                button->setCheckable(true);

                connect(button, SIGNAL(clicked(bool)), signalMapperSound, SLOT(map()));
                signalMapperSound->setMapping(button, name+";"+category+";"+scenario);

                soundLayout->addWidget(bWidget);
                break;
            case 2: // Radio
                button->setIcon(QIcon(":/resources/mediaIcons/radio_image.png"));

                connect(button, SIGNAL(clicked()), signalMapperRadio, SLOT(map()));
                signalMapperRadio->setMapping(button, name+";"+category+";"+scenario);

                radioLayout->addWidget(bWidget);
                break;
            default:
                break;
            }
        }
        settings.endArray();
    }
}

// Display the metadata of the currently playing song
void AudioTool::updateMetaData()
{
    if (musicPlayer->isMetaDataAvailable())
    {
        // Set Album Cover
        // Sometimes ThumbnailImage works, sometimes it does not. I have no idea why.
        // So the current solution is simply testing if any of the possible meta data values work
        QImage img = musicPlayer->metaData(QMediaMetaData::ThumbnailImage).value<QImage>();
        if (img.isNull())
            img = musicPlayer->metaData(QMediaMetaData::PosterImage).value<QImage>();
        if (img.isNull())
            img = musicPlayer->metaData(QMediaMetaData::CoverArtImage).value<QImage>();
        ui->label_albumCover->setPixmap(QPixmap::fromImage(img).scaledToWidth(ui->label_albumCover->width()));

        // Set other MetaData stuff
        ui->label_title->setText("Title: " + musicPlayer->metaData(QMediaMetaData::Title).toString());
        ui->label_album->setText("Album: " + musicPlayer->metaData(QMediaMetaData::AlbumTitle).toString());

        // Same thing as with the album cover, sometimes it works, sometimes not.
        QString artist = musicPlayer->metaData(QMediaMetaData::Author).toString();
        if (artist.isNull())
            artist = musicPlayer->metaData(QMediaMetaData::AlbumArtist).toString();
        if (artist.isNull())
            artist = musicPlayer->metaData(QMediaMetaData::Composer).toString();

        ui->label_artist->setText("Artist: " + artist);
    }
}

// Play Music
void AudioTool::playMusic(QString arg)
{
    QStringList args = arg.split(";");

    QString musicList = args.at(0);
    QString category = args.at(1);
    QString scenario = args.at(2);

    qDebug().noquote() << "Playing music list: " + musicList + " ...";

    radioPlayer->stop();
    radioActive = false;

    // Clear Playlist
    musicPlaylist->clear();
    ui->listWidget_songs->clear();

    // Create new Playlist
    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);
    int musicLists = settings.beginReadArray(category+"_"+scenario+"_MusicLists");

    bool randomPlaylist = false;
    bool randomPlayback = true;
    bool loop = false;
    bool sequential = false;
    bool hasSongs = true;

    for (int i = 0; i<musicLists; i++)
    {
        settings.setArrayIndex(i);
        if (settings.value("name").toString() == musicList)
        {
            // Get Playback Mode of Music List
            randomPlayback = settings.value("randomPlayback", true).toBool();
            randomPlaylist = settings.value("randomPlaylist", false).toBool();
            loop = settings.value("loop", false).toBool();
            sequential = settings.value("sequential", false).toBool();

            // Add Songs to Playlist
            int songCount = settings.beginReadArray("songs");
            qDebug().noquote() << "This element contains the following songs:" << songCount;

            if (songCount > 0)
            {
                QList<Song> songs;

                for (int j = 0; j<songCount; j++)
                {
                    settings.setArrayIndex(j);

                    QString name = settings.value("name").toString();
                    QString path = settings.value("path").toString();

                    path = settingsManager->getSetting(musicPath)+path;
                    qDebug().noquote() << "   " + name + "\n   " + path;

                    if (QFile(path).exists())
                    {
                        Song song;
                        song.title = name;
                        song.path = path;
                        songs.push_back(song);
                    }
                }

                if (randomPlaylist)
                    std::random_shuffle(songs.begin(), songs.end());

                for (Song s : songs)
                {
                    QListWidgetItem *listItem = new QListWidgetItem(s.title);
                    listItem->setToolTip(s.path);
                    ui->listWidget_songs->addItem(listItem);

                    musicPlaylist->addMedia(QUrl::fromLocalFile(s.path));
                }
            }
            else
                hasSongs = false;

            settings.endArray();
        }
    }
    settings.endArray();

    if (hasSongs)
    {
        // Set playlist, volume and play
        musicPlayer->setPlaylist(musicPlaylist);
        musicPlayer->setVolume(ui->horizontalSlider_music->value());

        // If random mode is active, start with a random song
        if (randomPlayback) {
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Random);
            musicPlaylist->next();
        } else if (loop || randomPlaylist) {
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
        } else if (sequential) {
            musicPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);
        }
        musicPlayer->play();
    }
    else
        qDebug() << "Error: Music list does have any songs in it!";
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
            qDebug().noquote() << "Stopping sound list: " + soundList + " ...";

            checked = true;
            sPlayer->stop();
            soundPlayerList.removeOne(sPlayer);
            delete sPlayer;
        }
    }

    if (!checked)
    {
        qDebug().noquote() << "Playing sound list: " + soundList + " ...";

        QMediaPlayer *player = new QMediaPlayer;
        QMediaPlaylist *playlist = new QMediaPlaylist;

        player->setPlaylist(playlist);
        soundPlayerList.push_back(player);

        player->setObjectName(soundList);

        bool random = true;
        bool loop = false;
        bool sequential = false;

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
                        playlist->addMedia(QUrl::fromLocalFile(path));
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

    qDebug().noquote() << "Playing radio: " + radio + " ...";

    // Stop music and clear song list
    musicPlayer->stop();
    ui->listWidget_songs->clear();
    radioActive = true;

    QSettings settings(settingsManager->getSetting(audioPath)+"/"+currentProject, QSettings::IniFormat);
    int radios = settings.beginReadArray(category+"_"+scenario+"_Radios");

    // Get Radio URL
    QString url;
    for (int i = 0; i<radios; i++)
    {
        settings.setArrayIndex(i);

        if (settings.value("name").toString() == radio)
            url = settings.value("url").toString();
    }
    qDebug().noquote() << "URL: " +url;
    settings.endArray();

    // If Radio was added via a .m3u or .pls Playlist
    if (url.contains(".m3u") || url.contains(".pls"))
    {
        QMediaPlaylist *radioPlaylist = new QMediaPlaylist;
        radioPlayer->setPlaylist(radioPlaylist);

        // Determine if radio is a local playlist or a URL to a playlist
        if (QFile(settingsManager->getSetting(radioPath)+url).exists())
            radioPlaylist->load(QUrl::fromLocalFile(settingsManager->getSetting(radioPath)+url));
        else
            radioPlaylist->load(QUrl(url));
    }
    else // Radio is only a URL
    {
        radioPlayer->setMedia(QUrl(url));
    }

    radioPlayer->setVolume(ui->horizontalSlider_music->value());
    radioPlayer->play();
}

void AudioTool::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_MediaPlay:
    {
        if (musicPlayer->state() == QMediaPlayer::PlayingState || radioPlayer->state() == QMediaPlayer::PlayingState)
            on_pushButton_pause_clicked();
        else if (musicPlayer->state() == QMediaPlayer::PausedState || radioPlayer->state() == QMediaPlayer::PausedState)
            on_pushButton_play_clicked();
        break;
    }
    case Qt::Key_MediaNext:
    {
        on_pushButton_next_clicked();
        break;
    }
    case Qt::Key_MediaPrevious:
    {
        on_pushButton_replay_clicked();
        break;
    }
    default:
        break;
    }
}

// Play
void AudioTool::on_pushButton_play_clicked()
{
    if (radioActive)
        radioPlayer->play();
    else
        musicPlayer->play();
}

// Pause
void AudioTool::on_pushButton_pause_clicked()
{
    if (radioActive)
        radioPlayer->pause();
    else
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

// Adjust music and radio volume
void AudioTool::on_horizontalSlider_music_valueChanged(int value)
{
    musicPlayer->setVolume(value);
    radioPlayer->setVolume(value);
}

// Adjust sound volume
void AudioTool::on_horizontalSlider_sound_valueChanged(int value)
{
    for (QMediaPlayer *player : soundPlayerList)
        player->setVolume(value);
}

// When a category is selected
void AudioTool::changeCategory(QString category)
{
    currentCategory = category;
    qDeleteAll(ui->scrollAreaWidgetContents->children());
    generateScenarioList(category);
}

// When a scenario is selected
void AudioTool::on_listWidget_scenarios_currentRowChanged(int currentRow)
{
    if (currentRow > -1)
        generateElementButtons(ui->listWidget_scenarios->currentItem()->text());
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
        settings.setValue("defaultProject", ui->comboBox_projects->currentText());
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
        ui->checkBox_setProjectAsDefault->setChecked(true);
    else
        ui->checkBox_setProjectAsDefault->setChecked(false);

    settings.endGroup();
}

// When a song is selected in the song list view, play it
void AudioTool::on_listWidget_songs_currentRowChanged(int currentRow)
{
    if (currentRow >= 0)
        musicPlaylist->setCurrentIndex(currentRow);
}

// Update Projects List
void AudioTool::on_pushButton_updateProjects_clicked()
{
    getProjects();
}

// Open audio tool wiki page
void AudioTool::on_pushButton_documentation_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/wiki/Audio-Tool"));
}

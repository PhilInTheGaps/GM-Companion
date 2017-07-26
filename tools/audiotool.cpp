#include "audiotool.h"
#include "ui_audiotool.h"
#include "functions.h"
#include "flowlayout.h"
#include "editors/audioeditor.h"

#include <QDebug>
#include <QScrollArea>
#include <QToolButton>

AudioTool::AudioTool(SettingsManager *sManager, QWidget *parent) : QWidget(parent), ui(new Ui::AudioTool)
{
    ui->setupUi(this);

    settingsManager = sManager;

    musicPlayer = new QMediaPlayer;
    connect(musicPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(updateMetaData()));

    musicPlaylist = new QMediaPlaylist;
    musicPlaylist->setPlaybackMode(QMediaPlaylist::Random);

    signalMapperMusic = new QSignalMapper;
    connect(signalMapperMusic, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));

//    QStringList categories = getCategories();

//    generateSmallButtons(categories);

//    generateCategoryList(categories);

    getProjects();

    FlowLayout *elementLayout = new FlowLayout;
    ui->frame_elements->setLayout(elementLayout);
}

AudioTool::~AudioTool()
{
    delete ui;
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
void AudioTool::playMusic(QString folder)
{
    // Get foldername from path
    QString folderName = folder;
    int index = folderName.lastIndexOf("/")+1;
    folderName = cleanText(folderName.mid(index));

    // Get category name from path
    QString category = folder;
    index = category.lastIndexOf("/", index-2);
    category = category.mid(index);

    // Clear Playlist
    musicPlaylist->clear();
    ui->comboBox_music->clear();

    // Add music files to playlist and ListView
    QStringList files = getFiles(folder);
    int row = 0;
    for (QString file : files){
        if (file.contains(".mp3") || file.contains(".wav")){
//            QTableWidgetItem *i = new QTableWidgetItem;
//            i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
//            i->setText(cleanText(file).replace(folderName, ""));
//            musicTable->setItem(row, 0, i);

            ui->comboBox_music->addItem(cleanText(file).replace(folderName, ""));

            musicPlaylist->addMedia(QUrl::fromLocalFile(folder+"/"+file));
            row++;
        }
    }

//    // Adjust ListView Column Size
//    musicTable->setRowCount(files.size());
//    if (files.size() > 9){
//        musicTable->setColumnWidth(0, 220);
//    }
//    else{
//        musicTable->setColumnWidth(0, 230);
//    }

    // Set playlist, volume and play
    musicPlayer->setPlaylist(musicPlaylist);
    musicPlayer->setVolume(ui->horizontalSlider_music->value());
    musicPlayer->play();

    // If random mode is active, start with a random song
    if (ui->pushButton_shuffle->isChecked()){
        musicPlaylist->next();
    }
}

// Display the metadata of the currently playing song
void AudioTool::updateMetaData()
{
    ui->comboBox_music->setCurrentIndex(musicPlaylist->currentIndex());
}

// Get a list of all music categories
QStringList AudioTool::getCategories()
{
    QStringList musicCategories = getFolders(settingsManager->getSetting(Setting::musicPath));

    return musicCategories;
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

// Generate the szenario list
void AudioTool::generateScenarioList(QString category)
{
    ui->listWidget_scenarios->clear();

    for (QString scenario : getFolders(settingsManager->getSetting(musicPath)+"/"+category))
    {
        if (!scenario.contains("."))
        {
            ui->listWidget_scenarios->addItem(cleanText(scenario));
            ui->listWidget_scenarios->item(ui->listWidget_scenarios->count()-1)->setToolTip(scenario);
        }
    }
}

// Generate the buttons for all the elements of a scenario
void AudioTool::generateElementButtons(QString scenario)
{
    QString category = ui->listWidget_categories->currentItem()->toolTip();

    qDebug() << "Removing old Elements...";
//    QLayoutItem *item;
//    while ((item = ui->frame_elements->layout()->takeAt(0)) != 0)
//    {
//        ui->frame_elements->layout()->removeItem(item);
//        delete item;
//    }
    qDeleteAll(ui->frame_elements->children());

    FlowLayout *layout = new FlowLayout;
    ui->frame_elements->setLayout(layout);

    qDebug() << "Adding new Elements...";
    qDebug() << settingsManager->getSetting(musicPath)+"/"+category+"/"+scenario;
    for (QString element : getFolders(settingsManager->getSetting(musicPath)+"/"+category+"/"+scenario))
    {
        if (!element.contains("."))
        {
            qDebug() << element;

            QPushButton *button = new QPushButton(element);

            ui->frame_elements->layout()->addWidget(button);
        }
    }
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

// Shuffle
void AudioTool::on_pushButton_shuffle_clicked()
{
    if (ui->pushButton_shuffle->isChecked())
    {
        musicPlaylist->setPlaybackMode(QMediaPlaylist::Random);
    }

    else
    {
        musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

// Adjust music volume
void AudioTool::on_horizontalSlider_music_valueChanged(int value)
{
    musicPlayer->setVolume(value);
}

void AudioTool::on_horizontalSlider_sound_valueChanged(int value)
{
    // TODO
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
        generateScenarioList(ui->listWidget_categories->currentItem()->toolTip());
    }
}

// When a scenario is selected
void AudioTool::on_listWidget_scenarios_currentRowChanged(int currentRow)
{
    if (currentRow > -1)
        generateElementButtons(ui->listWidget_scenarios->currentItem()->toolTip());
}

// Open Audio Editor
void AudioTool::on_pushButton_openEditor_clicked()
{
    AudioEditor *audioEditor = new AudioEditor;
    audioEditor->showMaximized();
}

// Load a project
void AudioTool::loadProject()
{
    getCategories();
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
    loadProject();
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

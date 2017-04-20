#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flowlayout.h"
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QMediaPlayer>
#include <QStringList>
#include <QDir>
#include <QPushButton>
#include <QIcon>
#include <QImage>
#include <QSize>
#include <QFile>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QToolBox>
#include <QMediaMetaData>
#include <QMediaObject>
#include <QPixmap>
#include <cstdlib>
#include <QScrollArea>

// Gets all folders in directory
QStringList getFolders(QString path){

    QDir myDir(path);
    myDir.setFilter(QDir::Dirs);
    QStringList folders = myDir.entryList();

    return folders;
}

// Gets all files in directory
QStringList getFiles(QString folder){
    QDir myDir(folder);
    myDir.setFilter(QDir::Files);
    QStringList files = myDir.entryList(); //QDir::Files
    return files;
}

// Replaces all undescores with whitespaces and removes .mp3
QString cleanText(QString text){
    text.replace("_", " ");
    text.replace(".mp3", "");
    text.replace(".wav", "");
    text.replace(".png", "");
    text.replace(".jpg", "");
    return text;
}

// Gets a specific setting from settings.txt
QString getSettings(QString indicator){
    QString setting = "";
    QFile file(QDir::currentPath()+"/settings.txt");

    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString content = stream.readAll();
    int begin = content.indexOf(indicator);
    if (begin > -1){
        int argStart = content.indexOf("=", begin)+1;
        int argEnd = content.indexOf(";", begin);
        int argLength = argEnd-argStart;
        setting = content.mid(argStart, argLength);
    }
    file.close();

    return setting;
}


// Writes a setting to settings.txt
void MainWindow::writeSettings(QString content, QString indicator, QString newSetting){
    QFile file(QDir::currentPath()+"/settings.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream output(&file);

    int i1 = content.indexOf(indicator);
    if (i1 > -1){
        int i2 = content.indexOf(";", i1+1);
        content.remove(i1, i2-i1);

        content.insert(i1, indicator+newSetting);
    }
    else{
        content.append(indicator+newSetting+";"+"\n");
    }

    output << content;
    file.close();
}

// Reads all the settings as one string
QString MainWindow::readSettings(){
    QFile file(QDir::currentPath()+"/settings.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);
    QString content = textStream.readAll();
    file.close();
    return content;
}

// Opens a Directory Chooser to set the new folder location
void MainWindow::setFolderLocations(QString indicator){
    QString path;
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle("Open Files");
    if (fileDialog->exec() == QDialog::Accepted){
        QStringList paths = fileDialog->selectedFiles();
        path = paths.at(0);
    }
    writeSettings(readSettings(), indicator, path);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    // Sets the paths
    musicPath = getSettings("MUSIC_PATH");
    soundPath = getSettings("SOUND_PATH");
    mapsPath = getSettings("MAPS_PATH");
    resourcesPath = getSettings("RESOURCES_PATH");

    signalMapperMusic = new QSignalMapper(this);
    signalMapperSound = new QSignalMapper(this);
    signalMapperMaps = new QSignalMapper(this);

    // Sets player and playlist for music
    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setVolume(ui->musicVolumeSlider->value());
    metaPlayer = new QMediaPlayer;
    musicPlaylist = new QMediaPlaylist();
    musicPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random); //QMediaPlaylist::PlaybackMode::Loop|
    musicPlayer->setPlaylist(musicPlaylist);

    // Sets player and playlist for sound
    soundPlayer = new QMediaPlayer(this);
    soundPlayer->setVolume(ui->soundVolumeSlider->value());
    soundPlaylist = new QMediaPlaylist();
    soundPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random); //QMediaPlaylist::PlaybackMode::Loop|
    soundPlayer->setPlaylist(soundPlaylist);

    connect(musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updateProgressBar()));
    connect(musicPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(updateMetaData()));

    // Connects the Menu signals with the according event slots
    connect(ui->actionSet_Music_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setMusicFolder_clicked()));
    connect(ui->actionSet_Sound_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setSoundFolder_clicked()));
    connect(ui->actionSet_Maps_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setMapsFolder_clicked()));
    connect(ui->actionSet_Resources_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setResourcesFolder_clicked()));

    // Creates Tab for music
    tabMusicLayout = new QHBoxLayout; // Layout inside the music tab
    ui->tabMusic->setLayout(tabMusicLayout);

    // Creates TableWidget to display song titles
    initialMusicPlay = true;
    musicTable = new QTableWidget;
    musicTable->setMaximumWidth(250);
    musicTable->setColumnWidth(0, 250);
    musicTable->setColumnCount(1);
    musicTable->setHorizontalHeaderLabels(QString("Title").split(";"));
    connect(musicTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_tableDoubleClicked(int,int)));

    QToolBox *toolBoxMusic = new QToolBox; // ToolBox for the different Categories
    tabMusicLayout->addWidget(toolBoxMusic);

    QStringList musicCategories = getFolders(musicPath); // List of all categories
    for (QString folder : musicCategories){

        if (!folder.contains(".")){
            QFrame *frame = new QFrame;
            toolBoxMusic->addItem(frame, cleanText(folder));

            if (QFile(resourcesPath+"/Backgrounds/"+folder+".png").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+resourcesPath+"/Backgrounds/"+folder+".png);}");
            }
            else if (QFile(resourcesPath+"/Backgrounds/"+folder+".jpg").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+resourcesPath+"/Backgrounds/"+folder+".jpg);}");
            }


            // Generating musicButtons
            QString path = musicPath+"/"+folder; //"C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/OSTs"
            QStringList musicFolders = getFolders(path);

            FlowLayout *flowLayoutMusic = new FlowLayout;
            frame->setLayout(flowLayoutMusic);

            for (QString s : musicFolders){
                if (!s.contains(".")){
                    QPushButton *b = new QPushButton(); //cleanText(s)
                    b->setStyleSheet("min-width: 155; min-height: 155; padding: 1px; border-radius: 2px");
                    connect(b, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
                    signalMapperMusic->setMapping(b, musicPath+"/"+folder+"/"+s);

                    //Setting Icon
                    if (QFile(resourcesPath+"/Icons/Music/"+folder+"/"+s+".png").exists()){
                        QIcon *icon = new QIcon(resourcesPath+"/Icons/Music/"+folder+"/"+s+".png");
                        b->setIcon(*icon);
                        b->setIconSize(QSize(150, 150));
                    }
                    else if (QFile(resourcesPath+"/Icons/Music/"+folder+"/"+s+".jpg").exists()){
                        QIcon *icon = new QIcon(resourcesPath+"/Icons/Music/"+folder+"/"+s+".jpg");
                        b->setIcon(*icon);
                        b->setIconSize(QSize(150, 150));
                    }
                    else{
                        b->setText(cleanText(s));
                    }

                    b->setFixedSize(155, 155);
                    b->setToolTip(cleanText(s));

                    flowLayoutMusic->addWidget(b);
                }
            }
        }  
    }

    //tabMusicLayout->addWidget(musicTable);
    connect(signalMapperMusic, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));

    // Creates Tab for sound
    QHBoxLayout *tabSoundLayout = new QHBoxLayout; // Layout inside the sound tab
    ui->tabSound->setLayout(tabSoundLayout);

    QToolBox *toolBoxSound = new QToolBox; // ToolBox for the different Categories
    tabSoundLayout->addWidget(toolBoxSound);

    QStringList soundCategories = getFolders(soundPath); // List of all categories
    for (QString folder : soundCategories){

        if (!folder.contains(".")){
            QFrame *frame = new QFrame;
            toolBoxSound->addItem(frame, cleanText(folder));

            // Generating musicButtons
            QString path = soundPath+"/"+folder;
            QStringList soundFolders = getFolders(path);

            FlowLayout *flowLayoutSound = new FlowLayout;
            frame->setLayout(flowLayoutSound);

            for (QString s : soundFolders){
                if (!s.contains(".")){
                    QPushButton *b = new QPushButton();
                    b->setStyleSheet("min-width: 155; min-height: 155; padding: 1px; border-radius: 2px");
                    connect(b, SIGNAL(clicked()), signalMapperSound, SLOT(map()));
                    signalMapperSound->setMapping(b, soundPath+"/"+folder+"/"+s);

                    //Setting Icon
                    if (QFile(resourcesPath+"/Icons/Sounds/"+folder+"/"+s+".png").exists()){
                        QIcon *icon = new QIcon(resourcesPath+"/Icons/Sounds/"+folder+"/"+s+".png");
                        b->setIcon(*icon);
                        b->setIconSize(QSize(150, 150));
                    }
                    else if (QFile(resourcesPath+"/Icons/Sounds/"+folder+"/"+s+".jpg").exists()){
                        QIcon *icon = new QIcon(resourcesPath+"/Icons/Sounds/"+folder+"/"+s+".jpg");
                        b->setIcon(*icon);
                        b->setIconSize(QSize(150, 150));
                    }
                    else{
                        b->setText(cleanText(s));
                    }

                    b->setFixedSize(155, 155);
                    b->setToolTip(cleanText(s));

                    flowLayoutSound->addWidget(b);
                }
            }
        }
    }

    connect(signalMapperSound, SIGNAL(mapped(QString)), this, SLOT(playSound(QString)));

    // Creates Tab for maps
    QStringList mapsList = getFiles(mapsPath);

    mapsImageLabel = new QLabel();
    mapsImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mapsImageLabel->setScaledContents(true);

    QScrollArea *mapsScrollArea = new QScrollArea;
    mapsScrollArea->setWidget(mapsImageLabel);

    FlowLayout *mapsFlowLayout = new FlowLayout;
    ui->mapsButtonFrame->setLayout(mapsFlowLayout);

    ui->mapsVBox->addWidget(mapsScrollArea);

    for (QString mapName : mapsList){
        if (mapName.contains(".png") || mapName.contains(".jpg")){
            QString mapPath = mapsPath+"/"+mapName;

            QPushButton *imageButton = new QPushButton;
            imageButton->setText(cleanText(mapName));
            mapsFlowLayout->addWidget(imageButton);

            connect(imageButton, SIGNAL(clicked()), signalMapperMaps, SLOT(map()));
            signalMapperMaps->setMapping(imageButton, mapPath);

//            mapsImageLabel->setPixmap(QPixmap(mapPath));
//            mapsImageLabel->adjustSize();

            ui->textEdit->append(mapPath+"\n");
        }
    }

    connect(signalMapperMaps, SIGNAL(mapped(QString)), this, SLOT(setMap(QString)));

}

void MainWindow::setMap(QString mapPath){
    ui->textEdit->append("Map: "+mapPath+"\n");
    mapsImageLabel->setPixmap(QPixmap(mapPath));
    mapsImageLabel->adjustSize();
}

void MainWindow::addToPlaylist(QUrl url, bool music){
    if (music){
        musicPlaylist->addMedia(url);
    }
    else{
        soundPlaylist->addMedia(url);
    }
}

QStringList shuffleStringList(QStringList list){
    for (int i = 0; i < list.size()*5; i++){
        int  index1 = rand() % list.size();
        int  index2 = rand() % list.size();
        list.swap(index1, index2);
    }
    return list;
}

void MainWindow::playSound(QString folder){
    QString folderName = folder;
    int index = folderName.lastIndexOf("/")+1;
    folderName = cleanText(folderName.mid(index));

    QStringList files = getFiles(folder);
    files = shuffleStringList(files);


    soundPlaylist->clear();

    for (QString file : files){
        if (file.contains(".mp3") || file.contains(".wav")){
            addToPlaylist(QUrl::fromLocalFile(folder+"/"+file), false);
        }
    }

    soundPlayer->play();

    ui->soundNameLabel->setText("Folder: "+folderName);
}

void MainWindow::playMusic(QString folder){
    QString folderName = folder;
    int index = folderName.lastIndexOf("/")+1;
    folderName = cleanText(folderName.mid(index));
    ui->textEdit->append("Index: "+QString::number(index)+"\n");
    ui->textEdit->append("FolderName: "+folderName+"\n");

    QString category = folder;
    index = category.lastIndexOf("/", index-2);
    category = category.mid(index);
    ui->textEdit->append("Index: "+QString::number(index)+"\n");
    ui->textEdit->append("Category: "+category+"\n");

    QStringList files = getFiles(folder);
    files = shuffleStringList(files);

    if (initialMusicPlay){
        tabMusicLayout->addWidget(musicTable);
        initialMusicPlay = false;
    }

    musicTable->setRowCount(files.size());
    if (files.size() > 9){
        musicTable->setColumnWidth(0, 220);
    }
    else{
        musicTable->setColumnWidth(0, 230);
    }
    musicPlaylist->clear();

    int row = 0;
    for (QString file : files){
        if (file.contains(".mp3") || file.contains(".wav")){
            QTableWidgetItem *i = new QTableWidgetItem;
            i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            i->setText(cleanText(file).replace(folderName, ""));
            musicTable->setItem(row, 0, i);

            addToPlaylist(QUrl::fromLocalFile(folder+"/"+file), true);
            ui->textEdit->append(file + "\n");
            row++;
        }
    }

    musicPlayer->play();

    //Setting Image
    if (QFile(resourcesPath+"/Icons/Music"+category+".png").exists()){
        //QPixmap *cover = new QPixmap(resourcesPath+"/Icons/Music"+category+".png");
        ui->musicCoverLabel->setPixmap(QPixmap(resourcesPath+"/Icons/Music"+category+".png").scaledToWidth(300));
    }
    else if (QFile(resourcesPath+"/Icons/Music"+category+".jpg").exists()){
        //QPixmap *cover = new QPixmap(resourcesPath+"/Icons/Music"+category+".jpg");
        ui->musicCoverLabel->setPixmap(QPixmap(resourcesPath+"/Icons/Music"+category+".jpg").scaledToWidth(300));
    }

}

void MainWindow::updateProgressBar(){
    if (musicPlayer->duration() == 0){
        ui->musicProgressBar->setMaximum(1);
        ui->musicProgressBar->setValue(0);
    }
    else{
        ui->musicProgressBar->setMaximum(musicPlayer->duration());
        ui->musicProgressBar->setValue(musicPlayer->position());
    }
}

void MainWindow::updateMetaData(){
    QString title = musicPlayer->metaData(QStringLiteral("Title")).toString();
    QString album = musicPlayer->metaData(QStringLiteral("AlbumTitle")).toString();
    if (musicPlayer->isMetaDataAvailable()){
        ui->musicTitleLabel->setText("Title: "+title);
        ui->musicAlbumLabel->setText("Album: "+album);
        ui->musicArtistLabel->setText("Artist: "+musicPlayer->metaData(QStringLiteral("Author")).toString());
        ui->musicYearLabel->setText("Year: "+musicPlayer->metaData(QStringLiteral("Year")).toString());

        ui->musicCoverLabel->setToolTip(album+": "+title);

        QTableWidgetItem *i = new QTableWidgetItem;
        i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        i->setText(title);
        musicTable->setItem(musicPlaylist->currentIndex(), 0, i);
    }
    else{
        ui->musicTitleLabel->setText("Title: Unknown");
        ui->musicAlbumLabel->setText("Album: Unknown");
        ui->musicArtistLabel->setText("Artist: Unknown");
        ui->musicYearLabel->setText("Year: Unknown");
    }
    musicTable->selectRow(musicPlaylist->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableDoubleClicked(int row, int column){
    musicPlaylist->setCurrentIndex(row);
}

void MainWindow::on_musicPauseButton_clicked()
{
    musicPlayer->pause();
}

void MainWindow::on_musicPlayButton_clicked()
{
    musicPlayer->play();
}

void MainWindow::on_musicReplayButton_clicked()
{
    musicPlayer->setPosition(0);
}

void MainWindow::on_musicNextButton_clicked()
{
    musicPlaylist->next();
}

void MainWindow::on_setMusicFolder_clicked(){
    setFolderLocations("MUSIC_PATH=");
}

void MainWindow::on_setSoundFolder_clicked(){
    setFolderLocations("SOUND_PATH=");
}

void MainWindow::on_setMapsFolder_clicked(){
    setFolderLocations("MAPS_PATH=");
}

void MainWindow::on_setResourcesFolder_clicked(){
    setFolderLocations("RESOURCES_PATH=");
}

void MainWindow::on_soundPlayButton_clicked()
{
    soundPlayer->play();
}

void MainWindow::on_soundPauseButton_clicked()
{
    soundPlayer->pause();
}

void MainWindow::on_soundReplayButton_clicked()
{
    soundPlayer->setPosition(0);
}

void MainWindow::on_soundNextButton_clicked()
{
    soundPlaylist->next();
}

void MainWindow::on_musicVolumeSlider_valueChanged(int value)
{
    musicPlayer->setVolume(value);
}

void MainWindow::on_soundVolumeSlider_valueChanged(int value)
{
    soundPlayer->setVolume(value);
}

void MainWindow::on_mapsZoomInButton_clicked()
{
    double factor = 1.5 * mapsZoomFactor;
    mapsImageLabel->resize(factor * mapsImageLabel->pixmap()->size());
    mapsZoomFactor = factor;
}

void MainWindow::on_mapsZoomOutButton_clicked()
{
    double factor = 0.75 * mapsZoomFactor;
    mapsImageLabel->resize(factor * mapsImageLabel->pixmap()->size());
    mapsZoomFactor = factor;
}

void MainWindow::on_mapsFitToViewButton_clicked()
{
    double width = mapsImageLabel->width();
    double height = mapsImageLabel->height();
    double ratio = width/height;
    double factor = 1;
    if (ratio >=1){
        factor = ui->mapsButtonFrame->width() / width;
        mapsImageLabel->resize(factor * mapsImageLabel->size());
    }
    else{
        factor = ui->mapsControlFrame->height() / height;
        mapsImageLabel->resize(factor * mapsImageLabel->size());
    }
    mapsZoomFactor = mapsZoomFactor*factor;
}

void MainWindow::on_mapsResetSizeButton_clicked()
{
    mapsImageLabel->resize(mapsImageLabel->pixmap()->size());
    mapsZoomFactor = 1.0;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flowlayout.h"
#include "characters.h"
#include "functions.h"
#include "settingsmanager.h"
#include "dicemanager.h"
#include "chareditor.h"

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
#include <QDesktopServices>
#include <QLineEdit>
#include <QSpinBox>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSpacerItem>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QFileSystemWatcher>
#include <QtWinExtras>
#include <QDebug>
#include <QXmlStreamReader>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    setVersion("0.3.0.0");

    // Gets all settings
    settingsManager = new SettingsManager;
    int checkUpdates = settingsManager->getSetting(Setting::checkForUpdatesOnStart).toInt();

    signalMapperMusic = new QSignalMapper(this);
    signalMapperSound = new QSignalMapper(this);
    signalMapperMaps = new QSignalMapper(this);
    signalMapperNames = new QSignalMapper(this);

    // Generates the dice page
    diceManager = new DiceManager;
    ui->tabDice->layout()->addWidget(diceManager->generateDiceFrame());

    // Sets player and playlist for music
    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setVolume(ui->musicVolumeSlider->value());
    metaPlayer = new QMediaPlayer;
    musicPlaylist = new QMediaPlaylist();
    musicPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random); //QMediaPlaylist::PlaybackMode::Loop|
    musicPlayer->setPlaylist(musicPlaylist);
    metaPlaylist = new QMediaPlaylist();
    metaPlayer->setPlaylist(metaPlaylist);

    // Radio Stuff
    radioPlayer = new QMediaPlayer(this);
    radioPlayer->setVolume(ui->musicVolumeSlider->value());
    radioNetworkManager = new QNetworkAccessManager;
    connect(radioNetworkManager, SIGNAL(finished(QNetworkReply*)), SLOT(on_radioNetworkAccessManager_finished(QNetworkReply*)));
    radioTimer = new QTimer;
    radioTimerDuration = 10000; // Update Interval in Milliseconds
    radioTimer->setInterval(radioTimerDuration);
    connect(radioTimer, SIGNAL(timeout()), SLOT(on_radioTimer_timeout()));

    // Name Generator
    generateNamesTab();

    // Sets player and playlist for sound
    soundPlayer = new QMediaPlayer(this);
    soundPlayer->setVolume(ui->soundVolumeSlider->value());
    soundPlaylist = new QMediaPlaylist();
    soundPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random);
    soundPlayer->setPlaylist(soundPlaylist);

    connect(musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updateProgressBar()));
    connect(musicPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(updateMetaData()));
    connect(metaPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(on_metaPlayerGotMetadata()));
    connect(radioPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(on_radioMetaDataChanged()));

    // Connects the Menu signals with the according event slots
    connect(ui->actionSet_Music_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setMusicFolder_clicked()));
    connect(ui->actionSet_Sound_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setSoundFolder_clicked()));
    connect(ui->actionSet_Maps_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setMapsFolder_clicked()));
    connect(ui->actionSet_Resources_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setResourcesFolder_clicked()));
    connect(ui->actionSet_Characters_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setCharactersFolder_clicked()));
    connect(ui->actionCheck_for_Updates_on_Program_Start, SIGNAL(triggered(bool)), this, SLOT(on_checkForUpdatesOnStart(bool)));

    connect(ui->menuGM_Help, SIGNAL(triggered(QAction*)), this, SLOT(on_menuGM_Help_triggered()));
    connect(ui->menuMusic, SIGNAL(triggered(QAction*)), this, SLOT(on_menuMusic_triggered()));
    connect(ui->menuSound, SIGNAL(triggered(QAction*)), this, SLOT(on_menuSound_triggered()));
    connect(ui->menuMaps, SIGNAL(triggered(QAction*)), this, SLOT(on_menuMaps_triggered()));

    connect(ui->actionDice, SIGNAL(triggered(bool)), this, SLOT(on_actionDice_clicked()));
    connect(ui->actionDatabase, SIGNAL(triggered(bool)), this, SLOT(on_actionDatabase_clicked()));

    connect(ui->actionMusic_Player, SIGNAL(triggered(bool)), this, SLOT(on_actionMusicPlayer_clicked()));
    connect(ui->actionSound_Player, SIGNAL(triggered(bool)), this, SLOT(on_actionSoundPlayer_clicked()));
    connect(ui->actionMap_Viewer, SIGNAL(triggered(bool)), this, SLOT(on_actionMapsViewer_clicked()));

    connect(ui->actionOpen_Wiki, SIGNAL(triggered(bool)), this, SLOT(on_openWiki_clicked()));
    connect(ui->actionReport_a_Bug, SIGNAL(triggered(bool)), this, SLOT(on_reportABug_clicked()));
    connect(ui->actionI_want_to_use_an_older_Version, SIGNAL(triggered(bool)), this, SLOT(on_iWantToUseAnOlderVersionClicked()));
    connect(ui->actionCheck_for_Updates, SIGNAL(triggered(bool)), this, SLOT(on_checkForUpdates_clicked()));

    connect(ui->actionInternet_Radio, SIGNAL(triggered(bool)), this, SLOT(on_actionRadio_clicked()));
    connect(ui->actionNamne_Generator, SIGNAL(triggered(bool)), this, SLOT(on_actionNameGenerator_clicked()));
    connect(ui->actionCharacters, SIGNAL(triggered(bool)), this, SLOT(on_actionCharacters_clicked()));

    // Network Stuff
    qDebug() << "Support SSL:  " << QSslSocket::supportsSsl()
            << "\nLib Version Number: " << QSslSocket::sslLibraryVersionNumber()
            << "\nLib Version String: " << QSslSocket::sslLibraryVersionString()
            << "\nLib Build Version Number: " << QSslSocket::sslLibraryBuildVersionNumber()
            << "\nLib Build Version String: " << QSslSocket::sslLibraryBuildVersionString();

    versionNetworkManager = new QNetworkAccessManager;
    connect(versionNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_versionNetworkAccessManagerFinished(QNetworkReply*)));
    blogNetworkManager = new QNetworkAccessManager;
    connect(blogNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_blogNetworkAccessManagerFinished(QNetworkReply*)));

    // Creates Tab for music
    // Creates TableWidget to display song titles
    initialMusicPlay = true;
    musicTable = new QTableWidget;
    musicTable->setMaximumWidth(250);
    musicTable->setColumnWidth(0, 250);
    musicTable->setColumnCount(1);
    musicTable->setHorizontalHeaderLabels(QString("Title").split(";"));
    connect(musicTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_tableDoubleClicked(int)));

    generateMusicButtons();
    connect(signalMapperMusic, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));

    // Creates Tab for sound
    generateSoundButtons();
    connect(signalMapperSound, SIGNAL(mapped(QString)), this, SLOT(playSound(QString)));

    // Creates Tab for maps
    generateMaps();
    connect(signalMapperMaps, SIGNAL(mapped(QString)), this, SLOT(setMap(QString)));

    // Character View
    QStringList characterList = getCharacterList();
    if (!characterList.empty()){
        for (QString character : characterList){
            QWidget* widget = getCharacterPage(character);
            ui->charactersStackedWidget->addWidget(widget);

            QListWidgetItem *listItem = new QListWidgetItem;
            listItem->setText(widget->toolTip());
            listItem->setToolTip(cleanText(character));
            ui->charactersListWidget->addItem(listItem);
        }
        on_characterListClicked(0);
    }
    connect(ui->charactersListWidget, SIGNAL(currentRowChanged(int)), SLOT(on_characterListClicked(int)));

    // Characters File System Watcher Updates the list when file is added
    QFileSystemWatcher* charactersWatcher = new QFileSystemWatcher;
    charactersWatcher->addPath(QDir::currentPath()+"/characters");
    //connect(charactersWatcher, SIGNAL(directoryChanged(QString)), SLOT(charactersFolderChanged()));

    updateCharactersTimer = new QTimer;
    updateCharactersTimer->setInterval(3000);
    connect(updateCharactersTimer, SIGNAL(timeout()), SLOT(charactersTimerFinished()));

    // Checks for updates on program start
    qDebug() << "CheckUpdates:";
    qDebug() << checkUpdates;
    if (checkUpdates == 0){
        ui->actionCheck_for_Updates_on_Program_Start->setChecked(true);
        on_checkForUpdates_clicked();
    }

    // Get Blog Feed
    blogNetworkManager->get(QNetworkRequest(QUrl("https://philinthegaps.github.io/GM-Companion/feed.xml")));

    programStart = false;

}

void MainWindow::on_characterListClicked(int index){
    if (index >= 0){
        ui->charactersStackedWidget->setCurrentIndex(index);

        qDebug() << settingsManager->getSetting(Setting::charactersPath)<< "/" << ui->charactersListWidget->item(index)->toolTip() << ".png";
        qDebug() << ui->charactersListWidget->item(index)->toolTip();

        QStringList list = ui->charactersStackedWidget->currentWidget()->accessibleName().split(",");
        qDebug() << "ICON PATH: " << list.at(0);

        if (QFile(settingsManager->getSetting(Setting::charactersPath)+"/"+list.at(0)+".png").exists()){
            QPixmap charIcon(settingsManager->getSetting(Setting::charactersPath)+"/"+list.at(0)+".png");
            qDebug() << settingsManager->getSetting(Setting::charactersPath)+"/"+list.at(0)+".png";
            ui->characterIconLabel->setPixmap(charIcon.scaled(180, 180));
            ui->characterIconLabel->adjustSize();
        }
        else{
            ui->characterIconLabel->clear();
        }
    }
    qDebug() << QString::number(index);
}

void MainWindow::addToPlaylist(QUrl url, bool music){
    if (music){
        musicPlaylist->addMedia(url);
        //metaPlaylist->addMedia(url);
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
    //files = shuffleStringList(files);

    soundPlaylist->clear();

    for (QString file : files){
        if (file.contains(".mp3") || file.contains(".wav")){
            addToPlaylist(QUrl::fromLocalFile(folder+"/"+file), false);
        }
    }

    if (ui->soundRandomButton->isChecked()){
        soundPlaylist->next();
    }
    soundPlayer->play();

    ui->soundNameLabel->setText("Folder: "+folderName);
}

void MainWindow::playMusic(QString folder){
    radioTimer->stop();
    ui->musicNextButton->setEnabled(true);
    ui->musicReplayButton->setEnabled(true);
    ui->musicRandomButton->setEnabled(true);

    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier) &&!initialMusicPlay && radioActive == false)
    {
        qDebug() << "SHIFT IS PRESSED";

        QString folderName = folder;
        int index = folderName.lastIndexOf("/")+1;
        folderName = cleanText(folderName.mid(index));
        qDebug() << "Index: " << QString::number(index);
        qDebug() << "FolderName: " << folderName;

        QString category = folder;
        index = category.lastIndexOf("/", index-2);
        category = category.mid(index);
        qDebug() << "Index: " << QString::number(index);
        qDebug() << "Category: " << category;

        QStringList files = getFiles(folder);
        //files = shuffleStringList(files);

        for (int i = 0; i < files.size(); i++){
            if (!files.at(i).contains(".mp3") && !files.at(i).contains(".wav")){
                files.removeAt(i);
            }
        }

//        int row = musicTable->rowCount();
//        musicTable->setRowCount(musicTable->rowCount()+files.size());

        if (musicTable->rowCount() > 9){
            musicTable->setColumnWidth(0, 220);
        }
        else{
            musicTable->setColumnWidth(0, 230);
        }

        for (QString file : files){
            if (file.contains(".mp3") || file.contains(".wav")){
                QTableWidgetItem *i = new QTableWidgetItem;
                i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                i->setText(cleanText(file).replace(folderName, ""));
                musicTable->insertRow(musicTable->rowCount());
                musicTable->setItem(musicTable->rowCount()-1, 0, i);

                addToPlaylist(QUrl::fromLocalFile(folder+"/"+file), true);
                qDebug() << file;
            }
        }

        musicPlayer->play();
    }
    else{
        radioActive = false;
        radioPlayer->stop();

        if (initialMusicPlay){
            ui->pageMusic->layout()->addWidget(musicTable);
            initialMusicPlay = false;
        }

        QString folderName = folder;
        int index = folderName.lastIndexOf("/")+1;
        folderName = cleanText(folderName.mid(index));
        qDebug() << "Index: "+QString::number(index);
        qDebug() << "FolderName: "+folderName;

        QString category = folder;
        index = category.lastIndexOf("/", index-2);
        category = category.mid(index);
        qDebug() << "Index: "+QString::number(index);
        qDebug() << "Category: "+category;

        QStringList files = getFiles(folder);
        //files = shuffleStringList(files);

        for (int i = 0; i < files.size(); i++){
            if (!files.at(i).contains(".mp3") && !files.at(i).contains(".wav")){
                files.removeAt(i);
            }
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
                qDebug() << file;
                row++;
            }
        }

        if (ui->musicRandomButton->isChecked()){
            musicPlaylist->next();
        }
        musicPlayer->play();

        //Setting Image
        if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".png").exists()){
            //QPixmap *cover = new QPixmap(resourcesPath+"/Icons/Music"+category+".png");
            ui->musicCoverLabel->setPixmap(QPixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".png").scaledToWidth(ui->musicCoverLabel->width()));
        }
        else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".jpg").exists()){
            //QPixmap *cover = new QPixmap(resourcesPath+"/Icons/Music"+category+".jpg");
            ui->musicCoverLabel->setPixmap(QPixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".jpg").scaledToWidth(ui->musicCoverLabel->width()));
        }
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

void MainWindow::on_metaPlayerGotMetadata(){
}

void MainWindow::updateMetaData(){
    qDebug() << "Average Audio Level: " << musicPlayer->metaData("AverageLevel").toString();
    qDebug() << "Cover Image: " << musicPlayer->metaData("CoverArtImage").toString();
    qDebug() << "Cover Image URL Small: " << musicPlayer->metaData("CoverArtUrlSmall").toString();
    qDebug() << "Cover Image URL Large: " << musicPlayer->metaData("CoverArtUrlLarge").toString();
    qDebug() << "Mood: " << musicPlayer->metaData("Mood").toString();
    qDebug() << "Title: " << musicPlayer->metaData("Title").toString();
    qDebug() << "Album: " << musicPlayer->metaData("AlbumTitle").toString();
    qDebug() << "Artist: " << musicPlayer->metaData("Author").toString();
    qDebug() << "Year: " << musicPlayer->metaData("Year").toString();

    QString title = musicPlayer->metaData(QStringLiteral("Title")).toString();
    QString album = musicPlayer->metaData(QStringLiteral("AlbumTitle")).toString();
    if (musicPlayer->isMetaDataAvailable()){
        ui->musicTitleLabel->setText("Title: "+title);
        ui->musicAlbumLabel->setText("Album: "+album);
        ui->musicArtistLabel->setText("Artist: "+musicPlayer->metaData(QStringLiteral("Author")).toString());

        ui->musicCoverLabel->setToolTip(album+": "+title);

        QString musicInfo;
        musicInfo.append(title+", "+album);
        ui->musicLabel->setText("Music");

        QTableWidgetItem *i = new QTableWidgetItem;
        i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        i->setText(title);
        musicTable->setItem(musicPlaylist->currentIndex(), 0, i);
    }
    else{
        ui->musicTitleLabel->setText("");
        ui->musicAlbumLabel->setText("");
        ui->musicArtistLabel->setText("");
    }
    musicTable->selectRow(musicPlaylist->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openWiki_clicked(){
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/wiki"));
}

void MainWindow::setVersion(QString versionAsString){
    versionString = versionAsString;
    versionNumber = 0;
    QString temp = versionAsString.replace(".", "");
    versionNumber = temp.toInt();

    qDebug() << "Version Number: "+QString::number(versionNumber);

}

void MainWindow::on_checkForUpdates_clicked(){
    versionNetworkManager->get(QNetworkRequest(QUrl("https://philinthegaps.github.io/GM-Companion/version.html")));
}

void MainWindow::on_blogNetworkAccessManagerFinished(QNetworkReply * reply){
    QString replyString = reply->readAll();
    qDebug() << replyString;

    QXmlStreamReader reader(replyString);

    if (reader.readNextStartElement()) {
            if (reader.name() == "feed"){
                while(reader.readNextStartElement()){
                    if(reader.name() == "entry"){
                        while(reader.readNextStartElement()){
                            if (reader.name() == "title"){
                                ui->blogTextEdit->append("<h1>"+reader.readElementText()+"</h1>");
                            }
                            else if (reader.name() == "updated"){
                                QString s = reader.readElementText();
                                QString date = s.left(s.indexOf("T"));
                                QDate d = QDate::fromString(date, "yyyy-MM-dd");

                                ui->blogTextEdit->append("(Last Update: "+d.longMonthName(d.month())+" "+QString::number(d.day())+" "+QString::number(d.year())+")");
                            }
                            else if (reader.name() == "content"){
                                ui->blogTextEdit->append(reader.readElementText());
                                ui->blogTextEdit->append(" ");
                            }
                            else{
                                reader.skipCurrentElement();
                            }
                        }
                    }
                    else
                        reader.skipCurrentElement();
                }
            }
            else
                reader.raiseError(QObject::tr("Incorrect file"));
    }

    //ui->blogTextEdit->setText(replyString);

}

void MainWindow::on_versionNetworkAccessManagerFinished(QNetworkReply* reply){
    QString replyString = reply->readAll();
    qDebug() << replyString;

    int index1 = replyString.indexOf("VERSION=");
    int index2 = replyString.indexOf(";", index1);

    int onlineVersion = 0;

    QString onlineVersionString = replyString.mid(index1+8, index2-index1-8);
    QString onlineVersionWithDots = onlineVersionString;
    QString temp = onlineVersionString.replace(".", "");
    onlineVersion = temp.toInt();
    qDebug() << "Most Current Version Number: "+QString::number(onlineVersion);

    if (versionNumber < onlineVersion){
        QFrame *dialogFrame = new QFrame;
        dialogFrame->setWindowTitle("Update Available!");
        dialogFrame->setMinimumSize(400, 50);
        QVBoxLayout *dialogLayout = new QVBoxLayout;
        dialogFrame->setLayout(dialogLayout);
        QLabel *l1 = new QLabel;
        l1->setText("A new version has been found:");
        QLabel *l2 = new QLabel;
        l2->setText(onlineVersionWithDots);

        QFrame *btnFrame = new QFrame;
        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnFrame->setLayout(btnLayout);

        QPushButton *openButton = new QPushButton;
        openButton->setText("Open Download Page");
        QPushButton *closeButton = new QPushButton;
        closeButton->setText("Close");
        btnLayout->addWidget(openButton);
        btnLayout->addWidget(closeButton);

        dialogLayout->addWidget(l1);
        dialogLayout->addWidget(l2);
        dialogLayout->addWidget(btnFrame);

        connect(openButton, SIGNAL(clicked(bool)), SLOT(on_iWantToUseAnOlderVersionClicked()));
        connect(closeButton, SIGNAL(clicked(bool)), dialogFrame, SLOT(close()));

        dialogFrame->show();
    }

}

void MainWindow::on_reportABug_clicked(){
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/issues/new"));
}

void MainWindow::on_iWantToUseAnOlderVersionClicked(){
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/releases"));
}

void MainWindow::on_tableDoubleClicked(int row){
    musicPlaylist->setCurrentIndex(row);
}

void MainWindow::on_musicPauseButton_clicked()
{
    if (radioActive){
        radioPlayer->pause();
        radioTimer->stop();
    }
    else{
        musicPlayer->pause();
    }
}

void MainWindow::on_musicPlayButton_clicked()
{
    if (radioActive){
        radioPlayer->play();
        radioTimer->start();
    }
    else{
        musicPlayer->play();
    }
}

void MainWindow::on_musicReplayButton_clicked()
{
    musicPlayer->setPosition(0);
}

void MainWindow::on_musicNextButton_clicked()
{
    if (!musicPlaylist->isEmpty()){
        musicPlaylist->next();
    }
}

void MainWindow::on_setMusicFolder_clicked(){
    settingsManager->setSetting(Setting::musicPath, true);
    QLayoutItem *child;
    while ((child = ui->pageMusic->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    musicPlayer->stop();
    musicPlaylist->clear();
    tabWidgetMusic = NULL;
    initialMusicPlay = true;
    generateMusicButtons();
}

void MainWindow::on_setSoundFolder_clicked(){
    settingsManager->setSetting(Setting::soundPath, true);
    QLayoutItem *child;
    while ((child = ui->pageSound->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    soundPlayer->stop();
    soundPlaylist->clear();
    tabWidgetSound = NULL;
    generateSoundButtons();
}

void MainWindow::on_setMapsFolder_clicked(){
    settingsManager->setSetting(Setting::mapsPath, true);
    QLayoutItem *child;
    while ((child = ui->mapsVBox->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    generateMaps();
}

void MainWindow::on_setCharactersFolder_clicked(){
    settingsManager->setSetting(Setting::charactersPath, true);
    updateCharacters();
}

void MainWindow::on_setResourcesFolder_clicked(){
    settingsManager->setSetting(Setting::resourcesPath, true);

    QLayoutItem *child;
    while ((child = ui->pageSound->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    soundPlayer->stop();
    soundPlaylist->clear();
    tabWidgetSound = NULL;
    generateSoundButtons();

    while ((child = ui->pageMusic->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    musicPlayer->stop();
    musicPlaylist->clear();
    tabWidgetMusic = NULL;
    initialMusicPlay = true;
    generateMusicButtons();

}

void MainWindow::on_checkForUpdatesOnStart(bool checked){
    if (checked){
        settingsManager->setSetting(Setting::checkForUpdatesOnStart, 0);
    }else{
        settingsManager->setSetting(Setting::checkForUpdatesOnStart, 1);
    }
}

void MainWindow::on_menuGM_Help_triggered(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionDice_clicked(){
    ui->tabWidgetGMHelp->setCurrentIndex(0);
}

void MainWindow::on_actionNameGenerator_clicked(){
    ui->tabWidgetGMHelp->setCurrentIndex(1);
}

void MainWindow::on_actionCharacters_clicked(){
    ui->tabWidgetGMHelp->setCurrentIndex(2);
}

void MainWindow::on_actionDatabase_clicked(){

}

void MainWindow::on_menuMusic_triggered(){
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionMusicPlayer_clicked(){

}

void MainWindow::on_menuSound_triggered(){
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionSoundPlayer_clicked(){

}

void MainWindow::on_menuMaps_triggered(){
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_actionMapsViewer_clicked(){

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
    if (!soundPlaylist->isEmpty()){
        soundPlaylist->next();
    }
}

void MainWindow::on_musicVolumeSlider_valueChanged(int value)
{
    musicPlayer->setVolume(value);
    radioPlayer->setVolume(value);
}

void MainWindow::on_soundVolumeSlider_valueChanged(int value)
{
    soundPlayer->setVolume(value);
}

void MainWindow::on_mapsZoomInButton_clicked()
{
    if (mapsImageLabel->pixmap() != 0){
        double factor = 1.5 * mapsZoomFactor;
        mapsImageLabel->resize(factor * mapsImageLabel->pixmap()->size());
        mapsZoomFactor = factor;
    }
}

void MainWindow::on_mapsZoomOutButton_clicked()
{
    if (mapsImageLabel->pixmap() != 0){
        double factor = 0.75 * mapsZoomFactor;
        mapsImageLabel->resize(factor * mapsImageLabel->pixmap()->size());
        mapsZoomFactor = factor;
    }
}

void MainWindow::on_mapsFitToViewButton_clicked()
{
    if (mapsImageLabel->pixmap() != 0){
        double width = mapsImageLabel->width();
        double height = mapsImageLabel->height();
        double ratio = width/height;
        double factor = 1;
        if (ratio >=1){
            factor = ui->mapsVBox->geometry().width() / width;
            mapsImageLabel->resize(factor * mapsImageLabel->size());
        }
        else{
            factor = ui->mapsControlFrame->height() / height;
            mapsImageLabel->resize(factor * mapsImageLabel->size());
        }
        mapsZoomFactor = mapsZoomFactor*factor;
    }
}

void MainWindow::on_mapsResetSizeButton_clicked()
{
    if (mapsImageLabel->pixmap() != 0){
        mapsImageLabel->resize(mapsImageLabel->pixmap()->size());
        mapsZoomFactor = 1.0;
    }
}

void MainWindow::on_createCharacterButton_clicked()
{

    CharEditor* charEditor = new CharEditor;
    charEditor->showMaximized();
}

void MainWindow::updateCharacters(){
    for(int i = ui->charactersStackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = ui->charactersStackedWidget->widget(i);
        ui->charactersStackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    ui->charactersListWidget->clear();

    QStringList characterList = getCharacterList();
    qDebug() << "Character Files after passing: " << characterList.size();
    if (!characterList.empty()){
        for (QString character : characterList){
            QWidget* widget = getCharacterPage(character);
            ui->charactersStackedWidget->addWidget(widget);

            QListWidgetItem *listItem = new QListWidgetItem;
            listItem->setText(widget->toolTip());
            listItem->setToolTip(cleanText(character));
            ui->charactersListWidget->addItem(listItem);
        }
        on_characterListClicked(0);
    }
}

void MainWindow::on_updateCharactersButton_clicked()
{
    updateCharacters();
}

void MainWindow::charactersFolderChanged(){
    if (listenForCharacterUpdaters){
        listenForCharacterUpdaters = false;
        qDebug() << "CHANGE DETECTED!!!";
        updateCharactersTimer->start();
    }
}

void MainWindow::charactersTimerFinished(){
    updateCharacters();
    listenForCharacterUpdaters = true;
}

void MainWindow::on_pushButton_clicked()
{
    int index = ui->charactersListWidget->currentRow();
    if (index >= 0){
        CharEditor* charEditor = new CharEditor;
        charEditor->load(index);

        charEditor->showMaximized();
    }
}

void MainWindow::createThumbnailToolbar(){
    QWinThumbnailToolBar* thumbnailToolBar = new QWinThumbnailToolBar(this);
    thumbnailToolBar->setWindow(this->windowHandle());

    playToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    playToolButton->setEnabled(true);
    playToolButton->setToolTip(tr("Music: Play"));
    playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playToolButton, SIGNAL(clicked()), this, SLOT(on_musicPlayButton_clicked()));

    pauseToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    pauseToolButton->setEnabled(true);
    pauseToolButton->setToolTip(tr("Music: Pause"));
    pauseToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    connect(pauseToolButton, SIGNAL(clicked()), this, SLOT(on_musicPauseButton_clicked()));

    nextToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    nextToolButton->setEnabled(true);
    nextToolButton->setToolTip(tr("Music: Next"));
    nextToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(nextToolButton, SIGNAL(clicked()), this, SLOT(on_musicNextButton_clicked()));

    thumbnailToolBar->addButton(playToolButton);
    thumbnailToolBar->addButton(pauseToolButton);
    thumbnailToolBar->addButton(nextToolButton);
}

void MainWindow::on_musicRandomButton_clicked()
{
    if (ui->musicRandomButton->isChecked()){
        ui->musicRandomButton->setIcon(QIcon(":/resources/mediaIcons/shuffleOn.png"));
        musicPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random);
    }
    else{
        ui->musicRandomButton->setIcon(QIcon(":/resources/mediaIcons/shuffleOff.png"));
        musicPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    }
}

void MainWindow::on_soundRandomButton_clicked()
{
    if (ui->soundRandomButton->isChecked()){
        ui->soundRandomButton->setIcon(QIcon(":/resources/mediaIcons/shuffleOn.png"));
        soundPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random);
    }
    else{
        ui->soundRandomButton->setIcon(QIcon(":/resources/mediaIcons/shuffleOff.png"));
        soundPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    }
}

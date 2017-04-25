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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    setVersion("0.3.0.0");

    // Gets all settings
    settingsManager = new SettingsManager;
    int checkUpdates = QString::compare(settingsManager->getSetting(Setting::checkForUpdatesOnStart), "1", Qt::CaseInsensitive);

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
    soundPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random); //QMediaPlaylist::PlaybackMode::Loop|
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
    networkManager = new QNetworkAccessManager;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_networkAccessManagerFinished(QNetworkReply*)));

    // Creates Tab for music
    // Creates TableWidget to display song titles
    initialMusicPlay = true;
    musicTable = new QTableWidget;
    musicTable->setMaximumWidth(250);
    musicTable->setColumnWidth(0, 250);
    musicTable->setColumnCount(1);
    musicTable->setHorizontalHeaderLabels(QString("Title").split(";"));
    connect(musicTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_tableDoubleClicked(int,int)));

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
    for (QString character : characterList){
        QFrame *frame = getCharacterPage(character);
        ui->charactersStackedWidget->addWidget(frame);

        QListWidgetItem *listItem = new QListWidgetItem;
        listItem->setText(frame->toolTip());
        listItem->setToolTip(cleanText(character));
        ui->charactersListWidget->addItem(listItem);
    }
    on_characterListClicked(0);
    connect(ui->charactersListWidget, SIGNAL(currentRowChanged(int)), SLOT(on_characterListClicked(int)));

    // Checks for updates on program start
    if (checkUpdates == 0){
        ui->actionCheck_for_Updates_on_Program_Start->setChecked(true);
        on_checkForUpdates_clicked();
    }

    programStart = false;

}

void MainWindow::on_characterListClicked(int index){
    if (index >= 0){
        ui->charactersStackedWidget->setCurrentIndex(index);

        ui->textEdit->append(QDir::currentPath()+"/characters/"+ui->charactersListWidget->item(index)->toolTip()+".png");
        ui->textEdit->append(ui->charactersListWidget->item(index)->toolTip());

        if (QFile(QDir::currentPath()+"/characters/"+ui->charactersListWidget->item(index)->toolTip()+".png").exists()){
            QPixmap charIcon(QDir::currentPath()+"/characters/"+ui->charactersListWidget->item(index)->toolTip()+".png");
            ui->characterIconLabel->setPixmap(charIcon.scaled(180, 180));
            ui->characterIconLabel->adjustSize();
        }
        else if (QFile(QDir::currentPath()+"/characters/"+ui->charactersListWidget->item(index)->toolTip()+".jpg").exists()){
            QPixmap charIcon(QDir::currentPath()+"/characters/"+ui->charactersListWidget->item(index)->toolTip()+".jpg");
            ui->characterIconLabel->setPixmap(charIcon.scaled(180, 180));
            ui->characterIconLabel->adjustSize();
        }
        else{
            ui->characterIconLabel->clear();
        }
    }
    ui->textEdit->append(QString::number(index));
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
    radioTimer->stop();
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier) &&!initialMusicPlay)
    {
        ui->textEdit->append("SHIFT IS PRESSED");

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
                ui->textEdit->append(file + "\n");
            }
        }

        musicPlayer->play();
    }
    else{
        radioActive = false;
        radioPlayer->stop();

        ui->musicNextButton->setEnabled(true);
        ui->musicReplayButton->setEnabled(true);

        if (initialMusicPlay){
            ui->pageMusic->layout()->addWidget(musicTable);
            initialMusicPlay = false;
        }

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
                ui->textEdit->append(file + "\n");
                row++;
            }
        }

        musicPlayer->play();

        //Setting Image
        if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".png").exists()){
            //QPixmap *cover = new QPixmap(resourcesPath+"/Icons/Music"+category+".png");
            ui->musicCoverLabel->setPixmap(QPixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".png").scaledToWidth(300));
        }
        else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".jpg").exists()){
            //QPixmap *cover = new QPixmap(resourcesPath+"/Icons/Music"+category+".jpg");
            ui->musicCoverLabel->setPixmap(QPixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".jpg").scaledToWidth(300));
        }
    }
//    ui->textEdit->append(QString::number(metaPlaylist->mediaCount()));
//    metaPlaylist->setCurrentIndex(0);
//    metaPlayer->setVolume(0);
//    metaPlayer->play();
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
//    if (musicPlayer->isMetaDataAvailable()){
//        ui->textEdit->append("FOUND SOMETHING");
//        QString title = metaPlayer->metaData(QStringLiteral("Title")).toString();
//        ui->textEdit->append(title);
////        QTableWidgetItem *i = new QTableWidgetItem;
////        i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
////        i->setText(title);
////        musicTable->setItem(metaPlaylist->currentIndex(), 0, i);

////        //metaPlaylist->removeMedia(metaPlaylist->currentIndex());
//        ui->textEdit->append(QString::number(metaPlaylist->currentIndex()));
//        if (metaPlaylist->currentIndex()<2){
//            metaPlaylist->next();
//        }
//    }
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

void MainWindow::on_openWiki_clicked(){
    QDesktopServices::openUrl(QUrl("https://github.com/PhilInTheGaps/GM-Companion/wiki"));
}

void MainWindow::setVersion(QString versionAsString){
    versionString = versionAsString;
    versionNumber = 0;
    QString temp = versionAsString.replace(".", "");
    versionNumber = temp.toInt();

    ui->textEdit->append("Version Number: "+QString::number(versionNumber));

}

void MainWindow::on_checkForUpdates_clicked(){
    networkManager->get(QNetworkRequest(QUrl("https://philinthegaps.github.io/GM-Companion/version.html")));
}

void MainWindow::on_networkAccessManagerFinished(QNetworkReply* reply){
    QString replyString = reply->readAll();
    ui->textEdit->clear();
    ui->textEdit->append(replyString+"\n");

    int index1 = replyString.indexOf("VERSION=");
    int index2 = replyString.indexOf(";", index1);

    int onlineVersion = 0;

    QString onlineVersionString = replyString.mid(index1+8, index2-index1-8);
    QString onlineVersionWithDots = onlineVersionString;
    QString temp = onlineVersionString.replace(".", "");
    onlineVersion = temp.toInt();
    ui->textEdit->append("Most Current Version Number: "+QString::number(onlineVersion));

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

void MainWindow::on_tableDoubleClicked(int row, int column){
    musicPlaylist->setCurrentIndex(row);
    int test = column;
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
    settingsManager->setSetting(Setting::checkForUpdatesOnStart, checked);
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
    charEditor->show();
}

void MainWindow::on_updateCharactersButton_clicked()
{
    for(int i = ui->charactersStackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = ui->charactersStackedWidget->widget(i);
        ui->charactersStackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    ui->charactersListWidget->clear();
    //disconnect(ui->charactersListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(on_characterListClicked(int)));

    QStringList characterList = getCharacterList();
    for (QString character : characterList){
        QFrame *frame = getCharacterPage(character);
        ui->charactersStackedWidget->addWidget(frame);

        QListWidgetItem *listItem = new QListWidgetItem;
        listItem->setText(frame->toolTip());
        listItem->setToolTip(cleanText(character));
        ui->charactersListWidget->addItem(listItem);
    }
    ui->charactersListWidget->setCurrentRow(0);
    on_characterListClicked(0);
    //connect(ui->charactersListWidget, SIGNAL(currentRowChanged(int)), SLOT(on_characterListClicked(int)));
}

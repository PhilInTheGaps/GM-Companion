#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flowlayout.h"
#include "characters.h"

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

void MainWindow::rollDice(int sides){

    int amount = ui->amountSpinBox->value();
    int modifier = ui->modifierSpinBox->value();
    int result = 0;

    ui->diceResultTableWidget->insertRow(0);

    for (int i = 0; i<amount; i++){
        QTableWidgetItem *i1 = new QTableWidgetItem;
        QTableWidgetItem *i2 = new QTableWidgetItem;
        QTableWidgetItem *i3 = new QTableWidgetItem;

        int temp = rand() % sides+1;
        i1->setText(QString::number(i+1)+"D"+QString::number(sides));
        i2->setText(QString::number(temp));
        result += temp;
        i3->setText(QString::number(result));

        ui->diceResultTableWidget->insertRow(0);

        ui->diceResultTableWidget->setItem(0, 0, i1);
        ui->diceResultTableWidget->setItem(0, 1, i2);
        ui->diceResultTableWidget->setItem(0, 2, i3);
    }

    result += modifier;

    ui->diceOutputLabel->setText(QString::number(result));
}

void MainWindow::generateDiceFrame(){
    int sidesArray[6] = {2, 3, 4, 6, 12, 20};

    ui->diceOutputLabel->setAlignment(Qt::AlignCenter);
    FlowLayout *diceFlowLayout = new FlowLayout;
    ui->diceFrame->setLayout(diceFlowLayout);

    for (int sides : sidesArray){
        QPushButton *button = new QPushButton;
        button->setText("D"+QString::number(sides));
        //button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //button->setMinimumSize(200, 200);
        button->setStyleSheet("min-width: 50; min-height: 50; max-width: 200; max-height: 200;");
        //button->setMaximumSize(100, 100);

        diceFlowLayout->addWidget(button);
        diceFlowLayout->setAlignment(Qt::AlignCenter);

        connect(button, SIGNAL(clicked()), signalMapperDice, SLOT(map()));
        signalMapperDice->setMapping(button, sides);
    }

    connect(signalMapperDice, SIGNAL(mapped(int)), this, SLOT(rollDice(int)));
}

void MainWindow::generateNamesTab(){
    QStringList folderList = getFolders(QDir::currentPath()+"/names");
    for (QString folder : folderList){
        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setWidgetResizable(true);
            QFrame *frame = new QFrame;
            scrollArea->setWidget(frame);
            QVBoxLayout *frameLayout = new QVBoxLayout;
            frame->setLayout(frameLayout);
            ui->nameTabWidget->addTab(scrollArea, folder);

            folder = QDir::currentPath()+"/names/"+folder;

//            QTabWidget *tabWidget = new QTabWidget;
//            frameLayout->addWidget(tabWidget);


            QStringList subfolderList = getFolders(folder);

            for (QString subfolder : subfolderList){
                if (!subfolder.contains(".")){
                    QFrame *subFrame = new QFrame;
                    //tabWidget->addTab(subFrame, subfolder);
                    frameLayout->addWidget(subFrame);

                    QHBoxLayout *hbox = new QHBoxLayout;
                    subFrame->setLayout(hbox);

                    QLabel *subfolderLabel = new QLabel(subfolder);
                    hbox->addWidget(subfolderLabel);

                    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
                    hbox->addSpacerItem(spacer);

                    QFrame *buttonFrame = new QFrame;
                    QHBoxLayout *buttonLayout = new QHBoxLayout;
                    buttonFrame->setLayout(buttonLayout);
                    buttonFrame->setMaximumWidth(200);
                    hbox->addWidget(buttonFrame);

                    QPushButton *maleButton = new QPushButton;
                    maleButton->setText("Male");
                    buttonLayout->addWidget(maleButton);

                    QPushButton *femaleButton = new QPushButton;
                    femaleButton->setText("Female");
                    buttonLayout->addWidget(femaleButton);

                    QStringList files = getFiles(folder+"/"+subfolder);

                    QString maleFile = folder+"/"+subfolder+"/male.txt";
                    QString femaleFile = folder+"/"+subfolder+"/female.txt";

                    connect(maleButton, SIGNAL(clicked()), signalMapperNames, SLOT(map()));
                    signalMapperNames->setMapping(maleButton, maleFile);

                    connect(femaleButton, SIGNAL(clicked()), signalMapperNames, SLOT(map()));
                    signalMapperNames->setMapping(femaleButton, femaleFile);
                }
            }
            QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
            frameLayout->addSpacerItem(spacer);
        }
    }
    connect(signalMapperNames, SIGNAL(mapped(QString)), this, SLOT(on_generateNames(QString)));
}

void MainWindow::on_generateNames(QString file){
    ui->nameTextEdit->clear();
    int index = file.lastIndexOf("/");
    QString path = file.left(index);

    QString surnamesPath = path+"/surname.txt";

//    ui->nameTextEdit->append(path);
//    ui->nameTextEdit->append(file);
//    ui->nameTextEdit->append(surnamesPath);

    QFile nameFile(file);
    nameFile.open(QIODevice::ReadOnly);
    QString namesAsString = QString::fromUtf8(nameFile.readAll());
    QStringList names = namesAsString.split(",");
    nameFile.close();

    QFile surnamesFile(surnamesPath);
    surnamesFile.open(QIODevice::ReadOnly);
    QString surnamesAsString = QString::fromUtf8(surnamesFile.readAll());
    ui->textEdit->append(surnamesAsString);
    QStringList surnames = surnamesAsString.split(",");
    surnamesFile.close();

    for (int i = 0; i<20; i++){
        QString name = names.at(rand() % names.size());
        QString surname = surnames.at(rand() % surnames.size());

        if (name.at(0)==" "){
            name = name.mid(1);
        }

        ui->nameTextEdit->append(name+" "+surname);
    }

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    setVersion("0.3.0.0");

    // Sets the paths
    musicPath = getSettings("MUSIC_PATH");
    soundPath = getSettings("SOUND_PATH");
    mapsPath = getSettings("MAPS_PATH");
    resourcesPath = getSettings("RESOURCES_PATH");
    checkForUpdatesOnStart = getSettings("CHECK_FOR_UPDATES_ON_START");
    ui->textEdit->append(checkForUpdatesOnStart);

    int checkUpdates = QString::compare(checkForUpdatesOnStart, "1", Qt::CaseInsensitive);

    signalMapperMusic = new QSignalMapper(this);
    signalMapperSound = new QSignalMapper(this);
    signalMapperMaps = new QSignalMapper(this);
    signalMapperDice = new QSignalMapper(this);
    signalMapperNames = new QSignalMapper(this);

    // Generates the dice page
    generateDiceFrame();

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
    connect(ui->actionSet_Database_Path, SIGNAL(triggered(bool)), this, SLOT(on_setDatabasePath_clicked()));
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

    QTabWidget *tabWidgetMusic = new QTabWidget;
    ui->pageMusic->layout()->addWidget(tabWidgetMusic);

    QStringList musicCategories = getFolders(musicPath); // List of all categories
    for (QString folder : musicCategories){

        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            QFrame *frame = new QFrame;

            tabWidgetMusic->addTab(scrollArea, cleanText(folder));
            scrollArea->setWidget(frame);
            scrollArea->setWidgetResizable(true);

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

    QTabWidget *tabWidgetSound = new QTabWidget;
    ui->pageSound->layout()->addWidget(tabWidgetSound);

    QStringList soundCategories = getFolders(soundPath); // List of all categories
    for (QString folder : soundCategories){

        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            QFrame *frame = new QFrame;

            tabWidgetSound->addTab(scrollArea, cleanText(folder));
            scrollArea->setWidget(frame);
            scrollArea->setWidgetResizable(true);

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

    // Character View
    QStringList characterList = getCharacterList();
    for (QString character : characterList){
        //QScrollArea *characterScrollArea = new QScrollArea;
        QFrame *frame = getCharacterPage(character);
//        characterScrollArea->setWidget(characterScrollArea);
//        characterScrollArea->setWidgetResizable(true);
        ui->charactersStackedWidget->addWidget(frame);

        QListWidgetItem *listItem = new QListWidgetItem;
        listItem->setText(frame->toolTip());
        ui->charactersListWidget->addItem(listItem);
    }
    connect(ui->charactersListWidget, SIGNAL(currentRowChanged(int)), SLOT(on_characterListClicked(int)));

    // Checks for updates on program start
    if (checkUpdates == 0){
        ui->actionCheck_for_Updates_on_Program_Start->setChecked(true);
        on_checkForUpdates_clicked();
    }

}

void MainWindow::on_characterListClicked(int index){
    //int row = index.row();
    ui->charactersStackedWidget->setCurrentIndex(index);
}

void MainWindow::setMap(QString mapPath){
    ui->textEdit->append("Map: "+mapPath+"\n");
    mapsImageLabel->setPixmap(QPixmap(mapPath));
    mapsImageLabel->adjustSize();
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

        int row = musicTable->rowCount();
        musicTable->setRowCount(musicTable->rowCount()+files.size());

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
                musicTable->setItem(row, 0, i);

                addToPlaylist(QUrl::fromLocalFile(folder+"/"+file), true);
                ui->textEdit->append(file + "\n");
                row++;
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
        if (QFile(resourcesPath+"/Icons/Music"+category+".png").exists()){
            //QPixmap *cover = new QPixmap(resourcesPath+"/Icons/Music"+category+".png");
            ui->musicCoverLabel->setPixmap(QPixmap(resourcesPath+"/Icons/Music"+category+".png").scaledToWidth(300));
        }
        else if (QFile(resourcesPath+"/Icons/Music"+category+".jpg").exists()){
            //QPixmap *cover = new QPixmap(resourcesPath+"/Icons/Music"+category+".jpg");
            ui->musicCoverLabel->setPixmap(QPixmap(resourcesPath+"/Icons/Music"+category+".jpg").scaledToWidth(300));
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

void MainWindow::on_setDatabasePath_clicked(){

}

void MainWindow::on_checkForUpdatesOnStart(bool checked){
    writeSettings(readSettings(), "CHECK_FOR_UPDATES_ON_START=", QString::number(checked));
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

void MainWindow::on_actionRadio_clicked(){
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_rivendellPlayButton_clicked()
{
    radioTimer->start();
    radioPlayer->setMedia(QUrl("http://radiorivendell.de/"));
    musicPlayer->stop();
    radioActive = true;
    radioID = 0;
    radioPlayer->play();

    ui->musicTitleLabel->setText("Radio: Radio Rivendell");
    ui->musicAlbumLabel->setText("");
    ui->musicArtistLabel->setText("");
    ui->musicYearLabel->setText("");

    ui->musicNextButton->setDisabled(true);
    ui->musicReplayButton->setDisabled(true);
}

void MainWindow::on_rivendellReloadButton_clicked()
{
    if (radioActive){
        radioPlayer->pause();
        radioTimer->start();
        radioPlayer->setMedia(QUrl("http://radiorivendell.de/"));

        ui->musicTitleLabel->setText("Radio: Radio Rivendell");
        ui->musicAlbumLabel->setText("");
        ui->musicArtistLabel->setText("");
        ui->musicYearLabel->setText("");

        radioPlayer->play();
    }
}

void MainWindow::on_radioNetworkAccessManager_finished(QNetworkReply* reply){
    ui->textEdit->clear();
    ui->textEdit->append("FINISHED \n");
    ui->textEdit->append(QString::number(radioID)+"\n");

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    ui->textEdit->append(redirectionTarget.toString());

    if (!redirectionTarget.isNull()) {
        QUrl newUrl = QUrl(redirectionTarget.toUrl());
        QNetworkRequest request(newUrl);
        radioNetworkManager->get(request);
    } else {
        QString replyString = reply->readAll();

        switch(radioID){
        case 0:
        {
            QTextDocument doc;
            doc.setHtml(replyString);
            replyString = doc.toPlainText();
            doc.clear();
            doc.deleteLater();

            // Gets Meta Info from string
            int startIndex = replyString.indexOf("GMT +1.");
            int timeIndex = replyString.indexOf(":", startIndex);
            QString temp = replyString;
            temp = temp.mid(startIndex, timeIndex-startIndex);

            ui->textEdit->append(temp+"\n");

            int nowIndex = temp.indexOf("Now");
            int lineIndex = temp.indexOf("\n", nowIndex+5);

            QString title = temp.mid(nowIndex+4);
            QString artist = temp.mid(lineIndex+1);

            title = title.replace(artist, "");
            title = title.replace("Spotify", "");
            title = title.left(title.length()-1);
            artist = artist.left(artist.length()-3);

            ui->textEdit->append("Title: "+title);
            ui->textEdit->append("Artist: "+artist);

            ui->musicAlbumLabel->setText("Title: "+title);
            ui->musicArtistLabel->setText("Artist: "+artist);

            replyString.clear();
            temp.clear();
            break;
        }
        case 1:
        {
            QTextDocument doc;
            doc.setHtml(replyString);
            replyString = doc.toPlainText();
            ui->textEdit->append(replyString+"\n");
            doc.clear();
            doc.deleteLater();

            // Gets Meta Info from string
            int artistStartIndex = replyString.indexOf("Jetzt läuft ");
            int titleStartIndex = replyString.indexOf("mit dem Titel »", artistStartIndex);
            int albumStartIndex = replyString.indexOf("vom Album »", titleStartIndex);
            int albumEndIndex = replyString.indexOf("«", albumStartIndex);
            QString temp = replyString;

            temp = temp.mid(artistStartIndex, albumEndIndex-artistStartIndex);


//            int nowIndex = temp.indexOf("Now");
//            int lineIndex = temp.indexOf("\n", nowIndex+5);

//            QString title = temp.mid(nowIndex+4);
//            QString artist = temp.mid(lineIndex+1);

//            title = title.replace(artist, "");
//            title = title.replace("Spotify", "");
//            title = title.left(title.length()-1);
//            artist = artist.left(artist.length()-3);

//            ui->textEdit->append("Title: "+title);
//            ui->textEdit->append("Artist: "+artist);

//            ui->musicAlbumLabel->setText("Title: "+title);
//            ui->musicArtistLabel->setText("Artist: "+artist);

//            replyString.clear();
//            temp.clear();
            break;
        }
        default:
        {
            ui->textEdit->append("Invalid RadioID \n");
        }
        }
    }
    reply->deleteLater();
}

void MainWindow::on_radioTimer_timeout(){
    ui->textEdit->append("TIMER FINISHED");
    switch (radioID) {
    case 0:
        radioNetworkManager->get(QNetworkRequest(QUrl("https://www.radiorivendell.com/page/last-played/")));
        radioTimer->start();
        break;
    case 1:
        radioNetworkManager->get(QNetworkRequest(QUrl("http://laut.fm/mmorpg")));
        break;
    default:
        ui->textEdit->append("ERROR: RadioID is not correct \n");
        break;
    }
}


void MainWindow::on_mmorpgPlayButton_clicked()
{
    //radioTimer->start();
    radioPlayer->setMedia(QUrl("http://stream3.laut.fm/mmorpg"));
    musicPlayer->stop();
    radioActive = true;
    radioID = 1;
    radioPlayer->play();

    ui->musicTitleLabel->setText("Radio: MMORPG Radio");
    ui->musicAlbumLabel->setText("Metadata Information");
    ui->musicArtistLabel->setText("are currently not");
    ui->musicYearLabel->setText("supported. Sorry.");

    ui->musicNextButton->setDisabled(true);
    ui->musicReplayButton->setDisabled(true);
}

void MainWindow::on_mmorpgReloadButton_clicked()
{
    if (radioActive){
        //radioTimer->start();
        radioPlayer->pause();
        radioPlayer->setMedia(QUrl("http://stream3.laut.fm/mmorpg"));

        ui->musicTitleLabel->setText("Radio: MMORPG Radio");
        ui->musicAlbumLabel->setText("Metadata Information");
        ui->musicArtistLabel->setText("are currently not");
        ui->musicYearLabel->setText("supported. Sorry.");

        radioPlayer->play();
    }
}

void MainWindow::on_radioMetaDataChanged(){
    QString title = radioPlayer->metaData(QStringLiteral("Title")).toString();
    QString album = radioPlayer->metaData(QStringLiteral("AlbumTitle")).toString();
    QString artist = musicPlayer->metaData(QStringLiteral("Author")).toString();
    if (radioPlayer->isMetaDataAvailable()){
        ui->musicAlbumLabel->setText("Title: "+title);
        ui->musicArtistLabel->setText("Artist: "+artist);
        ui->musicYearLabel->setText("Album: "+album);

        ui->musicCoverLabel->setToolTip(album+": "+title);
    }
    else{
        ui->musicAlbumLabel->setText("Metadata Information");
        ui->musicArtistLabel->setText("are currently not");
        ui->musicYearLabel->setText("supported. Sorry.");
    }
}

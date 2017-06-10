#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

#include <qtoolbutton.h>
#include <QSettings>

QString getStyleSheet(QString url){
    QString styleSheet = "QToolButton {"
                         "min-width: 152; "
                         "min-height: 152; "
                         "padding: 1px; "
                         "border-radius: 2px; "
                         "background-color: #222222;"
                         "background-image: url("+url+"); "
                         "background-repeat: no-repeat; "
                         "background-position: center center;}"
                         "QToolButton QWidget{"
                         "color: white"
                         "} ";

    return styleSheet;
}

void MainWindow::generateMusicButtons(){
    qDebug() << tr("Generating Music Buttons...");

    tabWidgetMusic = new QTabWidget;
    ui->tabMusic->layout()->addWidget(tabWidgetMusic);

    QStringList musicCategories = getFolders(settingsManager->getSetting(Setting::musicPath)); // List of all categories
    for (QString folder : musicCategories){
        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            QFrame *frame = new QFrame;
            QVBoxLayout* frameLayout = new QVBoxLayout;
            frame->setLayout(frameLayout);
            frameLayout->setAlignment(Qt::AlignTop);

            tabWidgetMusic->addTab(scrollArea, cleanText(folder));
            scrollArea->setWidget(frame);
            scrollArea->setWidgetResizable(true);

            if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png);"
                                     "background-position: center top;}");
            }
            else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg);"
                                     "background-position: center top;}");
            }

            // Get Groups
            QStringList groups;
            QList<QFrame*> groupFrames;

            QSettings settings(settingsManager->getSetting(Setting::musicPath)+"/"+folder+"/groups.ini", QSettings::IniFormat);
            int groupsSize = settings.beginReadArray("Groups");
            for (int i = 0; i<groupsSize; i++){
                settings.setArrayIndex(i);
                QString group = settings.value("group").toString();
                groups.push_back(group);

                QFrame* f = new QFrame;
                f->setStyleSheet("background: none;");
                FlowLayout* fLayout = new FlowLayout;
                f->setLayout(fLayout);

                QLabel* l = new QLabel(group);
                l->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
                l->setStyleSheet("background: none;");

                frame->layout()->addWidget(l);
                frame->layout()->addWidget(f);

                groupFrames.push_back(f);
            }
            settings.endArray();

            // Generating musicButtons
            QString path = settingsManager->getSetting(Setting::musicPath)+"/"+folder;
            QStringList musicFolders = getFolders(path);

            QLabel* allLabel = new QLabel("General");
            allLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
            allLabel->setStyleSheet("background: none;");
            frame->layout()->addWidget(allLabel);

            QFrame* allFrame = new QFrame;
            allFrame->setStyleSheet("background: none;");
            FlowLayout *flowLayoutMusic = new FlowLayout;
            allFrame->setLayout(flowLayoutMusic);
            frame->layout()->addWidget(allFrame);

            for (QString s : musicFolders){
                if (!s.contains(".")){
                    QToolButton* b = new QToolButton;
                    connect(b, SIGNAL(clicked()), signalMapperMusic, SLOT(map()));
                    signalMapperMusic->setMapping(b, settingsManager->getSetting(Setting::musicPath)+"/"+folder+"/"+s);

                    if (settingsManager->getSetting(Setting::buttonStyle) == "small"){

                        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                        b->setIconSize(QSize(64,64));
                        b->setMinimumWidth(150);
                        b->setMinimumHeight(90);
                        b->setStyleSheet("background-color: #222222;");

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


                    }else{
                        if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png").exists()){
                            b->setStyleSheet(getStyleSheet(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".png"));
                        }
                        else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg").exists()){
                            b->setStyleSheet(getStyleSheet(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music/"+folder+"/"+s+".jpg"));
                        }
                        else if (QFile(QApplication::applicationDirPath()+"/resources/button.png").exists()){
                           b->setStyleSheet(getStyleSheet(QApplication::applicationDirPath()+"/resources/button.png"));
                        }
                        else{
                            b->setStyleSheet("QToolButton {"
                                             "min-width: 152; "
                                             "min-height: 152; "
                                             "padding: 1px; "
                                             "border-radius: 2px; "
                                             "background-color: #222222;"
                                             "background-position: center center;}"
                                             "QToolButton QWidget{"
                                             "color: white"
                                             "} ");
                        }

                        b->setFixedSize(152, 152);
                    }


                    QFont font;
                    font.setPixelSize(15);
                    b->setFont(font);

                    QString title = cleanText(s);

                    if (title.length()>20){
                        int i = title.indexOf(" ", 14);
                        title.remove(i, 1);
                        title.insert(i, "\n");
                    }

                    b->setText(title);

                    b->setToolTip(cleanText(s));

                    bool inGroup = false;
                    for (int gIndex = 0; gIndex<groups.size(); gIndex++){
                        int size = settings.beginReadArray(groups.at(gIndex));

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

// Play a song from the selected folder
void MainWindow::playMusic(QString folder){

    if (!getFiles(folder).empty()){
        // Enable music Buttons in case they were disabled from playing radio before
        ui->musicNextButton->setEnabled(true);
        ui->musicReplayButton->setEnabled(true);
        ui->musicRandomButton->setEnabled(true);

        // Stop Radio
        radioActive = false;
        radioPlayer->stop();

        // Check if Shift is pressed. If true, adds selected folder to playlist, else creates new playlist
        if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ShiftModifier) &&!initialMusicPlay && radioActive == false)
        {
            // Get foldername from path
            QString folderName = folder;
            int index = folderName.lastIndexOf("/")+1;
            folderName = cleanText(folderName.mid(index));

            qDebug() << tr("Adding music from") << folderName << tr("to existing playlist...");

            // Get category name from path
            QString category = folder;
            index = category.lastIndexOf("/", index-2);
            category = category.mid(index);

            QStringList files = getFiles(folder);

            // Add music files to ListView
            for (QString file : files){
                if (file.contains(".mp3") || file.contains(".wav")){
                    QTableWidgetItem *i = new QTableWidgetItem;
                    i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    i->setText(cleanText(file).replace(folderName, ""));

                    musicTable->insertRow(musicTable->rowCount());
                    musicTable->setItem(musicTable->rowCount()-1, 0, i);

                    ui->musicTitleComboBox->addItem(cleanText(file).replace(folderName, ""));

                    addToPlaylist(QUrl::fromLocalFile(folder+"/"+file), true);
                }
            }

            // Adjust column width of ListView
            if (musicTable->rowCount() > 9){
                musicTable->setColumnWidth(0, 220);
            }
            else{
                musicTable->setColumnWidth(0, 230);
            }

            musicPlayer->play();
        }
        else{

            // Add ListView
            if (initialMusicPlay){
                if (settingsManager->getSetting(Setting::enableMusicTrackList).toInt() == 1)
                    ui->tabMusic->layout()->addWidget(musicTable);
                initialMusicPlay = false;
            }

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
            ui->musicTitleComboBox->clear();

            // Add music files to playlist and ListView
            QStringList files = getFiles(folder);
            int row = 0;
            for (QString file : files){
                if (file.contains(".mp3") || file.contains(".wav")){
                    QTableWidgetItem *i = new QTableWidgetItem;
                    i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    i->setText(cleanText(file).replace(folderName, ""));
                    musicTable->setItem(row, 0, i);

                    ui->musicTitleComboBox->addItem(cleanText(file).replace(folderName, ""));

                    addToPlaylist(QUrl::fromLocalFile(folder+"/"+file), true);
                    row++;
                }
            }

            // Adjust ListView Column Size
            musicTable->setRowCount(files.size());
            if (files.size() > 9){
                musicTable->setColumnWidth(0, 220);
            }
            else{
                musicTable->setColumnWidth(0, 230);
            }

            // If random mode is active, start with a random song
            if (ui->musicRandomButton->isChecked()){
                musicPlaylist->next();
            }

            musicPlayer->play();

            // Display Folder Image if existing
            if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".png").exists()){
                ui->musicCoverLabel->setPixmap(QPixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".png").scaledToWidth(ui->musicCoverLabel->width()));
            }
            else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".jpg").exists()){
                ui->musicCoverLabel->setPixmap(QPixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Music"+category+".jpg").scaledToWidth(ui->musicCoverLabel->width()));
            }
        }
    }
}

// Update the music progress bar
void MainWindow::updateProgressBar(){
    if (musicPlayer->duration() == 0){ // Needed because PB kind of escalates when 0 is maximum, minimum and current value
        ui->musicProgressSlider->setMaximum(1);
        ui->musicProgressSlider->setValue(0);
    }
    else{
        ui->musicProgressSlider->setMaximum(musicPlayer->duration());
        ui->musicProgressSlider->setValue(musicPlayer->position());
    }
}

// When user moves and releases slider, set music position
void MainWindow::on_musicProgressSlider_sliderReleased(){
    musicPlayer->setPosition(ui->musicProgressSlider->value());
}

// If new metadata is available, update the displayed stuff
void MainWindow::updateMetaData(){

    QString title = musicPlayer->metaData(QStringLiteral("Title")).toString();
    QString album = musicPlayer->metaData(QStringLiteral("AlbumTitle")).toString();
//    QString artist = musicPlayer->metaData(QStringLiteral("Author")).toString();
    if (musicPlayer->isMetaDataAvailable()){
        if (title.length() > 1){
            ui->musicTitleComboBox->setItemText(musicPlaylist->currentIndex(), title);
        }

        ui->musicCoverLabel->setToolTip(album+": "+title);

        // Replace ListView item with title
        QTableWidgetItem *i = new QTableWidgetItem;
        i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        i->setText(title);
        musicTable->setItem(musicPlaylist->currentIndex(), 0, i);

    }

    // Select currently played song in ListView
    musicTable->selectRow(musicPlaylist->currentIndex());
    ui->musicTitleComboBox->setCurrentIndex(musicPlaylist->currentIndex());
}

// Toggles Random mode
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

// Change Music / Radio Volume
void MainWindow::on_musicVolumeSlider_valueChanged(int value)
{
    musicPlayer->setVolume(value);
    radioPlayer->setVolume(value);
}

// Select a music file from list
void MainWindow::on_tableDoubleClicked(int row){
    musicPlaylist->setCurrentIndex(row);
}

// Pause
void MainWindow::on_musicPauseButton_clicked()
{
    if (radioActive){
        radioPlayer->pause();
    }
    else{
        musicPlayer->pause();
    }
}

// Play
void MainWindow::on_musicPlayButton_clicked()
{
    if (radioActive){
        radioPlayer->play();
    }
    else{
        musicPlayer->play();
    }
}

// Replay
void MainWindow::on_musicReplayButton_clicked()
{
    musicPlayer->setPosition(0);
}

// Next
void MainWindow::on_musicNextButton_clicked()
{
    if (!musicPlaylist->isEmpty()){
        musicPlaylist->next();
    }
}

// Change current track when another one is selected in the combobox
void MainWindow::on_musicTitleComboBox_currentIndexChanged(int index)
{
    if (index != musicPlaylist->currentIndex()){
        musicPlaylist->setCurrentIndex(index);
    }
}

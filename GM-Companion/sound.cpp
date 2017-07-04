#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

#include <QToolButton>

QString getSoundStyleSheet(QString url){
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

void MainWindow::generateSoundButtons(){
    qDebug() << tr("Generating Sound Buttons...");
    tabWidgetSound = new QTabWidget;
    ui->tabSound->layout()->addWidget(tabWidgetSound);

    QStringList soundCategories = getFolders(settingsManager->getSetting(Setting::soundPath)); // List of all categories
    for (QString folder : soundCategories){

        if (!folder.contains(".")){
            QScrollArea *scrollArea = new QScrollArea;
            QFrame *frame = new QFrame;

            tabWidgetSound->addTab(scrollArea, cleanText(folder));
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

            // Generating soundButtons
            QString path = settingsManager->getSetting(Setting::soundPath)+"/"+folder;
            QStringList soundFolders = getFolders(path);

            FlowLayout *flowLayoutSound = new FlowLayout;
            frame->setLayout(flowLayoutSound);

            for (QString s : soundFolders){
                if (!s.contains(".")){
                    QToolButton* b = new QToolButton;
                    connect(b, SIGNAL(clicked()), signalMapperSound, SLOT(map()));
                    signalMapperSound->setMapping(b, settingsManager->getSetting(Setting::soundPath)+"/"+folder+"/"+s);

                    if (settingsManager->getSetting(Setting::buttonStyle) == "small"){

                        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                        b->setIconSize(QSize(64,64));
                        b->setMinimumWidth(150);
                        b->setMinimumHeight(90);
                        b->setStyleSheet("background-color: #222222;");

                        if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png").exists()){
                            QPixmap pixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png");
                            QIcon icon(pixmap);
                            b->setIcon(icon);
                        }
                        else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg").exists()){
                            QPixmap pixmap(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg");
                            QIcon icon(pixmap);
                            b->setIcon(icon);
                        }


                    }else{
                        if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png").exists()){
                            b->setStyleSheet(getSoundStyleSheet(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png"));
                        }
                        else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg").exists()){
                            b->setStyleSheet(getSoundStyleSheet(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpf"));
                        }
                        else if (QFile(QApplication::applicationDirPath()+"/resources/button.png").exists()){
                            b->setStyleSheet(getSoundStyleSheet(QApplication::applicationDirPath()+"/resources/button.png"));
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

                    flowLayoutSound->addWidget(b);
                }
            }
        }
    }
}

// Plays a sound from the selected folder
void MainWindow::playSound(QString folder){
    QString folderName = folder;
    int index = folderName.lastIndexOf("/")+1;
    folderName = cleanText(folderName.mid(index));

    QStringList files = getFiles(folder);

    soundPlaylist->clear(); // Clear sound playlist

    for (QString file : files){
        if (file.contains(".mp3") || file.contains(".wav")){ // There could be other files in the folder
            addToPlaylist(QUrl::fromLocalFile(folder+"/"+file), false);
        }
    }

    // If mode is Random, skip to next sound, so it does not always start with the same
    if (ui->soundRandomButton->isChecked()){
        soundPlaylist->next();
    }

    soundPlayer->play();

    // Display the selected sound folder
    ui->soundNameLabel->setText(tr("Folder: ")+folderName);
}

// Toggles Random mode
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

// Change Sound Volume
void MainWindow::on_soundVolumeSlider_valueChanged(int value)
{
    soundPlayer->setVolume(value);
}

// Play
void MainWindow::on_soundPlayButton_clicked()
{
    soundPlayer->play();
}

// Pause
void MainWindow::on_soundPauseButton_clicked()
{
    soundPlayer->pause();
}

// Replay
void MainWindow::on_soundReplayButton_clicked()
{
    soundPlayer->setPosition(0);
}

// Next
void MainWindow::on_soundNextButton_clicked()
{
    if (!soundPlaylist->isEmpty()){
        soundPlaylist->next();
    }
}

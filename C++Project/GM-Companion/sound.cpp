#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

#include <QToolButton>

void MainWindow::generateSoundButtons(){
    qDebug() << "Generating Sound Buttons...";
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
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".png); background-attachment: fixed;}");
            }
            else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg").exists()){
                frame->setStyleSheet("QFrame{background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Backgrounds/"+folder+".jpg); background-attachment: fixed;}");
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

                    if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png").exists()){
                        b->setStyleSheet("QToolButton {min-width: 152; "
                                         "min-height: 152; "
                                         "padding: 1px; "
                                         "border-radius: 2px; "
                                         "background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".png); "
                                         "background-repeat: no-repeat; "
                                         "background-position: center center;}"
                                         "QToolButton QWidget{"
                                         "color: white} ");
                    }
                    else if (QFile(settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg").exists()){
                        b->setStyleSheet("QToolButton {min-width: 152; "
                                         "min-height: 152; "
                                         "padding: 1px; "
                                         "border-radius: 2px; "
                                         "background-image: url("+settingsManager->getSetting(Setting::resourcesPath)+"/Icons/Sounds/"+folder+"/"+s+".jpg); "
                                         "background-repeat: no-repeat; "
                                         "background-position: center center;}"
                                         "QToolButton QWidget{"
                                         "color: white} ");
                    }
                    else if (QFile(QApplication::applicationDirPath()+"/resources/button.png").exists()){
                        b->setStyleSheet("QToolButton {min-width: 152; "
                                         "min-height: 152; "
                                         "padding: 1px; "
                                         "border-radius: 2px; "
                                         "background-image: url("+QApplication::applicationDirPath()+"/resources/button.png); "
                                         "background-repeat: no-repeat; "
                                         "background-position: center center;}"
                                         "QToolButton QWidget{"
                                         "color: white} ");
                    }
                    else{
                        b->setStyleSheet("QToolButton {min-width: 152; "
                                         "min-height: 152; "
                                         "padding: 1px; "
                                         "border-radius: 2px;");
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

                    b->setFixedSize(152, 152);
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
    ui->soundNameLabel->setText("Folder: "+folderName);
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

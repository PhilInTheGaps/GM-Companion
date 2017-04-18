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

QStringList getFolders(QString path){
    //C:\Users\Phil\Google Drive\GM Boys\GM-Companion\Music\Fantasy
    //C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy

    QDir myDir(path);
    myDir.setFilter(QDir::Dirs);
    QStringList folders = myDir.entryList();

    return folders;
}

QStringList getFiles(QString folder){
    QDir myDir(folder);
    myDir.setFilter(QDir::Files);
    QStringList files = myDir.entryList(); //QDir::Files
    return files;
}

QString cleanText(QString text){
    text.replace("_", " ");
    return text;
}

void writeSettings(QString content, QString indicator, QString newSetting){
    QFile file("C:/Users/Phil/git/RPGMSPlayer/C++Project/GM-Companion/settings.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream output(&file);
    int i1 = content.indexOf(indicator);
    if (i1 > -1){
        int i2 = content.indexOf(";", i1);
        content.remove(i1, i2);
        content.insert(i1, indicator+newSetting);
    }
    else{
        content.append(indicator+newSetting+";"+"\n");
    }

    output << content;
    file.close();
}

QString readSettings(){
    QFile file("C:/Users/Phil/git/RPGMSPlayer/C++Project/GM-Companion/settings.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);
    QString content = textStream.readAll();
    file.close();
    return content;
}

void setFolderLocations(QString path){
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setWindowTitle("Open Files");
//    fileDialog->setDirectory(QStandardPaths::standardLocations(QStandardPaths::HomeLocation));
    if (fileDialog->exec() == QDialog::Accepted){
        QStringList paths = fileDialog->selectedFiles();
        path = paths.at(0);
    }
    writeSettings(readSettings(), "RESOURCES_PATH=", path);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    musicPath = "C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music";
    soundPath = "C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Sounds";
    mapsPath = "C:/Users/Phil/Google Drive/GM Boys/Regelwerk und Co_/DSA5/Karten";
    resourcesPath = "C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Resources (New)";

    signalMapper = new QSignalMapper(this);

    musicPlayer = new QMediaPlayer(this);
    musicPlaylist = new QMediaPlaylist();
    musicPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    musicPlayer->setPlaylist(musicPlaylist);

    connect(ui->actionSet_Resources_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setResourcesFolder_clicked()));

    // Generating ToolBox for Music
    ui->toolBoxMusic->removeItem(0);
    QStringList musicCategories = getFolders(musicPath);
    for (QString folder : musicCategories){
        if (!folder.contains(".")){
            QFrame *frame = new QFrame;
            ui->toolBoxMusic->addItem(frame, cleanText(folder));

            // Generating musicButtons
            QString path = musicPath+"/"+folder; //"C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/OSTs"
            QStringList musicFolders = getFolders(path);

            FlowLayout *flowLayout = new FlowLayout;
            frame->setLayout(flowLayout);

            for (QString s : musicFolders){
                if (!s.contains(".")){
                    QPushButton *b = new QPushButton(); //cleanText(s)
                    connect(b, SIGNAL(clicked()), signalMapper, SLOT(map()));
                    signalMapper->setMapping(b, musicPath+"/"+folder+"/"+s);

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

                    b->setFixedSize(160, 160);
                    b->setToolTip(cleanText(s));

                    flowLayout->addWidget(b);
                }
            }
        }
    }

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));
}

void MainWindow::addToPlaylist(QUrl url){
        musicPlaylist->addMedia(url);
}

void MainWindow::playMusic(QString folder){
    ui->textEdit->clear();
    ui->textEdit->append(folder+"\n");
    QStringList files = getFiles(folder);
    musicPlaylist->clear();
    for (QString file : files){
        addToPlaylist(QUrl::fromLocalFile(folder+"/"+file));
        ui->textEdit->append(file + "\n");
    }

    //musicPlayer->setMedia(QUrl::fromLocalFile("C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy/Action/TitanSoul.mp3")); //"C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy/Action/TitanSoul.mp3"
    musicPlayer->setVolume(10);
    musicPlayer->play();
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_setResourcesFolder_clicked(){
    setFolderLocations(resourcesPath);
}

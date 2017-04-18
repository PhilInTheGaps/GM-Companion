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

// Replaces all undescores with whitespaces
QString cleanText(QString text){
    text.replace("_", " ");
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
void writeSettings(QString content, QString indicator, QString newSetting){
    QFile file(QDir::currentPath()+"/settings.txt");
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

// Reads all the settings as one string
QString readSettings(){
    QFile file(QDir::currentPath()+"/settings.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);
    QString content = textStream.readAll();
    file.close();
    return content;
}

// Opens a Directory Chooser to set the new folder location
void setFolderLocations(QString indicator){
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

    signalMapper = new QSignalMapper(this);

    musicPlayer = new QMediaPlayer(this);
    musicPlaylist = new QMediaPlaylist();
    musicPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random); //QMediaPlaylist::PlaybackMode::Loop|
    musicPlayer->setPlaylist(musicPlaylist);

    // Connects the Menu signals with the according event slots
    connect(ui->actionSet_Music_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setMusicFolder_clicked()));
    connect(ui->actionSet_Sound_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setSoundFolder_clicked()));
    connect(ui->actionSet_Maps_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setMapsFolder_clicked()));
    connect(ui->actionSet_Resources_Folder, SIGNAL(triggered(bool)), this, SLOT(on_setResourcesFolder_clicked()));

    // Creates Tab for music
    QHBoxLayout *tabMusicLayout = new QHBoxLayout; // Layout inside the music tab
    ui->tabMusic->setLayout(tabMusicLayout);

    musicTable = new QTableWidget;
    musicTable->setMaximumWidth(250);
    musicTable->setColumnWidth(0, 250);
    musicTable->setColumnWidth(1, 250);
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

    tabMusicLayout->addWidget(musicTable);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(playMusic(QString)));
}

void MainWindow::addToPlaylist(QUrl url){
        musicPlaylist->addMedia(url);
}

void MainWindow::playMusic(QString folder){
    QString folderName = folder;
    int index = folderName.lastIndexOf("/")+1;
    folderName = cleanText(folderName.mid(index));

    ui->textEdit->clear();
    ui->textEdit->append(folderName+"\n");
    QStringList files = getFiles(folder);

    musicTable->setRowCount(files.size());
    musicPlaylist->clear();

    int row = 0;
    for (QString file : files){
        QTableWidgetItem *i = new QTableWidgetItem;
        i->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        i->setText(file);
        musicTable->setItem(row, 0, i);

        addToPlaylist(QUrl::fromLocalFile(folder+"/"+file));
        ui->textEdit->append(file + "\n");
        row++;
    }

    musicPlayer->setVolume(10);
    musicPlayer->play();
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

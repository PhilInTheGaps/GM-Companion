#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QMediaPlayer>
#include <QStringList>
#include <QDir>
#include <QPushButton>

QStringList getFolders(QString path) {
    //C:\Users\Phil\Google Drive\GM Boys\GM-Companion\Music\Fantasy
    //C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy

    QDir myDir(path);
    QStringList folders = myDir.entryList();

    return folders;
}

//void addMusicButtons(class window){
//    QStringList musicFolders = getFolders("C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy");
//    for (QString s : musicFolders)
//    {
//        if (!s.contains("."))
//        {
//            QPushButton *b = new QPushButton(s);
//            QObject::connect(&b, SIGNAL (clicked()), window, SLOT (playMusic()));
//        }
//    }
//}

QVector<QPushButton> *getButtons(MainWindow *mainWindow){
    QVector<QPushButton> *v = new QVector<QPushButton>;
    QPushButton *b = new QPushButton("TEST");
    //ui->infoBox->addWidget(b);
    QObject::connect(b, SIGNAL(clicked()), mainWindow, SLOT(playMusic()));
    v->push_back(*b);
    return v;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->infoBox->addWidget(new QLabel("TEST LABEL!!!"));

    // Get musicButtons
    QStringList musicFolders = getFolders("C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy");

    for (QString s : musicFolders){

        QPushButton *b = new QPushButton(s);
        connect(b, SIGNAL (clicked()), this, SLOT (playMusic()));

        ui->gridLayout->addWidget(b);
    }

}

void MainWindow::playMusic(){
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy/Action/TitanSoul.mp3")); //"C:/Users/Phil/Google Drive/GM Boys/GM-Companion/Music/Fantasy/Action/TitanSoul.mp3"
    player->setVolume(10);
    player->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}




//void MainWindow::on_goButton_clicked()
//{
//    QString word = ui->lineEdit->text();
//    ui->textEdit->find(word, QTextDocument::FindWholeWords);
//}

//void MainWindow::getTextFile(){
//    QFile myFile(":/textFiles/testTextFile.txt");
//    myFile.open(QIODevice::ReadOnly);

//    QTextStream textStream(&myFile);
//    QString line = textStream.readAll();
//    myFile.close();

//    ui->textEdit->setPlainText(line);
//    QTextCursor textCursor = ui->textEdit->textCursor();
//    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
//}

//void MainWindow::on_musicPlayButton_clicked()
//{

//}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    getTextFile();
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

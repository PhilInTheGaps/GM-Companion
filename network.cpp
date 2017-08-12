#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>
#include <QXmlStreamReader>

#include <QDebug>

// Check for Updates
void MainWindow::on_actionCheck_for_Updates_triggered(){
    qDebug() << tr("Checking for updates...");
    versionNetworkManager->get(QNetworkRequest(QUrl("https://philinthegaps.github.io/GM-Companion/version.html")));
}

// Evaluate received version.html
void MainWindow::on_versionNetworkAccessManagerFinished(QNetworkReply* reply){
    QString replyString = reply->readAll();

    int index1 = replyString.indexOf("VERSION=");
    int index2 = replyString.indexOf(";", index1);

    int onlineVersion = 0;

    QString onlineVersionString = replyString.mid(index1+8, index2-index1-8);
    QString onlineVersionWithDots = onlineVersionString;
    QString temp = onlineVersionString.replace(".", "");
    onlineVersion = temp.toInt();

    if (versionNumber < onlineVersion){
        qDebug() << tr("New version has been found!");

        QFrame *dialogFrame = new QFrame;
        dialogFrame->setWindowTitle(tr("Update Available!"));
        dialogFrame->setMinimumSize(400, 50);
        QVBoxLayout *dialogLayout = new QVBoxLayout;
        dialogFrame->setLayout(dialogLayout);
        QLabel *l1 = new QLabel;
        l1->setText(tr("A new version has been found:"));
        QLabel *l2 = new QLabel;
        l2->setText(onlineVersionWithDots);

        QFrame *btnFrame = new QFrame;
        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnFrame->setLayout(btnLayout);

        QPushButton *openButton = new QPushButton;
        openButton->setText(tr("Open Download Page"));
        QPushButton *closeButton = new QPushButton;
        closeButton->setText(tr("Close"));
        btnLayout->addWidget(openButton);
        btnLayout->addWidget(closeButton);

        dialogLayout->addWidget(l1);
        dialogLayout->addWidget(l2);
        dialogLayout->addWidget(btnFrame);

        connect(openButton, SIGNAL(clicked(bool)), SLOT(actionI_want_to_use_an_older_Version_triggered()));
        connect(closeButton, SIGNAL(clicked(bool)), dialogFrame, SLOT(close()));

        dialogFrame->show();
    }
    else{

        qDebug() << tr("No updates found.");

        if(!onStartUpdateCheck){
            QFrame* f = new QFrame;
            f->setMinimumSize(300, 100);
            QVBoxLayout* l = new QVBoxLayout;
            f->setLayout(l);

            QLabel* label = new QLabel(tr("No updates have been found!"));
            l->addWidget(label);

            QPushButton* ok = new QPushButton;
            ok->setText(tr("Ok"));
            l->addWidget(ok);

            connect(ok, SIGNAL(clicked(bool)), f, SLOT(close()));

            f->show();
        }
    }

    onStartUpdateCheck = false;
}

// Evaluate blog entries
void MainWindow::on_blogNetworkAccessManagerFinished(QNetworkReply * reply){
    ui->blogTextEdit->clear();

    qDebug() << tr("Evaluating blog blog feed ...");

    QString replyString = reply->readAll();
    QXmlStreamReader reader(replyString);

    qDebug() << "Starting blog feed reader ...";
    if (reader.readNextStartElement()) {

        qDebug().noquote() << reader.name();

        if (reader.name() == "rss"){
            qDebug() << "Reading element: rss ...";
            while(reader.readNextStartElement()){

                qDebug().noquote() << "   rss item:" << reader.name();

                while (reader.readNextStartElement()){

                    qDebug().noquote() << "      item:" << reader.name();

                    if(reader.name() == "item"){
                        qDebug() << "Reading element: item ...";

                        QString title;
                        QString pubDate;
                        QString description;

                        while(reader.readNextStartElement()){
                            if (reader.name() == "title"){
                                title = "<h1>"+reader.readElementText()+"</h1>";
                            }
                            else if (reader.name() == "pubDate"){
                                pubDate = reader.readElementText();
                            }
                            else if (reader.name() == "description"){
                                description = reader.readElementText();
                            }
                            else{
                                reader.skipCurrentElement();
                            }
                        }

                        pubDate = pubDate.replace("00:00:00", "").replace(" +0200", "");

                        ui->blogTextEdit->append(title + "\n (" + pubDate + ")\n\n" + description);
                        ui->blogTextEdit->append(" ");

                    }else
                        reader.skipCurrentElement();
                }
            }
        }
        else
            reader.raiseError(QObject::tr("Incorrect file"));
    }

    ui->blogTextEdit->verticalScrollBar()->setValue(0);

}

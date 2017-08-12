#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>
#include <QXmlStreamReader>

#include <QDebug>

// Evaluate blog entries
void MainWindow::on_blogNetworkAccessManagerFinished(QNetworkReply * reply){
    ui->blogTextEdit->clear();

    qDebug().noquote() << "Evaluating blog feed ...";

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

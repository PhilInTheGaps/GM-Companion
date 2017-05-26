#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>
#include <QXmlStreamReader>

// Check for Updates
void MainWindow::on_actionCheck_for_Updates_triggered(){
    qDebug() << "Checking for updates...";
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
        qDebug() << "New version has been found!";

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

        connect(openButton, SIGNAL(clicked(bool)), SLOT(actionI_want_to_use_an_older_Version_triggered()));
        connect(closeButton, SIGNAL(clicked(bool)), dialogFrame, SLOT(close()));

        dialogFrame->show();
    }
    else{

        qDebug() << "No updates found.";

        if(!onStartUpdateCheck){
            QFrame* f = new QFrame;
            f->setMinimumSize(300, 100);
            QVBoxLayout* l = new QVBoxLayout;
            f->setLayout(l);

            QLabel* label = new QLabel("No updates have been found!");
            l->addWidget(label);

            QPushButton* ok = new QPushButton;
            ok->setText("Ok");
            l->addWidget(ok);

            connect(ok, SIGNAL(clicked(bool)), f, SLOT(close()));

            f->show();
        }
    }

    onStartUpdateCheck = false;
}

// Evaluate blog entries
void MainWindow::on_blogNetworkAccessManagerFinished(QNetworkReply * reply){
    QString replyString = reply->readAll();

    QXmlStreamReader reader(replyString);

    if (reader.readNextStartElement()) {
            if (reader.name() == "feed"){
                while(reader.readNextStartElement()){
                    if(reader.name() == "entry"){
                        while(reader.readNextStartElement()){
                            if (reader.name() == "title"){
                                ui->blogTextEdit->append("<h1>"+reader.readElementText()+"</h1>");
                            }
                            else if (reader.name() == "updated"){
                                QString s = reader.readElementText();
                                QString date = s.left(s.indexOf("T"));
                                QDate d = QDate::fromString(date, "yyyy-MM-dd");

                                ui->blogTextEdit->append("(Last Update: "+d.longMonthName(d.month())+" "+QString::number(d.day())+" "+QString::number(d.year())+")");
                            }
                            else if (reader.name() == "content"){
                                ui->blogTextEdit->append(reader.readElementText());
                                ui->blogTextEdit->append(" ");
                            }
                            else{
                                reader.skipCurrentElement();
                            }
                        }
                    }
                    else
                        reader.skipCurrentElement();
                }
            }
            else
                reader.raiseError(QObject::tr("Incorrect file"));
    }

    ui->blogTextEdit->verticalScrollBar()->setValue(0);

}

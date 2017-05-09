#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

void MainWindow::on_actionRadio_clicked(){
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_rivendellPlayButton_clicked()
{
    radioTimer->start();
    radioPlayer->setMedia(QUrl("http://radiorivendell.de/"));
    musicPlayer->stop();
    radioActive = true;
    radioID = 0;
    radioPlayer->play();

    ui->musicTitleLabel->setText("Radio: Radio Rivendell");
    ui->musicAlbumLabel->setText("");
    ui->musicArtistLabel->setText("");
    ui->musicYearLabel->setText("");

    ui->musicNextButton->setDisabled(true);
    ui->musicReplayButton->setDisabled(true);
}

void MainWindow::on_rivendellReloadButton_clicked()
{
    if (radioActive){
        radioPlayer->pause();
        radioTimer->start();
        radioPlayer->setMedia(QUrl("http://radiorivendell.de/"));

        ui->musicTitleLabel->setText("Radio: Radio Rivendell");
        ui->musicAlbumLabel->setText("");
        ui->musicArtistLabel->setText("");
        ui->musicYearLabel->setText("");

        radioPlayer->play();
    }
}

void MainWindow::on_radioNetworkAccessManager_finished(QNetworkReply* reply){
    qDebug() << "FINISHED";
    qDebug() << QString::number(radioID);

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    qDebug() << redirectionTarget.toString();

    if (!redirectionTarget.isNull()) {
        QUrl newUrl = QUrl(redirectionTarget.toUrl());
        QNetworkRequest request(newUrl);
        radioNetworkManager->get(request);
    } else {
        QString replyString = reply->readAll();

        switch(radioID){
        case 0:
        {
            QTextDocument doc;
            doc.setHtml(replyString);
            replyString = doc.toPlainText();
            doc.clear();
            doc.deleteLater();

            // Gets Meta Info from string
            int startIndex = replyString.indexOf("GMT +1.");
            int timeIndex = replyString.indexOf(":", startIndex);
            QString temp = replyString;
            temp = temp.mid(startIndex, timeIndex-startIndex);

            qDebug() << temp;

            int nowIndex = temp.indexOf("Now");
            int lineIndex = temp.indexOf("\n", nowIndex+5);

            QString title = temp.mid(nowIndex+4);
            QString artist = temp.mid(lineIndex+1);

            title = title.replace(artist, "");
            title = title.replace("Spotify", "");
            title = title.left(title.length()-1);
            artist = artist.left(artist.length()-3);

            qDebug() << "Title: " << title;
            qDebug() << "Artist: " << artist;

            ui->musicAlbumLabel->setText("Title: "+title);
            ui->musicArtistLabel->setText("Artist: "+artist);

            replyString.clear();
            temp.clear();
            break;
        }
        case 1:
        {
            QTextDocument doc;
            doc.setHtml(replyString);
            replyString = doc.toPlainText();
            qDebug() << replyString;
            doc.clear();
            doc.deleteLater();

            // Gets Meta Info from string
            int artistStartIndex = replyString.indexOf("Jetzt läuft ");
            int titleStartIndex = replyString.indexOf("mit dem Titel »", artistStartIndex);
            int albumStartIndex = replyString.indexOf("vom Album »", titleStartIndex);
            int albumEndIndex = replyString.indexOf("«", albumStartIndex);
            QString temp = replyString;

            temp = temp.mid(artistStartIndex, albumEndIndex-artistStartIndex);

            ui->musicTitleLabel->setText("Radio: MMORPG Radio");
            ui->musicAlbumLabel->setText("Metadata Information");
            ui->musicArtistLabel->setText("are currently not");
            ui->musicYearLabel->setText("supported. Sorry.");

            break;
        }
        default:
        {
            qDebug() << "Invalid RadioID";
        }
        }
    }
    reply->deleteLater();
}

void MainWindow::on_radioTimer_timeout(){
    qDebug() << "TIMER FINISHED";
    switch (radioID) {
    case 0:
        radioNetworkManager->get(QNetworkRequest(QUrl("https://www.radiorivendell.com/page/last-played/")));
        radioTimer->start();
        break;
    case 1:
        radioNetworkManager->get(QNetworkRequest(QUrl("http://laut.fm/mmorpg")));
        break;
    default:
        qDebug() << "ERROR: RadioID is not correct";
        break;
    }
}


void MainWindow::on_mmorpgPlayButton_clicked()
{
    //radioTimer->start();
    radioPlayer->setMedia(QUrl("http://stream3.laut.fm/mmorpg"));
    musicPlayer->stop();
    radioActive = true;
    radioID = 1;
    radioPlayer->play();

    ui->musicTitleLabel->setText("Radio: MMORPG Radio");
    ui->musicAlbumLabel->setText("Metadata Information");
    ui->musicArtistLabel->setText("are currently not");
    ui->musicYearLabel->setText("supported. Sorry.");

    ui->musicNextButton->setDisabled(true);
    ui->musicReplayButton->setDisabled(true);
}

void MainWindow::on_mmorpgReloadButton_clicked()
{
    if (radioActive){
        //radioTimer->start();
        radioPlayer->pause();
        radioPlayer->setMedia(QUrl("http://stream3.laut.fm/mmorpg"));

        ui->musicTitleLabel->setText("Radio: MMORPG Radio");
        ui->musicAlbumLabel->setText("Metadata Information");
        ui->musicArtistLabel->setText("are currently not");
        ui->musicYearLabel->setText("supported. Sorry.");

        radioPlayer->play();
    }
}

void MainWindow::on_radioMetaDataChanged(){
//    QString title = radioPlayer->metaData(QStringLiteral("Title")).toString();
//    QString album = radioPlayer->metaData(QStringLiteral("AlbumTitle")).toString();
//    QString artist = musicPlayer->metaData(QStringLiteral("Author")).toString();
//    if (radioPlayer->isMetaDataAvailable()){
//        ui->musicAlbumLabel->setText("Title: "+title);
//        ui->musicArtistLabel->setText("Artist: "+artist);
//        ui->musicYearLabel->setText("Album: "+album);

//        ui->musicCoverLabel->setToolTip(album+": "+title);
//    }
//    else{
//        ui->musicAlbumLabel->setText("Metadata Information");
//        ui->musicArtistLabel->setText("are currently not");
//        ui->musicYearLabel->setText("supported. Sorry.");
//    }
    ui->musicAlbumLabel->setText("Metadata Information");
    ui->musicArtistLabel->setText("are currently not");
    ui->musicYearLabel->setText("supported. Sorry.");
}

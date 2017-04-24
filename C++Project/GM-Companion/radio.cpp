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
    ui->textEdit->clear();
    ui->textEdit->append("FINISHED \n");
    ui->textEdit->append(QString::number(radioID)+"\n");

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    ui->textEdit->append(redirectionTarget.toString());

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

            ui->textEdit->append(temp+"\n");

            int nowIndex = temp.indexOf("Now");
            int lineIndex = temp.indexOf("\n", nowIndex+5);

            QString title = temp.mid(nowIndex+4);
            QString artist = temp.mid(lineIndex+1);

            title = title.replace(artist, "");
            title = title.replace("Spotify", "");
            title = title.left(title.length()-1);
            artist = artist.left(artist.length()-3);

            ui->textEdit->append("Title: "+title);
            ui->textEdit->append("Artist: "+artist);

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
            ui->textEdit->append(replyString+"\n");
            doc.clear();
            doc.deleteLater();

            // Gets Meta Info from string
            int artistStartIndex = replyString.indexOf("Jetzt läuft ");
            int titleStartIndex = replyString.indexOf("mit dem Titel »", artistStartIndex);
            int albumStartIndex = replyString.indexOf("vom Album »", titleStartIndex);
            int albumEndIndex = replyString.indexOf("«", albumStartIndex);
            QString temp = replyString;

            temp = temp.mid(artistStartIndex, albumEndIndex-artistStartIndex);


//            int nowIndex = temp.indexOf("Now");
//            int lineIndex = temp.indexOf("\n", nowIndex+5);

//            QString title = temp.mid(nowIndex+4);
//            QString artist = temp.mid(lineIndex+1);

//            title = title.replace(artist, "");
//            title = title.replace("Spotify", "");
//            title = title.left(title.length()-1);
//            artist = artist.left(artist.length()-3);

//            ui->textEdit->append("Title: "+title);
//            ui->textEdit->append("Artist: "+artist);

//            ui->musicAlbumLabel->setText("Title: "+title);
//            ui->musicArtistLabel->setText("Artist: "+artist);

//            replyString.clear();
//            temp.clear();
            break;
        }
        default:
        {
            ui->textEdit->append("Invalid RadioID \n");
        }
        }
    }
    reply->deleteLater();
}

void MainWindow::on_radioTimer_timeout(){
    ui->textEdit->append("TIMER FINISHED");
    switch (radioID) {
    case 0:
        radioNetworkManager->get(QNetworkRequest(QUrl("https://www.radiorivendell.com/page/last-played/")));
        radioTimer->start();
        break;
    case 1:
        radioNetworkManager->get(QNetworkRequest(QUrl("http://laut.fm/mmorpg")));
        break;
    default:
        ui->textEdit->append("ERROR: RadioID is not correct \n");
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
    QString title = radioPlayer->metaData(QStringLiteral("Title")).toString();
    QString album = radioPlayer->metaData(QStringLiteral("AlbumTitle")).toString();
    QString artist = musicPlayer->metaData(QStringLiteral("Author")).toString();
    if (radioPlayer->isMetaDataAvailable()){
        ui->musicAlbumLabel->setText("Title: "+title);
        ui->musicArtistLabel->setText("Artist: "+artist);
        ui->musicYearLabel->setText("Album: "+album);

        ui->musicCoverLabel->setToolTip(album+": "+title);
    }
    else{
        ui->musicAlbumLabel->setText("Metadata Information");
        ui->musicArtistLabel->setText("are currently not");
        ui->musicYearLabel->setText("supported. Sorry.");
    }
}

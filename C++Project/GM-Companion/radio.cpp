#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

void MainWindow::on_actionInternet_Radio_triggered(){
//    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_rivendellPlayButton_clicked()
{
    radioPlayer->setMedia(QUrl("http://radiorivendell.de/"));
    musicPlayer->stop();
    radioActive = true;
    radioID = 0;
    radioPlayer->play();

//    ui->musicLabel->setText("Radio");
    ui->musicTitleLabel->setText("Radio: Radio Rivendell");
//    ui->musicAlbumLabel->setText("");
//    ui->musicArtistLabel->setText("");
    ui->musicCoverLabel->clear();

    ui->musicNextButton->setDisabled(true);
    ui->musicReplayButton->setDisabled(true);
    ui->musicRandomButton->setDisabled(true);
}

void MainWindow::on_rivendellReloadButton_clicked()
{
    if (radioActive){
        radioPlayer->pause();
        radioPlayer->setMedia(QUrl("http://radiorivendell.de/"));

//        ui->musicLabel->setText("Radio");
        ui->musicTitleLabel->setText("Radio: Radio Rivendell");
//        ui->musicAlbumLabel->setText("");
//        ui->musicArtistLabel->setText("");
        ui->musicCoverLabel->clear();

        radioPlayer->play();
    }
}

void MainWindow::on_mmorpgPlayButton_clicked()
{
    radioPlayer->setMedia(QUrl("http://stream3.laut.fm/mmorpg"));
    musicPlayer->stop();
    radioActive = true;
    radioID = 1;
    radioPlayer->play();

//    ui->musicLabel->setText("Radio");
    ui->musicTitleLabel->setText("Radio: MMORPG Radio");
//    ui->musicAlbumLabel->setText("");
//    ui->musicArtistLabel->setText("");
    ui->musicCoverLabel->clear();

    ui->musicNextButton->setDisabled(true);
    ui->musicReplayButton->setDisabled(true);
    ui->musicRandomButton->setDisabled(true);
}

void MainWindow::on_mmorpgReloadButton_clicked()
{
    if (radioActive){
        radioPlayer->pause();
        radioPlayer->setMedia(QUrl("http://stream3.laut.fm/mmorpg"));

//        ui->musicLabel->setText("Radio");
        ui->musicTitleLabel->setText("Radio: MMORPG Radio");
//        ui->musicAlbumLabel->setText("");
//        ui->musicArtistLabel->setText("");
        ui->musicCoverLabel->clear();

        radioPlayer->play();
    }
}

void MainWindow::on_radioMetaDataChanged(){

}

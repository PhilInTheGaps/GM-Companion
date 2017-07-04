#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

// Zoom In
void MainWindow::on_mapsZoomInButton_clicked()
{
    if (mapsImageLabel->pixmap() != 0){
        double factor = 1.5 * mapsZoomFactor;
        mapsImageLabel->resize(factor * mapsImageLabel->pixmap()->size());
        mapsZoomFactor = factor;
    }
}

// Zoom Out
void MainWindow::on_mapsZoomOutButton_clicked()
{
    if (mapsImageLabel->pixmap() != 0){
        double factor = 0.75 * mapsZoomFactor;
        mapsImageLabel->resize(factor * mapsImageLabel->pixmap()->size());
        mapsZoomFactor = factor;
    }
}

// Resize maps to fit to view
void MainWindow::on_mapsFitToViewButton_clicked()
{
    if (mapsImageLabel->pixmap() != 0){
        double width = mapsImageLabel->width();
        double height = mapsImageLabel->height();
        double ratio = width/height;
        double factor = 1;
        if (ratio >=1){
            factor = ui->mapsVBox->geometry().width() / width;
            mapsImageLabel->resize(factor * mapsImageLabel->size());
        }
        else{
            factor = ui->mapsControlFrame->height() / height;
            mapsImageLabel->resize(factor * mapsImageLabel->size());
        }
        mapsZoomFactor = mapsZoomFactor*factor;
    }
}

// Reset Map Size
void MainWindow::on_mapsResetSizeButton_clicked()
{
    if (mapsImageLabel->pixmap() != 0){
        mapsImageLabel->resize(mapsImageLabel->pixmap()->size());
        mapsZoomFactor = 1.0;
    }
}

// Generate UI
void MainWindow::generateMaps(){
    QStringList mapsList = getFiles(settingsManager->getSetting(Setting::mapsPath));

    mapsImageLabel = new QLabel();
    mapsImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mapsImageLabel->setScaledContents(true);

    QFrame* mapsButtonFrame = new QFrame;
    mapsButtonFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mapsVBox->addWidget(mapsButtonFrame);

    QScrollArea *mapsScrollArea = new QScrollArea;
    mapsScrollArea->setWidget(mapsImageLabel);

    FlowLayout *mapsFlowLayout = new FlowLayout;
    mapsButtonFrame->setLayout(mapsFlowLayout);

    ui->mapsVBox->addWidget(mapsScrollArea);

    for (QString mapName : mapsList){
        if (mapName.contains(".png") || mapName.contains(".jpg")){
            QString mapPath = settingsManager->getSetting(Setting::mapsPath)+"/"+mapName;

            QPushButton *imageButton = new QPushButton;
            imageButton->setText(cleanText(mapName));
            mapsFlowLayout->addWidget(imageButton);

            connect(imageButton, SIGNAL(clicked()), signalMapperMaps, SLOT(map()));
            signalMapperMaps->setMapping(imageButton, mapPath);

        }
    }
}

// Display a map
void MainWindow::setMap(QString mapPath){
    mapsZoomFactor = 1.0;
    mapsImageLabel->setPixmap(QPixmap(mapPath));
    mapsImageLabel->adjustSize();
}

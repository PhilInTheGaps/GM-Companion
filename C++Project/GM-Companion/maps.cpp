#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

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

            ui->textEdit->append(mapPath+"\n");
        }
    }
}

void MainWindow::setMap(QString mapPath){
    mapsZoomFactor = 1.0;
    ui->textEdit->append("Map: "+mapPath+"\n");
    mapsImageLabel->setPixmap(QPixmap(mapPath));
    mapsImageLabel->adjustSize();
}

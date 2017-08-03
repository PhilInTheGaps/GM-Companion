#include "mapviewertool.h"
#include "ui_mapviewertool.h"
#include "flowlayout.h"
#include "managers/settingsmanager.h"
#include "functions.h"

#include <QDebug>
#include <QScrollArea>

MapViewerTool::MapViewerTool(QWidget *parent) : QWidget(parent), ui(new Ui::MapViewerTool)
{
    qDebug() << "Loading Map Viewer Tool ...";

    ui->setupUi(this);

    // Initialize Signal Mapper
    signalMapperMaps = new QSignalMapper(this);
    connect(signalMapperMaps, SIGNAL(mapped(QString)), this, SLOT(setMap(QString)));

    // Create ScrollArea
    QScrollArea *mapsScrollArea = new QScrollArea;
    ui->verticalLayout->addWidget(mapsScrollArea);

    // Create MapLabel
    mapLabel = new QLabel;
    mapLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mapLabel->setScaledContents(true);

    mapsScrollArea->setWidget(mapLabel);

    getMaps();
}

MapViewerTool::~MapViewerTool()
{
    delete ui;
}

// Create buttons for all the maps
void MapViewerTool::getMaps()
{
    // Clear all old buttons
    qDeleteAll(ui->frame_mapButtons->children());

    // Set map button layout
    FlowLayout *mapButtonLayout = new FlowLayout;
    ui->frame_mapButtons->setLayout(mapButtonLayout);

    SettingsManager *settingsManager = new SettingsManager;
    QStringList mapsList = getFiles(settingsManager->getSetting(Setting::mapsPath));

    for (QString mapName : mapsList){
        if (mapName.contains(".png") || mapName.contains(".jpg")){
            QString mapPath = settingsManager->getSetting(Setting::mapsPath)+"/"+mapName;

            QPushButton *imageButton = new QPushButton;
            imageButton->setText(cleanText(mapName));
            mapButtonLayout->addWidget(imageButton);

            connect(imageButton, SIGNAL(clicked()), signalMapperMaps, SLOT(map()));
            signalMapperMaps->setMapping(imageButton, mapPath);
        }
    }
}

// Display a map
void MapViewerTool::setMap(QString mapPath){
    mapsZoomFactor = 1.0;

    mapLabel->setPixmap(QPixmap(mapPath));
    mapLabel->adjustSize();
}

// Zoom In
void MapViewerTool::on_pushButton_zoomIn_clicked()
{
    if (mapLabel->pixmap() != 0){
        // Multiply current zoom factor with 1.5
        double factor = 1.5 * mapsZoomFactor;

        // Resize label
        mapLabel->resize(factor * mapLabel->pixmap()->size());

        // Set new zoom factor
        mapsZoomFactor = factor;
    }
}

// Zoom Out
void MapViewerTool::on_pushButton_zoomOut_clicked()
{
    if (mapLabel->pixmap() != 0){
        // Multiply current zoom factor with 0.75
        double factor = 0.75 * mapsZoomFactor;

        // Resize label
        mapLabel->resize(factor * mapLabel->pixmap()->size());

        // Set new zoom factor
        mapsZoomFactor = factor;
    }
}

// Set label size to fit into the view
void MapViewerTool::on_pushButton_fitToView_clicked()
{
    if (mapLabel->pixmap() != 0){
        double width = mapLabel->width();
        double height = mapLabel->height();
        double ratio = width/height;
        double factor = 1;

        if (ratio >=1){
            factor = ui->frame_mapButtons->geometry().width() / width;
            mapLabel->resize(factor * mapLabel->size());
        }
        else{
            factor = ui->frame_controls->geometry().height() / height;
            mapLabel->resize(factor * mapLabel->size());
        }


        mapsZoomFactor = mapsZoomFactor*factor;
    }
}

// Reset label size back to normal
void MapViewerTool::on_pushButton_resetSize_clicked()
{
    if (mapLabel->pixmap() != 0){
        mapLabel->resize(mapLabel->pixmap()->size());

        mapsZoomFactor = 1.0;
    }
}

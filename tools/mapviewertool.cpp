#include "mapviewertool.h"
#include "ui_mapviewertool.h"
#include "ui/flowlayout.h"
#include "managers/settingsmanager.h"
#include "functions.h"

#include <QDebug>
#include <QScrollArea>
#include <QGraphicsScene>

MapViewerTool::MapViewerTool(QWidget *parent) : QWidget(parent), ui(new Ui::MapViewerTool)
{
    qDebug() << "Loading Map Viewer Tool ...";

    ui->setupUi(this);

    // Initialize Signal Mapper
    signalMapperMaps = new QSignalMapper(this);
    connect(signalMapperMaps, SIGNAL(mapped(QString)), this, SLOT(setMap(QString)));

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
void MapViewerTool::setMap(QString mapPath)
{
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);

    scene->addPixmap(QPixmap(mapPath));
}

// Zoom In
void MapViewerTool::on_pushButton_zoomIn_clicked()
{
    ui->graphicsView->scale(1.5, 1.5);
}

// Zoom Out
void MapViewerTool::on_pushButton_zoomOut_clicked()
{
    ui->graphicsView->scale(0.75, 0.75);
}

// Reset label size back to normal
void MapViewerTool::on_pushButton_resetSize_clicked()
{
    ui->graphicsView->resetTransform();
}

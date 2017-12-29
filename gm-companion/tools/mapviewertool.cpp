#include "mapviewertool.h"
#include "ui_mapviewertool.h"
#include "gm-companion/ui/flowlayout.h"
#include "gm-companion/managers/settingsmanager.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QScrollArea>
#include <QGraphicsScene>
#include <QSpacerItem>

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
    SettingsManager *settingsManager = new SettingsManager;

    QString path = settingsManager->getSetting(Setting::mapsPath);

    // Clear all old buttons
    qDeleteAll(ui->scrollAreaWidgetContents->children());

    // Set map button layout
    QVBoxLayout *mapButtonLayout = new QVBoxLayout;
    ui->scrollAreaWidgetContents->setLayout(mapButtonLayout);

    QStringList mapsList = getFiles(path);

    // Create a button for every map
    for (QString mapName : mapsList){
        if (mapName.contains(".png") || mapName.contains(".jpg")){
            QString mapPath = path + "/" + mapName;

            QPushButton *imageButton = new QPushButton;
            imageButton->setText(cleanText(mapName));
            mapButtonLayout->addWidget(imageButton);

            connect(imageButton, SIGNAL(clicked()), signalMapperMaps, SLOT(map()));
            signalMapperMaps->setMapping(imageButton, mapPath);
        }
    }

    // Add a verical spacer
    mapButtonLayout->addItem(new QSpacerItem(0,10, QSizePolicy::Minimum, QSizePolicy::Expanding));
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
    ui->graphicsView->scale(2, 2);
}

// Zoom Out
void MapViewerTool::on_pushButton_zoomOut_clicked()
{
    ui->graphicsView->scale(0.5, 0.5);
}

// Reset label size back to normal
void MapViewerTool::on_pushButton_resetSize_clicked()
{
    ui->graphicsView->resetTransform();
}

void MapViewerTool::on_checkBox_googleDrive_toggled(bool checked)
{
    useGoogleDrive = checked;

    if (checked)
        getMaps();
}

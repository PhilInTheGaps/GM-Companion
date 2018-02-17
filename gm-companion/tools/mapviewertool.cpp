#include "mapviewertool.h"
#include "ui_mapviewertool.h"
#include "gm-companion/ui/flowlayout.h"
#include "gm-companion/settings/settingsmanager.h"
#include "gm-companion/functions.h"

#include <QDebug>
#include <QScrollArea>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSpacerItem>
#include <QPushButton>
#include <QScrollBar>

MapViewerTool::MapViewerTool(QWidget *parent) : QWidget(parent), ui(new Ui::MapViewerTool)
{
    qDebug() << "Loading Map Viewer Tool ...";

    ui->setupUi(this);

    listVisible = true;

    zoom = 1;

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
    ui->scrollArea_mapButtons->setWidgetResizable(true);

    QStringList mapsList = getFiles(path);

    // Create a button for every map
    for (QString mapName : mapsList)
    {
        if (mapName.contains(".png") || mapName.contains(".jpg"))
        {
            QString mapPath = path + "/" + mapName;

            QVBoxLayout *l = new QVBoxLayout;

            QPushButton *imageButton = new QPushButton;
            imageButton->setMaximumWidth(150);
            imageButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
            QPixmap p = QPixmap(mapPath).scaled(145, 145, Qt::KeepAspectRatio, Qt::FastTransformation);

            imageButton->setIcon(QIcon(p));
            imageButton->setIconSize(QSize(145, 145));
            l->addWidget(imageButton);

            QLabel *label = new QLabel;
            label->setText(cleanText(mapName));
            label->setText(cleanText(mapName));
            label->setWordWrap(true);
            l->addWidget(label);
            label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

            connect(imageButton, &QPushButton::clicked, this, [ = ]() { setMap(mapPath); });

            mapButtonLayout->addLayout(l);
        }
    }

    // Add a verical spacer
    mapButtonLayout->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    ui->scrollArea_mapButtons->setMinimumWidth(170 + ui->scrollArea_mapButtons->verticalScrollBar()->sizeHint().width());
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
    float z = (zoom + 0.1) / (zoom);

    zoom += 0.1;

    ui->graphicsView->scale(z, z);
}

// Zoom Out
void MapViewerTool::on_pushButton_zoomOut_clicked()
{
    float z = (zoom - 0.1) / (zoom);

    if (zoom - 0.1 > 0)
    {
        zoom -= 0.1;
        ui->graphicsView->scale(z, z);
    }
}

// Reset label size back to normal
void MapViewerTool::on_pushButton_resetSize_clicked()
{
    ui->graphicsView->resetTransform();
}

// Show or Hide maps list
void MapViewerTool::on_pushButton_toggleMaps_clicked()
{
    if (listVisible)
    {
        listVisible = false;
        ui->scrollArea_mapButtons->setHidden(true);
        ui->pushButton_toggleMaps->setText("Show List");
    }
    else
    {
        listVisible = true;
        ui->scrollArea_mapButtons->setHidden(false);
        ui->pushButton_toggleMaps->setText("Hide List");
    }
}

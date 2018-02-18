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
#include <QTabWidget>

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

    // Add Buttons
    createMapButtons(getFiles(path), path, mapButtonLayout, ui->scrollArea_mapButtons);

    // Addon Buttons
    loadAddonMaps();
}

void MapViewerTool::createMapButtons(QStringList fileList, QString path, QVBoxLayout *mapButtonLayout, QScrollArea *area)
{
    // Create a button for every map
    for (QString mapName : fileList)
    {
        if (mapName.contains(".png") || mapName.contains(".jpg"))
        {
            QString mapPath = path + "/" + mapName;

            QVBoxLayout *l = new QVBoxLayout;

            QPushButton *imageButton = new QPushButton;
            imageButton->setMaximumWidth(130);
            imageButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
            imageButton->setToolTip(cleanText(mapName));
            QPixmap p = QPixmap(mapPath).scaled(125, 125, Qt::KeepAspectRatio, Qt::FastTransformation);

            imageButton->setIcon(QIcon(p));
            imageButton->setIconSize(QSize(125, 125));
            l->addWidget(imageButton);

            QLabel *label = new QLabel;
            label->setText(cleanText(mapName));
            label->setToolTip(cleanText(mapName));
            label->setWordWrap(true);
            l->addWidget(label);
            label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

            connect(imageButton, &QPushButton::clicked, this, [ = ]() { setMap(mapPath); });
            mapButtonLayout->addLayout(l);
        }
    }

    // Add a verical spacer
    mapButtonLayout->addItem(new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    area->setMinimumWidth(130 + ui->scrollArea_mapButtons->verticalScrollBar()->sizeHint().width());
}

void MapViewerTool::loadAddonMaps()
{
    qDebug() << "Loading addon maps ...";
    QString addonPath  = QDir::homePath() + "/.gm-companion/addons";
    QStringList addons = getFolders(addonPath);

    SettingsManager *settingsmanager = new SettingsManager;

    QString path;
    bool    addedMaps = false;

    for (QString addon : addons)
    {
        path = addonPath + "/" + addon + "/maps";
        qDebug() << "Loading addon maps:" << path;

        if (settingsmanager->getIsAddonEnabled(addon) && QDir(path).exists() && !addon.contains("."))
        {
            QScrollArea *area   = new QScrollArea;
            QWidget     *widget = new QWidget;
            QVBoxLayout *layout = new QVBoxLayout;
            widget->setLayout(layout);
            area->setWidget(widget);
            area->setWidgetResizable(true);
            area->setFrameShape(QFrame::NoFrame);

            createMapButtons(getFiles(path), path, layout, area);

            ui->tabWidget->addTab(area, addon);
            addedMaps = true;
        }
    }

    if (addedMaps)
    {
        ui->tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
    zoom = 1;
}

// Show or Hide maps list
void MapViewerTool::on_pushButton_toggleMaps_clicked()
{
    if (listVisible)
    {
        listVisible = false;
        ui->tabWidget->setHidden(true);
        ui->pushButton_toggleMaps->setText("Show List");
    }
    else
    {
        listVisible = true;
        ui->tabWidget->setHidden(false);
        ui->pushButton_toggleMaps->setText("Hide List");
    }
}

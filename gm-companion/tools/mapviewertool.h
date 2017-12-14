#ifndef MAPVIEWERTOOL_H
#define MAPVIEWERTOOL_H

#include <QWidget>
#include <QSignalMapper>
#include <QLabel>

#include "gm-companion/managers/googledrive.h"

namespace Ui {
class MapViewerTool;
}

class MapViewerTool : public QWidget
{
    Q_OBJECT

public:
    explicit MapViewerTool(QWidget *parent = 0);
    ~MapViewerTool();

private slots:
    void on_pushButton_zoomIn_clicked();

    void on_pushButton_zoomOut_clicked();

    void on_pushButton_resetSize_clicked();

    void setMap(QString mapPath);

    void on_checkBox_googleDrive_toggled(bool checked);

private:
    Ui::MapViewerTool *ui;

    QSignalMapper *signalMapperMaps;

    void getMaps();

    GoogleDrive *drive;

    bool useGoogleDrive;
};

#endif // MAPVIEWERTOOL_H

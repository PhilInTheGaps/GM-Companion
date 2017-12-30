#ifndef MAPVIEWERTOOL_H
#define MAPVIEWERTOOL_H

#include <QWidget>
#include <QSignalMapper>
#include <QLabel>

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

private:
    Ui::MapViewerTool *ui;

    QSignalMapper *signalMapperMaps;

    void getMaps();
};

#endif // MAPVIEWERTOOL_H

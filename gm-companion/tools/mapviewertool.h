#ifndef MAPVIEWERTOOL_H
#define MAPVIEWERTOOL_H

#include <QWidget>
#include <QSignalMapper>
#include <QLabel>
#include <QStringList>
#include <QScrollArea>
#include <QVBoxLayout>

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

    void on_pushButton_toggleMaps_clicked();

private:
    Ui::MapViewerTool *ui;

    float zoom;

    void getMaps();
    void createMapButtons(QStringList fileList, QString path, QVBoxLayout *mapButtonLayout, QScrollArea *area);
    void loadAddonMaps();

    void setMap(QString mapPath);
    bool listVisible;
};

#endif // MAPVIEWERTOOL_H

#ifndef MAPSFILEMANAGER_H
#define MAPSFILEMANAGER_H

#include <QObject>
#include <QList>
#include "src/tools/maps/map.h"
#include "src/cloud/googledrive.h"
#include "src/settings/settingsmanager.h"

class MapsFileManager : public QObject
{
    Q_OBJECT
public:
    explicit MapsFileManager(GoogleDrive *google, QObject *parent = nullptr);

    void findMaps(int mode);
    void findMapPaths(int index);

    void saveMarkers(Map* map, QString category);

signals:
    void mapsChanged(QList<MapCategory*> categories);

private:
    SettingsManager sManager;
    GoogleDrive *googleDrive;

    QList<MapCategory*> m_categories;
    int m_mode;
    QList<int> m_requestIDs;

    QList<MapCategory*> getAddonMaps();

private slots:
    void mapsReceived(int reqId, QList<GoogleFile> files);
};

#endif // MAPSFILEMANAGER_H

#ifndef SHOPFILEMANAGER_H
#define SHOPFILEMANAGER_H

#include <QObject>
#include "cloud/googledrive.h"
#include "tools/shop/shopproject.h"
#include "settings/settingsmanager.h"

class ShopFileManager : public QObject
{
    Q_OBJECT
public:
    explicit ShopFileManager(GoogleDrive *google);

    void findShops(int mode, bool editor = false);
    void saveProject(ShopProject *project);
    void findItems(int mode, bool editor = false);
    void saveItems(ItemGroup *group);

signals:
    void receivedShops(QList<ShopProject*> projects);
    void receivedEditorShops(QList<ShopProject*> projects);
    void receivedItems(QList<ItemGroup*> items);
    void receivedEditorItems(ItemGroup *group);

private:
    GoogleDrive *googleDrive;
    SettingsManager sManager;

    int m_mode;

    int m_projectRequestId = -1;
    QList<int> m_projectRequestIds;
    QList<ShopProject*> m_projects;

    int m_itemRequestId = -1;
    QList<int> m_itemRequestIds;
    QList<ItemGroup*> m_itemGroups;

    ShopProject* initProject(QString filePath);

    void findLocalShops(bool editor);
    void findLocalItems(bool editor);
    QList<ItemGroup*> findAddonItems();
    ItemGroup* loadItemGroup(QString groupName, QString path);
};

#endif // SHOPFILEMANAGER_H

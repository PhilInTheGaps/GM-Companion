#include "shopfilemanager.h"
#include "src/functions.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QTemporaryFile>

ShopFileManager::ShopFileManager(GoogleDrive *google)
    : googleDrive(google)
{
    qDebug() << "Loading ShopFileManager ...";

    connect(google, &GoogleDrive::receivedFolderContent, [ = ](int id, QList<GoogleFile>list) {
        if (id == m_projectRequestId)
        {
            for (auto a : list)
            {
                if (a.fileName.endsWith(".shop"))
                {
                    m_projectRequestIds.append(google->getFile(a.id));
                }
                else if (a.fileName.endsWith("CustomItems.items"))
                {
                    m_itemRequestIds.append(google->getFile(a.id));
                }
            }
        }
        else if (id == m_itemRequestId)
        {
            for (auto a : list)
            {
                if (a.fileName.endsWith("CustomItems.items"))
                {
                    m_itemRequestIds.append(google->getFile(a.id));
                }
            }
        }
    });

    connect(google, &GoogleDrive::receivedFile, [ = ](int id, QByteArray data) {
        if (m_projectRequestIds.contains(id))
        {
            QTemporaryFile f;
            f.setAutoRemove(false);

            if (f.open())
            {
                f.write(data);
                f.close();

                auto p = initProject(f.fileName());

                if (p)
                {
                    m_projects.append(p);
                    emit receivedShops(m_projects);
                }
            }
        }
        else if (m_itemRequestIds.contains(id))
        {
            QTemporaryFile f;
            f.setAutoRemove(false);

            if (f.open())
            {
                f.write(data);
                f.close();

                if ((m_itemGroups.size() > 0) && (m_itemGroups[0]->name() != tr("Custom")))
                {
                    auto g = loadItemGroup(tr("Custom"), f.fileName());

                    if (g)
                    {
                        m_itemGroups.insert(0, g);
                        emit receivedItems(m_itemGroups);
                    }
                }
            }
        }
    });
}

void ShopFileManager::findShops(int mode, bool editor)
{
    qDebug() << "ShopFileManager: Finding shops ( mode:" << mode << ") ...";
    m_mode = mode;

    if (editor)
    {
        findLocalShops(editor);
        return;
    }

    switch (mode)
    {
    case 0: // Local
    {
        findLocalShops(editor);
        break;
    }

    case 1: // Google Drive
        m_projectRequestId = googleDrive->getFolderContent(sManager.getSetting(Setting::googlePath, "shop"));
        break;
    }
}

void ShopFileManager::findLocalShops(bool editor)
{
    QList<ShopProject *> projects;
    QString path = sManager.getSetting(Setting::shopPath);

    qDebug() << "ShopFileManager: Path:" << path;

    for (auto f : getFiles(path))
    {
        qDebug() << "   " << f;

        if (f.endsWith(".shop"))
        {
            projects.append(initProject(path + "/" + f));
        }
    }

    if (editor) emit receivedEditorShops(projects);
    else emit receivedShops(projects);
}

ShopProject * ShopFileManager::initProject(QString filePath)
{
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        // Get Categories
        auto root          = doc.object();
        auto categoryArray = root.value("categories").toArray();
        QList<ShopCategory *> categories;

        for (auto c : categoryArray)
        {
            auto co = c.toObject();

            // Get Shops in Category
            QList<ItemShop *> shops;
            auto shopArray = co.value("shops").toArray();

            for (auto s : shopArray)
            {
                auto so = s.toObject();

                // Get Items in Shop
                QList<Item *> items;
                auto itemArray = so.value("items").toArray();

                for (auto i : itemArray)
                {
                    auto io = i.toObject();

                    Item *item = new Item(io.value("name").toString(), io.value("price").toString(), io.value("description").toString(), io.value("category").toString());
                    items.append(item);
                }

                ItemShop *shop = new ItemShop(so.value("name").toString(), so.value("owner").toString(), so.value("description").toString(), items);
                shops.append(shop);
            }


            ShopCategory *category = new ShopCategory(co.value("name").toString(), shops);
            category->setCurrentShop(0);

            categories.append(category);
        }

        ShopProject *project = new ShopProject(root.value("name").toString(), categories);
        project->setCurrentCategory(0);

        return project;
    }
    else
    {
        qWarning() << "Could not open shop file:" << filePath << "!";
    }

    return nullptr;
}

void ShopFileManager::saveProject(ShopProject *project)
{
    if (!project) return;

    QJsonObject projectO;
    projectO.insert("name",    project->name());
    projectO.insert("version", 3);

    QJsonArray categoryArray;

    for (auto c : project->categories())
    {
        if (c)
        {
            QJsonObject categoryO;
            categoryO.insert("name", c->name());

            QJsonArray shopArray;

            for (auto s : c->shops())
            {
                if (s)
                {
                    QJsonObject shopO;
                    shopO.insert("name",        s->name());
                    shopO.insert("owner",       s->owner());
                    shopO.insert("description", s->description());

                    QJsonArray itemArray;

                    for (auto i : s->items())
                    {
                        if (i)
                        {
                            QJsonObject itemO;
                            itemO.insert("name",        i->name());
                            itemO.insert("price",       i->price());
                            itemO.insert("description", i->description());
                            itemO.insert("category",    i->category());
                            itemArray.append(itemO);
                        }
                    }
                    shopO.insert("items", itemArray);
                    shopArray.append(shopO);
                }
            }
            categoryO.insert("shops", shopArray);
            categoryArray.append(categoryO);
        }
    }
    projectO.insert("categories", categoryArray);

    QJsonDocument doc;
    doc.setObject(projectO);

    QFile f(sManager.getSetting(Setting::shopPath) + "/" + project->name() + ".shop");

    if (f.open(QIODevice::WriteOnly))
    {
        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();
    }
}

void ShopFileManager::findItems(int mode, bool editor)
{
    qDebug() << "ShopFileManager: Finding items ( mode:" << mode << ") ...";
    m_mode = mode;

    if (editor)
    {
        findLocalItems(editor);
        return;
    }

    switch (mode)
    {
    case 0: // Local
    {
        findLocalItems(editor);
        break;
    }

    case 1: // Google Drive
        m_itemRequestId = googleDrive->getFolderContent(sManager.getSetting(Setting::googlePath, "shop"));
        m_itemGroups.append(findAddonItems());
        emit receivedItems(m_itemGroups);
        break;
    }
}

void ShopFileManager::findLocalItems(bool editor)
{
    QString path = sManager.getSetting(Setting::shopPath);

    if (editor)
    {
        emit receivedEditorItems(loadItemGroup(tr("Custom"), path + "/CustomItems.items"));
    }
    else
    {
        QList<ItemGroup *> groups;
        groups.append(loadItemGroup(tr("Custom"), path + "/CustomItems.items"));
        groups.append(findAddonItems());
        emit receivedItems(groups);
    }
}

QList<ItemGroup *>ShopFileManager::findAddonItems()
{
    QList<ItemGroup *> groups;
    QStringList paths = { ":/addons", QDir::homePath() + "/.gm-companion/addons" };

    for (auto path : paths)
    {
        for (auto folder : getFolders(path))
        {
            if (!folder.contains("."))
            {
                QSettings settings(path + "/" + folder + "/addon.ini", QSettings::IniFormat);
                auto  addon = settings.value("name").toString();
                auto  file  = path + "/" + folder + "/shop/AddonItems.items";
                QFile f(file);

                if (sManager.getIsAddonEnabled(addon) && (settings.value("addons_version", 0).toInt() > 2) && f.exists())
                {
                    groups.append(loadItemGroup(addon, file));
                }
            }
        }
    }

    return groups;
}

ItemGroup * ShopFileManager::loadItemGroup(QString groupName, QString path)
{
    QList<Item *> items;
    QFile f(path);

    if (f.exists() && f.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
        f.close();

        auto root       = doc.object();
        auto categories = root.value("categories").toArray();

        for (auto c : categories)
        {
            auto category = c.toObject().value("name").toString();

            for (auto i : c.toObject().value("items").toArray())
            {
                auto name        = i.toObject().value("name").toString();
                auto price       = i.toObject().value("price").toString();
                auto description = i.toObject().value("description").toString();

                auto item = new Item(name, price, description, category);
                items.append(item);
            }
        }
    }

    auto group = new ItemGroup(groupName, items);
    return group;
}

void ShopFileManager::saveItems(ItemGroup *group)
{
    if (!group) return;

    QString path = sManager.getSetting(Setting::shopPath) + "/CustomItems.items";

    QJsonObject root;
    root.insert("version", 2);

    QJsonArray  categoryArray;
    QStringList categoryNames;

    // Find all categories
    for (auto i : group->items())
    {
        if (i && !categoryNames.contains(i->category()))
        {
            categoryNames.append(i->category());
        }
    }

    for (auto c : categoryNames)
    {
        QJsonObject category;
        category.insert("name", c);

        QJsonArray itemArray;

        for (auto i : group->items())
        {
            if (i && (i->category() == c))
            {
                QJsonObject item;
                item.insert("name",        i->name());
                item.insert("price",       i->price());
                item.insert("description", i->description());
                item.insert("category",    c);

                itemArray.append(item);
            }
        }

        category.insert("items", itemArray);
        categoryArray.append(category);
    }

    root.insert("categories", categoryArray);
    QJsonDocument doc;
    doc.setObject(root);

    QFile f(path);

    if (f.open(QIODevice::WriteOnly))
    {
        f.write(doc.toJson(QJsonDocument::Indented));
        f.close();
    }
}

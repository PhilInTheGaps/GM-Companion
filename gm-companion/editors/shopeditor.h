#ifndef SHOPEDITOR_H
#define SHOPEDITOR_H

#include <QWidget>
#include <QString>
#include <QTreeWidgetItem>
#include <QTableWidget>

#include "gm-companion/managers/settingsmanager.h"
#include "gm-companion/functions.h"

namespace Ui {
class ShopEditor;
}

class ShopEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ShopEditor(QWidget *parent = 0);
    ~ShopEditor();

private slots:
    void on_pushButton_newProject_clicked();
    void on_pushButton_loadProject_clicked();

    void on_pushButton_newShop_clicked();
    void on_pushButton_newCategory_clicked();

    void on_treeWidget_shops_currentItemChanged(QTreeWidgetItem *current);

    void on_pushButton_saveShop_clicked();

    void on_pushButton_openItemEditor_clicked();
    void on_pushButton_refreshItems_clicked();



    void on_pushButton_clearItems_clicked();

private:
    Ui::ShopEditor *ui;

    SettingsManager *settingsManager;

    QString currentProject;
    QString currentCategory;
    QString currentShop;

    void loadProject(QString project);
    void getProjects();

    void loadShops();
    void loadShop();
    void getCategories();

    void loadItems();

    void addItemToShop(int index, QTableWidget *table);
    void removeItemFromShop(int index);
};

#endif // SHOPEDITOR_H

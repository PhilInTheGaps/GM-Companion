#ifndef SHOPTOOL_H
#define SHOPTOOL_H

#include <QDialog>
#include <QStringList>
#include <QTreeWidgetItem>

#include "gm-companion/settings/settingsmanager.h"

namespace Ui {
class ShopTool;
}

class ShopTool : public QDialog
{
    Q_OBJECT

public:
    explicit ShopTool(QWidget *parent = 0);
    ~ShopTool();

private slots:
    void on_pushButton_loadProject_clicked();

    void on_pushButton_openEditor_clicked();

    void on_comboBox_loadProject_currentTextChanged(const QString &arg1);

    void on_checkBox_setDefault_toggled(bool checked);

    void on_pushButton_updateProjects_clicked();

    void on_treeWidget_shopList_currentItemChanged(QTreeWidgetItem *current);

private:
    Ui::ShopTool *ui;

    SettingsManager *settingsManager;

    void loadProject(QString project);
    void loadShops();
    void loadShop();
    void getProjects();
    QStringList getShopList();

    QString currentProject;
    QString currentCategory;
    QString currentShop;
};

#endif // SHOPTOOL_H

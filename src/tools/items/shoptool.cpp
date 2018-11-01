#include "shoptool.h"
#include "ui_shoptool.h"
#include "gm-companion/tools/items/shopeditor.h"
#include "gm-companion/functions.h"

#include <QDebug>

ShopTool::ShopTool(QWidget *parent) : QDialog(parent), ui(new Ui::ShopTool)
{
    qDebug() << "Loading ShopTool ...";

    ui->setupUi(this);

    settingsManager = new SettingsManager;

    getProjects();

    // Load default project
    QSettings settings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);
    settings.beginGroup("ShopTool");

    for (int i = 0; i < ui->comboBox_loadProject->count(); i++)
    {
        if (ui->comboBox_loadProject->itemText(i) ==
            settings.value("defaultProject").toString())
        {
            qDebug().noquote() << "Loading default project: " + ui->comboBox_loadProject->itemText(i) + " ...";
            ui->comboBox_loadProject->setCurrentIndex(i);
            loadProject(ui->comboBox_loadProject->itemText(i));
        }
    }
}

ShopTool::~ShopTool()
{
    delete ui;
}

void ShopTool::loadProject(QString project)
{
    currentProject = project;
    loadShops();
}

// Refreshes the project list in the combo box
void ShopTool::getProjects()
{
    ui->comboBox_loadProject->clear();

    for (QString file : getFiles(settingsManager->getSetting(Setting::shopPath)))
    {
        if (file.contains(".shop"))
        {
            QString project = file;
            project = project.replace(".shop", "");
            ui->comboBox_loadProject->addItem(project);
        }
    }
}

// Load all shops into the tree view
void ShopTool::loadShops()
{
    qDebug() << "Loading shops ...";

    ui->treeWidget_shopList->clear();

    QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
    QSettings settings(projectFolder + "/" + currentProject + ".shop", QSettings::IniFormat);

    // Get all categories
    QStringList categories;
    int count = settings.beginReadArray("categories");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);
        categories.push_back(settings.value("name", tr("UNKNOWN CATEGORY")).toString());
    }
    settings.endArray();

    // Get all shops
    for (QString category : categories)
    {
        // Create top level category item
        QTreeWidgetItem *baseItem = new QTreeWidgetItem(0);
        baseItem->setText(0, category);
        baseItem->setToolTip(0, category);
        ui->treeWidget_shopList->addTopLevelItem(baseItem);
        baseItem->setExpanded(true);

        int shops = settings.beginReadArray(category + "_shops");

        for (int i = 0; i < shops; i++)
        {
            settings.setArrayIndex(i);
            QString shop = settings.value("name", "NULL").toString();

            if (shop != "NULL")
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(1);
                item->setText(0, shop);
                item->setToolTip(0, shop);
                baseItem->addChild(item);

                if ((shop == currentShop) &&
                    (category ==
                     currentCategory)) ui->treeWidget_shopList->setCurrentItem(
                        item);
            }
        }
        settings.endArray();
    }
}

// Load the current shop
void ShopTool::loadShop()
{
    if (!currentShop.isNull() && !currentCategory.isNull())
    {
        QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
        QSettings settings(projectFolder + "/" + currentProject + ".shop", QSettings::IniFormat);

        // Clear old info
        ui->label_shopTitle->clear();
        ui->label_shopOwner->clear();
        ui->label_shopDescription->clear();

        ui->tableWidget_items->clearContents();
        ui->tableWidget_items->setRowCount(0);

        // Get new shop info
        int shops = settings.beginReadArray(currentCategory + "_shops");

        for (int i = 0; i < shops; i++)
        {
            settings.setArrayIndex(i);

            // Find shop in list
            if (settings.value("name", "").toString() == currentShop)
            {
                ui->label_shopTitle->setText(settings.value("name", tr("UNKNOWN SHOP")).toString());
                ui->label_shopOwner->setText(settings.value("owner", "").toString());
                ui->label_shopDescription->setText(settings.value("description", "").toString());

                // Add items
                int items = settings.beginReadArray("items");

                QStringList categories;

                // Get all categories
                for (int j = 0; j < items; j++)
                {
                    settings.setArrayIndex(j);

                    QString category = settings.value("category", tr("Unknown Category")).toString();

                    if (!categories.contains(category))
                    {
                        categories.append(category);
                        qDebug() << category;
                    }
                }

                int rowIndex = 0;

                // Get all items
                for (QString category : categories)
                {
                    ui->tableWidget_items->insertRow(rowIndex);

                    QTableWidgetItem *catItem = new QTableWidgetItem;
                    catItem->setText(category);
                    catItem->setToolTip(category);

                    QFont f = catItem->font();
                    f.setBold(true);
                    catItem->setFont(f);

                    ui->tableWidget_items->setItem(rowIndex, 0, catItem);
                    rowIndex++;

                    for (int j = 0; j < items; j++)
                    {
                        settings.setArrayIndex(j);

                        // If item belongs to category
                        if (category == settings.value("category", tr("Unknown Category")).toString())
                        {
                            ui->tableWidget_items->insertRow(rowIndex);

                            QString name        = settings.value("name", tr("Unknown Item")).toString();
                            QString price       = settings.value("price", "").toString();
                            QString description = settings.value("description", "").toString();

                            QStringList info =
                            { name, price, description, category };

                            for (int k = 0; k < 3; k++)
                            {
                                QTableWidgetItem *item = new QTableWidgetItem;
                                item->setText(info.at(k));
                                item->setToolTip(info.at(k));

                                // Make Price align right
                                if (k == 1)
                                {
                                    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                                    item->setText(info.at(k) + "  ");
                                }

                                ui->tableWidget_items->setItem(rowIndex, k, item);
                            }

                            rowIndex++;
                        }
                    }

                    // Insert empty line after category is complete
                    ui->tableWidget_items->insertRow(rowIndex);
                    rowIndex++;
                }
                settings.endArray();

                // Remove last empty line
                ui->tableWidget_items->setRowCount(rowIndex - 1);
            }
        }
        settings.endArray();

        ui->tableWidget_items->resizeColumnToContents(1);
    }
}

QStringList ShopTool::getShopList()
{
    QStringList list;

    return list;
}

void ShopTool::on_pushButton_loadProject_clicked()
{
    loadProject(ui->comboBox_loadProject->currentText());
}

// Open Shop Editor
void ShopTool::on_pushButton_openEditor_clicked()
{
    ShopEditor *editor = new ShopEditor;

    editor->showMaximized();
}

// When a project is selected in the combo box
void ShopTool::on_comboBox_loadProject_currentTextChanged(const QString& arg1)
{
    if (!arg1.isNull())
    {
        QSettings settings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);
        settings.beginGroup("ShopTool");

        if (arg1 == settings.value("defaultProject", "").toString()) ui->checkBox_setDefault->setChecked(true);
        else ui->checkBox_setDefault->setChecked(false);

        settings.endGroup();
    }
}

// When "set as default" combo box is toggled
void ShopTool::on_checkBox_setDefault_toggled(bool checked)
{
    QSettings settings(QDir::homePath() + "/.gm-companion/settings.ini", QSettings::IniFormat);

    settings.beginGroup("ShopTool");

    if (checked)
    {
        settings.setValue("defaultProject", ui->comboBox_loadProject->currentText());
    }
    else
    {
        if (ui->comboBox_loadProject->currentText() == settings.value("defaultProject", "").toString())
        {
            settings.setValue("defaultProject", "");
        }
    }
    settings.endGroup();
}

// Refresh project list
void ShopTool::on_pushButton_updateProjects_clicked()
{
    getProjects();
}

// When the currently selected tree item changes
void ShopTool::on_treeWidget_shopList_currentItemChanged(QTreeWidgetItem *current)
{
    if (current != NULL)
    {
        if (current->type() == 0) // Category
        {
            currentCategory = current->text(0);
            currentShop     = "";
        }
        else if (current->type() == 1) // Shop
        {
            currentCategory = current->parent()->text(0);
            currentShop     = current->text(0);

            loadShop();
        }
    }
}

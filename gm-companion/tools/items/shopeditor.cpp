#include "shopeditor.h"
#include "ui_shopeditor.h"
#include "gm-companion/tools/items/itemeditor.h"

#include <QDebug>
#include <QSettings>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

ShopEditor::ShopEditor(QWidget *parent) : QWidget(parent), ui(new Ui::ShopEditor)
{
    qDebug() << "Loading Shop Editor ...";

    ui->setupUi(this);

    settingsManager = new SettingsManager;

    getProjects();

    loadItems();
}

ShopEditor::~ShopEditor()
{
    delete ui;
}

// Create a new project
void ShopEditor::on_pushButton_newProject_clicked()
{
    if (!ui->lineEdit_newProject->text().isNull())
    {
        // Load project and clear line edit
        loadProject(ui->lineEdit_newProject->text());
        ui->lineEdit_newProject->clear();

        getProjects();
    }
    else
    {
        qDebug() << "Error: No project name set!";
    }
}

// Load a project
void ShopEditor::loadProject(QString project)
{
    currentProject = project;

    QString projectFolder = settingsManager->getSetting(Setting::shopPath);
    qDebug() << "Creating new shop project: " + projectFolder + "/" + project +
        ".shop";

    QSettings settings(projectFolder + "/" + project + ".shop",
                       QSettings::IniFormat);

    settings.setValue("project_name", project);

    getCategories();
    loadShops();
}

// Refreshes the project list in the combo box
void ShopEditor::getProjects()
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

// Refreshes the category list in the combo box
void ShopEditor::getCategories()
{
    if (!currentProject.isNull())
    {
        QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
        QSettings settings(projectFolder + "/" + currentProject + ".shop",
                           QSettings::IniFormat);

        QStringList categories;
        int count = settings.beginReadArray("categories");

        for (int i = 0; i < count; i++)
        {
            settings.setArrayIndex(i);
            QString category = settings.value("name", "").toString();

            if (!category.isNull()) categories.push_back(category);
        }
        settings.endArray();

        ui->comboBox_newShop->clear();
        ui->comboBox_newShop->addItems(categories);
    }
}

// Create new shop
void ShopEditor::on_pushButton_newShop_clicked()
{
    if (!ui->lineEdit_newShop->text().isNull() &&
        !ui->comboBox_newShop->currentText().isNull() && !currentProject.isNull())
    {
        currentShop     = ui->lineEdit_newShop->text();
        currentCategory = ui->comboBox_newShop->currentText();
        ui->lineEdit_newShop->clear();

        qDebug().noquote() << "Creating new shop:" << currentShop << "in" <<
            currentCategory;

        QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
        QSettings settings(projectFolder + "/" + currentProject + ".shop",
                           QSettings::IniFormat);

        // Write shop data to project file
        int index = settings.beginReadArray(currentCategory + "_shops");
        settings.endArray();

        settings.beginWriteArray(currentCategory + "_shops");
        settings.setArrayIndex(index);
        settings.setValue("name", currentShop);
        settings.endArray();

        loadShops();
    }
}

// Create new category
void ShopEditor::on_pushButton_newCategory_clicked()
{
    if (!ui->lineEdit_newCategory->text().isNull() && !currentProject.isNull())
    {
        QString category = ui->lineEdit_newCategory->text();
        ui->lineEdit_newCategory->clear();

        QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
        QSettings settings(projectFolder + "/" + currentProject + ".shop",
                           QSettings::IniFormat);

        int index = settings.beginReadArray("categories");
        settings.endArray();

        settings.beginWriteArray("categories");
        settings.setArrayIndex(index);
        settings.setValue("name", category);
        settings.endArray();

        getCategories();
        ui->comboBox_newShop->setCurrentText(category);
    }
}

// Load all shops to tree view
void ShopEditor::loadShops()
{
    qDebug() << "Loading shops ...";

    ui->treeWidget_shops->clear();

    QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
    QSettings settings(projectFolder + "/" + currentProject + ".shop",
                       QSettings::IniFormat);

    // Get all categories
    QStringList categories;
    int count = settings.beginReadArray("categories");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);
        categories.push_back(settings.value("name",
                                            "UNKNOWN CATEGORY").toString());
    }
    settings.endArray();

    // Get all shops
    for (QString category : categories)
    {
        // Create top level category item
        QTreeWidgetItem *baseItem = new QTreeWidgetItem(0);
        baseItem->setText(0, category);
        ui->treeWidget_shops->addTopLevelItem(baseItem);
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
                baseItem->addChild(item);

                if ((shop == currentShop) &&
                    (category ==
                     currentCategory)) ui->treeWidget_shops->setCurrentItem(item);
            }
        }
        settings.endArray();
    }
}

// Load the current shop
void ShopEditor::loadShop()
{
    if (!currentShop.isNull() && !currentCategory.isNull())
    {
        QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
        QSettings settings(projectFolder + "/" + currentProject + ".shop",
                           QSettings::IniFormat);

        // Clear old info
        ui->label_shopName->clear();
        ui->lineEdit_shopOwner->clear();
        ui->lineEdit_shopDescription->clear();

        ui->tableWidget_shopItems->clearContents();
        ui->tableWidget_shopItems->setRowCount(0);

        // Get new shop info
        int shops = settings.beginReadArray(currentCategory + "_shops");

        for (int i = 0; i < shops; i++)
        {
            settings.setArrayIndex(i);

            // Find shop in list
            if (settings.value("name", "").toString() == currentShop)
            {
                ui->label_shopName->setText(settings.value("name",
                                                           "UNKNOWN SHOP").toString());
                ui->lineEdit_shopOwner->setText(settings.value("owner",
                                                               "").toString());
                ui->lineEdit_shopDescription->setText(settings.value("description",
                                                                     "").toString());

                // Add items
                int items = settings.beginReadArray("items");

                for (int j = 0; j < items; j++)
                {
                    settings.setArrayIndex(j);
                    ui->tableWidget_shopItems->insertRow(j);

                    QString name =
                        settings.value("name", "UNKNOWN ITEM").toString();
                    QString price =
                        settings.value("price", "UNKNOWN PRICE").toString();
                    QString description =
                        settings.value("description", "").toString();
                    QString category = settings.value("category",
                                                      "UNKNOWN CATEGORY").
                                       toString();

                    QStringList info = { name, price, description, category };

                    for (int k = 0; k < 4; k++)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem;
                        item->setText(info.at(k));
                        ui->tableWidget_shopItems->setItem(j, k + 1, item);
                    }

                    // Add "Remove" button
                    QWidget *widget     = new QWidget;
                    QPushButton *button = new QPushButton;
                    button->setText("Remove");
                    button->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);
                    widget->setSizePolicy(QSizePolicy::Minimum,
                                          QSizePolicy::Minimum);

                    connect(button, &QPushButton::clicked, this, [ = ]() {
                        removeItemFromShop(j);
                    });

                    QHBoxLayout *bLayout = new QHBoxLayout(widget);
                    bLayout->addWidget(button);
                    bLayout->setAlignment(Qt::AlignCenter);
                    bLayout->setContentsMargins(0, 0, 0, 0);
                    widget->setLayout(bLayout);
                    ui->tableWidget_shopItems->setCellWidget(j, 0, widget);
                }
                settings.endArray();
            }
        }
        settings.endArray();
    }
}

void ShopEditor::loadItems()
{
    QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
    QSettings settings(projectFolder + "/CustomItems.items",
                       QSettings::IniFormat);

    // Remove addon tabs
    for (int i = ui->tabWidget_itemLists->count() - 1; i > 0; i--)
    {
        ui->tabWidget_itemLists->removeTab(i);
    }

    // Custom Items
    ui->tableWidget_customItems->clearContents();
    ui->tableWidget_customItems->setRowCount(0);

    int count = settings.beginReadArray("items");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        QStringList info = { "", "", "", "" };

        info.replace(0, settings.value("name", "").toString());
        info.replace(1, settings.value("price", "").toString());
        info.replace(2, settings.value("category", "").toString());
        info.replace(3, settings.value("description", "").toString());

        ui->tableWidget_customItems->insertRow(i);

        for (int j = 0; j < 4; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(info.at(j));
            ui->tableWidget_customItems->setItem(i, j + 1, item);
        }

        // Add "add" button
        QWidget *widget     = new QWidget;
        QPushButton *button = new QPushButton;
        button->setText("Add");
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        connect(button, &QPushButton::clicked, this, [ = ]() {
            addItemToShop(i, ui->tableWidget_customItems);
        });

        QHBoxLayout *bLayout = new QHBoxLayout(widget);
        bLayout->addWidget(button);
        bLayout->setAlignment(Qt::AlignCenter);
        bLayout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(bLayout);
        ui->tableWidget_customItems->setCellWidget(i, 0, widget);
    }

    // Addon Items
    QString addonsFolder = QDir::homePath() + "/.gm-companion/addons";

    for (QString addon : getFolders(addonsFolder))
    {
        if (!addon.contains("."))
        {
            if (settingsManager->getIsAddonEnabled(addon) &&
                QDir(addonsFolder + "/" + addon + "/shop").exists())
            {
                QTableWidget *table = new QTableWidget;
                table->setColumnCount(5);
                table->setHorizontalHeaderLabels({ "", "Item", "Price",
                                                   "Category", "Description" });
                table->horizontalHeader()->setStretchLastSection(true);
                table->setSortingEnabled(true);
                table->verticalHeader()->hide();

                ui->tabWidget_itemLists->addTab(table, addon);

                QString   folder = addonsFolder + "/" + addon + "/shop";
                QSettings addonSettings(folder + "/AddonItems.items",
                                        QSettings::IniFormat);

                int itemCount = addonSettings.beginReadArray("items");

                for (int i = 0; i < itemCount; i++)
                {
                    addonSettings.setArrayIndex(i);

                    QStringList info = { "", "", "", "" };

                    info.replace(0, addonSettings.value("name", "").toString());
                    info.replace(1, addonSettings.value("price", "").toString());
                    info.replace(2,
                                 addonSettings.value("category", "").toString());
                    info.replace(3, addonSettings.value("description",
                                                        "").toString());

                    table->insertRow(i);

                    for (int j = 0; j < 4; j++)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem;
                        item->setText(info.at(j));
                        table->setItem(i, j + 1, item);
                    }

                    // Add "add" button
                    QWidget *widget     = new QWidget;
                    QPushButton *button = new QPushButton;
                    button->setText("Add");
                    button->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);
                    widget->setSizePolicy(QSizePolicy::Minimum,
                                          QSizePolicy::Minimum);

                    connect(button, &QPushButton::clicked, this, [ = ]() {
                        addItemToShop(i, table);
                    });

                    QHBoxLayout *bLayout = new QHBoxLayout(widget);
                    bLayout->addWidget(button);
                    bLayout->setAlignment(Qt::AlignCenter);
                    bLayout->setContentsMargins(0, 0, 0, 0);
                    widget->setLayout(bLayout);
                    table->setCellWidget(i, 0, widget);
                }
            }
        }
    }


    settings.endArray();
}

// Add Item to Shop
void ShopEditor::addItemToShop(int index, QTableWidget *table)
{
    if (!currentProject.isNull())
    {
        int newIndex = ui->tableWidget_shopItems->rowCount();

        ui->tableWidget_shopItems->insertRow(newIndex);

        for (int j = 0; j < 4; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(table->item(index, j + 1)->text());
            ui->tableWidget_shopItems->setItem(newIndex, j + 1, item);
        }

        // Add "Remove" button
        QWidget *widget     = new QWidget;
        QPushButton *button = new QPushButton;
        button->setText("Remove");
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        QHBoxLayout *bLayout = new QHBoxLayout(widget);
        bLayout->addWidget(button);
        bLayout->setAlignment(Qt::AlignCenter);
        bLayout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(bLayout);
        ui->tableWidget_shopItems->setCellWidget(newIndex, 0, widget);

        connect(button, &QPushButton::clicked, this, [ = ]() {
            removeItemFromShop(newIndex);
        });
    }
}

// Remove item from shop
void ShopEditor::removeItemFromShop(int index)
{
    qDebug() << "Removing row" << index;
    ui->tableWidget_shopItems->removeRow(index);
    on_pushButton_saveShop_clicked();
    loadShop();
}

void ShopEditor::on_pushButton_loadProject_clicked()
{
    if (!ui->comboBox_loadProject->currentText().isNull())
    {
        loadProject(ui->comboBox_loadProject->currentText());
    }
}

// When the current item in the shop tree view changes
void ShopEditor::on_treeWidget_shops_currentItemChanged(QTreeWidgetItem *current)
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

// Save shop
void ShopEditor::on_pushButton_saveShop_clicked()
{
    if (!currentProject.isNull())
    {
        QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
        QSettings settings(projectFolder + "/" + currentProject + ".shop",
                           QSettings::IniFormat);

        int shops = settings.beginReadArray(currentCategory + "_shops");
        settings.endArray();

        settings.beginWriteArray(currentCategory + "_shops");

        // Iterate through all shops in the category and find the correct one
        for (int i = 0; i < shops; i++)
        {
            settings.setArrayIndex(i);

            if (settings.value("name", "").toString() == currentShop)
            {
                // Shop Info
                settings.setValue("owner", ui->lineEdit_shopOwner->text());
                settings.setValue("description",
                                  ui->lineEdit_shopDescription->text());

                // Items
                int items = ui->tableWidget_shopItems->rowCount();

                settings.beginWriteArray("items");

                for (int j = 0; j < items; j++)
                {
                    settings.setArrayIndex(j);
                    settings.setValue("name",        ui->tableWidget_shopItems->item(
                                          j,
                                          1)->text());
                    settings.setValue("price",       ui->tableWidget_shopItems->item(
                                          j,
                                          2)->text());
                    settings.setValue("category",    ui->tableWidget_shopItems->item(
                                          j,
                                          3)->text());
                    settings.setValue("description", ui->tableWidget_shopItems->item(
                                          j,
                                          4)->text());
                }
                settings.endArray();
            }
        }
        settings.endArray();
    }
}

// Open item editor
void ShopEditor::on_pushButton_openItemEditor_clicked()
{
    ItemEditor *itemEditor = new ItemEditor;

    itemEditor->show();
}

// Refresh item list
void ShopEditor::on_pushButton_refreshItems_clicked()
{
    loadItems();
}

// Remove all Items from shop
void ShopEditor::on_pushButton_clearItems_clicked()
{
    ui->tableWidget_shopItems->clearContents();
    ui->tableWidget_shopItems->setRowCount(0);
    on_pushButton_saveShop_clicked();
}

// Delete Selected Shop or Category
void ShopEditor::on_pushButton_deleteSelected_clicked()
{
    if (ui->treeWidget_shops->currentColumn() > -1)
    {
        QTreeWidgetItem *item   = ui->treeWidget_shops->currentItem();
        QString   projectFolder = settingsManager->getSetting(Setting::shopPath);
        QSettings settings(projectFolder + "/" + currentProject + ".shop",
                           QSettings::IniFormat);

        if (item->type() == 0) // Category
        {
            QStringList categories;
            int index = settings.beginReadArray("categories");

            for (int i = 0; i < index; i++)
            {
                settings.setArrayIndex(i);
                categories.append(settings.value("name", "").toString());
            }

            settings.endArray();
            categories.removeOne(item->text(0));

            settings.beginWriteArray("categories");

            for (int i = 0; i < categories.size(); i++)
            {
                settings.setArrayIndex(i);
                settings.setValue("name", categories.at(i));
            }

            settings.endArray();
            getCategories();
        }
        else if (item->type() == 1) // Shop
        {
            int index    = settings.beginReadArray(currentCategory + "_shops");
            int remIndex = -1;

            for (int i = 0; i < index; i++)
            {
                settings.setArrayIndex(i);

                if (settings.value("name", "").toString() ==
                    item->text(0))
                {
                    remIndex = i;
                }
            }

            settings.endArray();

            settings.beginWriteArray(currentCategory + "_shops");

            for (int i = 0; i < index; i++)
            {
                settings.setArrayIndex(i);

                if (i == remIndex)
                {
                    settings.remove("");
                }
            }
            settings.endArray();
        }

        loadShops();
    }
}

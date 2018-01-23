#include "itemeditor.h"
#include "ui_itemeditor.h"

#include <QDebug>
#include <QStringList>

ItemEditor::ItemEditor(QWidget *parent) : QWidget(parent), ui(new Ui::ItemEditor)
{
    qDebug() << "Loading Item Editor ...";
    ui->setupUi(this);

    settingsManager = new SettingsManager;

    getCategories();
    loadItems();
}

ItemEditor::~ItemEditor()
{
    delete ui;
}

void ItemEditor::loadItems()
{
    QString projectFolder = settingsManager->getSetting(Setting::shopPath);
    QSettings settings(projectFolder + "/CustomItems.items", QSettings::IniFormat);

    ui->tableWidget_itemList->clearContents();
    ui->tableWidget_itemList->setRowCount(0);

    int count = settings.beginReadArray("items");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);

        QString name = settings.value("name", "").toString();
        QString price = settings.value("price", "/").toString();
        QString category = settings.value("category", "").toString();
        QString description = settings.value("description", "").toString();

        addItemToList(i, {name, price, category, description});
    }
}

void ItemEditor::addItemToList(int index, QStringList info)
{
    ui->tableWidget_itemList->insertRow(index);

    for (int j = 0; j < 4; j++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(info.at(j));

        ui->tableWidget_itemList->setItem(index, j+1, item);
    }

    // Add remove button
    QWidget* widget = new QWidget;
    QPushButton* button = new QPushButton;
    button->setText("Remove");
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout* bLayout = new QHBoxLayout(widget);
    bLayout->addWidget(button);
    bLayout->setAlignment(Qt::AlignCenter);
    bLayout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(bLayout);
    ui->tableWidget_itemList->setCellWidget(index, 0, widget);

    connect(button, &QPushButton::clicked, this, [=]() { removeItem(index); });
}

// Add new item
void ItemEditor::on_pushButton_addItem_clicked()
{
    if (!ui->lineEdit_itemName->text().isNull())
    {
        QString name = ui->lineEdit_itemName->text();
        QString price = ui->lineEdit_itemPrice->text();
        QString category = ui->comboBox_category->currentText();
        QString description = ui->lineEdit_itemDescription->text();

        int index = ui->tableWidget_itemList->rowCount();
        addItemToList(index, {name, price, category, description});
    }
}

// Add new category
void ItemEditor::on_pushButton_addCategory_clicked()
{
    if (!ui->lineEdit_categoryName->text().isNull())
    {
        QString category = ui->lineEdit_categoryName->text();
        ui->lineEdit_categoryName->clear();

        QString projectFolder = settingsManager->getSetting(Setting::shopPath);
        QSettings settings(projectFolder + "/CustomItems.items", QSettings::IniFormat);

        int count = settings.beginReadArray("categories");
        settings.endArray();

        settings.beginWriteArray("categories");
        settings.setArrayIndex(count);
        settings.setValue("name", category);
        settings.endArray();

        getCategories();

        ui->comboBox_category->setCurrentText(category);
    }
}

// Refresh category list
void ItemEditor::getCategories()
{
    ui->comboBox_category->clear();
    QStringList categories;

    QString projectFolder = settingsManager->getSetting(Setting::shopPath);
    QSettings settings(projectFolder + "/CustomItems.items", QSettings::IniFormat);

    int count = settings.beginReadArray("categories");

    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex(i);
        categories.push_back(settings.value("name").toString());
    }
    settings.endArray();

    ui->comboBox_category->addItems(categories);
}

// Remove item
void ItemEditor::removeItem(int index)
{
    ui->tableWidget_itemList->removeRow(index);
    on_pushButton_save_clicked();
    loadItems();
}

// Save all items in table to file
void ItemEditor::on_pushButton_save_clicked()
{
    QString projectFolder = settingsManager->getSetting(Setting::shopPath);
    QSettings settings(projectFolder + "/CustomItems.items", QSettings::IniFormat);

    int count = ui->tableWidget_itemList->rowCount();

    settings.beginWriteArray("items");

    for (int i = 0; i < count; i++)
    {
        QStringList info = {"", "/", "", ""};

        for (int j = 0; j < 4; j++)
        {
            if (ui->tableWidget_itemList->item(i, j+1) != NULL)
                info.replace(j, ui->tableWidget_itemList->item(i, j+1)->text());
        }

        settings.setArrayIndex(i);

        settings.setValue("name", info.at(0));
        settings.setValue("price", info.at(1));
        settings.setValue("category", info.at(2));
        settings.setValue("description", info.at(3));
    }

    settings.endArray();
}

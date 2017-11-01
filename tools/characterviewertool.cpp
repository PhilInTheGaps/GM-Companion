#include "characterviewertool.h"
#include "ui_characterviewertool.h"
#include "functions.h"

#include <QDebug>
#include <QTableWidgetItem>

CharacterViewerTool::CharacterViewerTool(QWidget *parent) : QWidget(parent), ui(new Ui::CharacterViewerTool)
{
    qDebug() << "Loading CharacterViewerTool ...";

    ui->setupUi(this);

    settingsManager = new SettingsManager;

    getCharacters();

    // Get all available templates
    for (QString sheetTemplate : getFiles(settingsManager->getSetting(charactersPath) + "/templates"))
    {
        if (sheetTemplate.contains(".ini"))
            ui->comboBox_template->addItem(sheetTemplate.replace(".ini", ""));
    }

    // Get addon templates
    QString addonsPath = QDir::homePath() + "/.gm-companion/addons";
    for (QString addon : getFolders(addonsPath))
    {
        if (settingsManager->getIsAddonEnabled(addon) && QFile(addonsPath+"/"+addon+"/CharacterSheet.ini").exists())
        {
            ui->comboBox_template->addItem(addon);
        }
    }
}

CharacterViewerTool::~CharacterViewerTool()
{
    delete ui;
}

void CharacterViewerTool::getCharacters()
{
    // Clear lists
    ui->listWidget_characters->clear();
    ui->listWidget_activeChars->clear();
    ui->listWidget_inactiveChars->clear();

    QString path = settingsManager->getSetting(charactersPath);

    QStringList inactiveCharacters = settingsManager->getInactiveCharacters();

    for (QString file : getFiles(path))
    {
        if (file.contains(".ini"))
        {
            QSettings settings(path+"/"+file, QSettings::IniFormat);

            // Will look like this: "Brynden Rivers (Peter)"
            QString listText = settings.value("name").toString() + " (" + settings.value("player").toString() + ")";

            // For display in the normal list widget
            QListWidgetItem *item1 = new QListWidgetItem(listText);
            item1->setToolTip(file.replace(".ini", ""));

            // For active or inactive list
            QListWidgetItem *item2 = new QListWidgetItem(listText);
            item2->setWhatsThis(file.replace(".ini", ""));

            // Add items to lists
            if (!inactiveCharacters.contains(file.replace(".ini", "")))
            {
                ui->listWidget_characters->addItem(item1);
                ui->listWidget_activeChars->addItem(item2);
            }
            else
            {
                ui->listWidget_inactiveChars->addItem(item2);
            }
        }
    }
}

void CharacterViewerTool::on_listWidget_characters_currentItemChanged(QListWidgetItem *current)
{
    if (ui->listWidget_characters->currentRow() > -1)
    {
        QString file = current->toolTip() + ".ini";
        QString charPath = settingsManager->getSetting(charactersPath);
        QString filePath = charPath + "/" + file;

        QSettings settings(filePath, QSettings::IniFormat);

        // Bio & Info
        QString info = settings.value("info").toString();
        ui->textEdit_bioInfo->setHtml(info);

        QString info2 = settings.value("info2").toString();
        ui->textEdit_bioInfo2->setHtml(info2);

        // Inventory
        int inventoryCount = settings.beginReadArray("inventory");
        ui->tableWidget_inventory->setRowCount(inventoryCount);

        for (int i = 0; i < inventoryCount; i++)
        {
            settings.setArrayIndex(i);

            QString item = settings.value("item").toString();
            QString where = settings.value("where").toString();
            QString weight = settings.value("weight").toString();

            QTableWidgetItem *i1 = new QTableWidgetItem;
            i1->setText(item);
            i1->setToolTip(item);

            QTableWidgetItem *i2 = new QTableWidgetItem;
            i2->setText(where);
            i2->setToolTip(where);

            QTableWidgetItem *i3 = new QTableWidgetItem;
            i3->setText(weight);
            i3->setToolTip(weight);

            ui->tableWidget_inventory->setItem(i, 0, i1);
            ui->tableWidget_inventory->setItem(i, 1, i2);
            ui->tableWidget_inventory->setItem(i, 2, i3);
        }
        settings.endArray();

        // Attributes
        int attributeCount = settings.beginReadArray("attributes");
        ui->tableWidget_attributes->setColumnCount(attributeCount);
        ui->tableWidget_attributes->setRowCount(2);

        for (int i = 0; i < attributeCount; i++)
        {
            settings.setArrayIndex(i);

            QString name = settings.value("name").toString();
            QString value = settings.value("value").toString();

            QTableWidgetItem *i1 = new QTableWidgetItem(name);
            i1->setToolTip(name);
            ui->tableWidget_attributes->setItem(0, i, i1);

            QTableWidgetItem *i2 = new QTableWidgetItem(value);
            i2->setToolTip(value);
            ui->tableWidget_attributes->setItem(1, i, i2);
        }
        settings.endArray();

        // Abilities
        int abilityCount = settings.beginReadArray("abilities");
        ui->tableWidget_abilities->setRowCount(abilityCount);

        for (int i = 0; i < abilityCount; i++)
        {
            settings.setArrayIndex(i);

            QString name = settings.value("name").toString();
            QString rank = settings.value("rank").toString();
            QString notes = settings.value("notes").toString();

            QTableWidgetItem *i1 = new QTableWidgetItem;
            i1->setText(name);
            i1->setToolTip(name);

            QTableWidgetItem *i2 = new QTableWidgetItem;
            i2->setText(rank);
            i2->setToolTip(rank);

            QTableWidgetItem *i3 = new QTableWidgetItem;
            i3->setText(notes);
            i3->setToolTip(notes);

            ui->tableWidget_abilities->setItem(i, 0, i1);
            ui->tableWidget_abilities->setItem(i, 1, i2);
            ui->tableWidget_abilities->setItem(i, 2, i3);
        }
        settings.endArray();

        // Notes
        ui->textEdit_notes->setHtml(settings.value("notes").toString());
    }
    else
    {
        ui->textEdit_bioInfo->clear();

        ui->tableWidget_inventory->setRowCount(0);

        ui->tableWidget_abilities->setRowCount(0);

        ui->tableWidget_attributes->setRowCount(0);
        ui->tableWidget_attributes->setColumnCount(0);

        ui->textEdit_notes->clear();
    }
}

// Add a new character
void CharacterViewerTool::on_pushButton_newCharacter_clicked()
{
    QString characterName = ui->lineEdit_characterName->text();
    QString playerName = ui->lineEdit_playerName->text();

    if (!characterName.isEmpty() && !playerName.isEmpty())
    {
        QSettings settings(settingsManager->getSetting(charactersPath)+"/"+characterName+".ini", QSettings::IniFormat);

        settings.setValue("name", characterName);
        settings.setValue("player", playerName);

        if (ui->comboBox_template->currentIndex() != 0)
        {
            QString templateName = ui->comboBox_template->currentText();
            QString templatePath;

            // Check if template belongs to an addon
            if (settingsManager->getIsAddonEnabled(templateName))
                templatePath = QDir::homePath() + "/.gm-companion/addons/" + templateName + "/CharacterSheet.ini";
            else
                templatePath = settingsManager->getSetting(Setting::charactersPath)+"/templates/" + templateName + ".ini";

            QSettings temp(templatePath, QSettings::IniFormat);

            // Copy values
            settings.setValue("info", temp.value("info").toString());
            settings.setValue("info2", temp.value("info2").toString());
            settings.setValue("notes", temp.value("notes").toString());

            int inventorySize = temp.beginReadArray("inventory");
            settings.beginWriteArray("inventory");
            for (int i = 0; i < inventorySize; i++)
            {
                temp.setArrayIndex(i);
                settings.setArrayIndex(i);

                QString item = temp.value("item").toString();
                QString where = temp.value("where").toString();
                QString weight = temp.value("weight").toString();

                settings.setValue("item", item);
                settings.setValue("where", where);
                settings.setValue("weight", weight);
            }
            settings.endArray();
            temp.endArray();

            int attributeCount = temp.beginReadArray("attributes");
            settings.beginWriteArray("attributes");
            for (int i = 0; i < attributeCount; i++)
            {
                temp.setArrayIndex(i);
                settings.setArrayIndex(i);

                QString name = temp.value("name").toString();
                QString value = temp.value("value").toString();

                settings.setValue("name", name);
                settings.setValue("value", value);
            }
            settings.endArray();
            temp.endArray();

            int abilityCount = temp.beginReadArray("abilities");
            settings.beginWriteArray("abilities");
            for (int i = 0; i < abilityCount; i++)
            {
                temp.setArrayIndex(i);
                settings.setArrayIndex(i);

                QString name = temp.value("name").toString();
                QString rank = temp.value("rank").toString();
                QString notes = temp.value("notes").toString();

                settings.setValue("name", name);
                settings.setValue("rank", rank);
                settings.setValue("notes", notes);
            }
            settings.endArray();
            temp.endArray();
        }
    }
    getCharacters();
}

// Save character info
void CharacterViewerTool::on_pushButton_save_clicked()
{
    if (ui->listWidget_characters->currentRow() > -1)
    {
        QString filename = ui->listWidget_characters->currentItem()->toolTip() + ".ini";

        QSettings settings(settingsManager->getSetting(charactersPath)+"/"+filename, QSettings::IniFormat);

        // Bio & Info
        QString info = ui->textEdit_bioInfo->document()->toHtml();
        settings.setValue("info", info);

        QString info2 = ui->textEdit_bioInfo2->document()->toHtml();
        settings.setValue("info2", info2);

        // Inventory
        int inventoryCount = ui->tableWidget_inventory->rowCount();
        settings.beginWriteArray("inventory");
        for (int i = 0; i < inventoryCount; i++)
        {
            settings.setArrayIndex(i);

            QString item = ui->tableWidget_inventory->item(i, 0)->text();
            QString where = ui->tableWidget_inventory->item(i, 1)->text();
            QString weight = ui->tableWidget_inventory->item(i, 2)->text();

            settings.setValue("item", item);
            settings.setValue("where", where);
            settings.setValue("weight", weight);
        }
        settings.endArray();

        // Attributes
        int attributeCount = ui->tableWidget_attributes->columnCount();
        settings.beginWriteArray("attributes");
        for (int i = 0; i < attributeCount; i++)
        {
            settings.setArrayIndex(i);

            QString name = ui->tableWidget_attributes->item(0, i)->text();
            QString value = ui->tableWidget_attributes->item(1, i)->text();

            settings.setValue("name", name);
            settings.setValue("value", value);
        }
        settings.endArray();

        // Abilities
        int abilityCount = ui->tableWidget_abilities->rowCount();
        settings.beginWriteArray("abilities");
        for (int i = 0; i < abilityCount; i++)
        {
            settings.setArrayIndex(i);

            QString name = ui->tableWidget_abilities->item(i, 0)->text();
            QString rank = ui->tableWidget_abilities->item(i, 1)->text();
            QString notes = ui->tableWidget_abilities->item(i, 2)->text();

            settings.setValue("name", name);
            settings.setValue("rank", rank);
            settings.setValue("notes", notes);
        }
        settings.endArray();

        // Notes
        settings.setValue("notes", ui->textEdit_notes->document()->toHtml());
    }
}

// Add Inventory Row
void CharacterViewerTool::on_pushButton_addItemRow_clicked()
{
    int rows = ui->tableWidget_inventory->rowCount();
    ui->tableWidget_inventory->setRowCount(rows+1);

    QTableWidgetItem *i1 = new QTableWidgetItem;
    QTableWidgetItem *i2 = new QTableWidgetItem;
    QTableWidgetItem *i3 = new QTableWidgetItem;

    ui->tableWidget_inventory->setItem(rows, 0, i1);
    ui->tableWidget_inventory->setItem(rows, 1, i2);
    ui->tableWidget_inventory->setItem(rows, 2, i3);
}

// Remove Inventory Row
void CharacterViewerTool::on_pushButton_removeItemRow_clicked()
{
    int rows = ui->tableWidget_inventory->rowCount();
    ui->tableWidget_inventory->setRowCount(rows-1);
}

// Add Attribute Column
void CharacterViewerTool::on_pushButton_addAttribute_clicked()
{
    int columns = ui->tableWidget_attributes->columnCount();
    ui->tableWidget_attributes->setColumnCount(columns+1);

    QTableWidgetItem *i1 = new QTableWidgetItem;
    QTableWidgetItem *i2 = new QTableWidgetItem;

    ui->tableWidget_attributes->setItem(0, columns, i1);
    ui->tableWidget_attributes->setItem(1, columns, i2);
}

// Remove Attribute Column
void CharacterViewerTool::on_pushButton_removeAttribute_clicked()
{
    int columns = ui->tableWidget_attributes->columnCount();
    ui->tableWidget_attributes->setColumnCount(columns-1);
}

// Add ability row
void CharacterViewerTool::on_pushButton_addAbility_clicked()
{
    int rows = ui->tableWidget_abilities->rowCount();
    ui->tableWidget_abilities->setRowCount(rows+1);

    QTableWidgetItem *i1 = new QTableWidgetItem;
    QTableWidgetItem *i2 = new QTableWidgetItem;
    QTableWidgetItem *i3 = new QTableWidgetItem;

    ui->tableWidget_abilities->setItem(rows, 0, i1);
    ui->tableWidget_abilities->setItem(rows, 1, i2);
    ui->tableWidget_abilities->setItem(rows, 2, i3);
}

// Remove ability row
void CharacterViewerTool::on_pushButton_removeAbility_clicked()
{
    int rows = ui->tableWidget_abilities->rowCount();
    ui->tableWidget_abilities->setRowCount(rows-1);
}

// Make character inactive
void CharacterViewerTool::on_listWidget_activeChars_itemDoubleClicked(QListWidgetItem *item)
{
    QListWidgetItem *newItem = item->clone();
    ui->listWidget_inactiveChars->addItem(newItem);

    delete item;

    QStringList inactiveCharacters;

    for (int i = 0; i < ui->listWidget_inactiveChars->count(); i++)
    {
        QString character = ui->listWidget_inactiveChars->item(i)->whatsThis();
        inactiveCharacters.push_back(character);
    }

    settingsManager->setInactiveCharacters(inactiveCharacters);

    getCharacters();
}

// Make character active
void CharacterViewerTool::on_listWidget_inactiveChars_itemDoubleClicked(QListWidgetItem *item)
{
    QListWidgetItem *newItem = item->clone();
    ui->listWidget_activeChars->addItem(newItem);

    delete item;

    QStringList inactiveCharacters;

    for (int i = 0; i < ui->listWidget_inactiveChars->count(); i++)
    {
        QString character = ui->listWidget_inactiveChars->item(i)->whatsThis();
        inactiveCharacters.push_back(character);
    }

    settingsManager->setInactiveCharacters(inactiveCharacters);

    getCharacters();
}

// Make all characters active
void CharacterViewerTool::on_pushButton_makeAllActive_clicked()
{
    qDebug() << "Making all characters active ...";

    ui->listWidget_inactiveChars->clear();

    QStringList characters;
    settingsManager->setInactiveCharacters(characters);

    getCharacters();
}

// Make all characters inactive
void CharacterViewerTool::on_pushButton_makeAllInactive_clicked()
{
    qDebug() << "Making all characters inactive ...";

    QStringList characters;

    for (int i = 0; i < ui->listWidget_activeChars->count(); i++)
    {
        QString character = ui->listWidget_activeChars->item(i)->whatsThis();

        characters.push_back(character);
    }

    ui->listWidget_activeChars->clear();

    QStringList inactiveChars = settingsManager->getInactiveCharacters();
    inactiveChars.append(characters);

    settingsManager->setInactiveCharacters(inactiveChars);

    getCharacters();
}

#include "charactertool.h"
#include "ui_charactertool.h"

#include <QComboBox>
#include <QDebug>
#include <QSettings>
#include <QListWidget>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QGraphicsView>

#include "gm-companion/functions.h"

CharacterTool::CharacterTool(QWidget *parent) : QWidget(parent), ui(new Ui::CharacterTool)
{
    ui->setupUi(this);

    settingsManager    = new SettingsManager;
    inactiveCharacters = settingsManager->getInactiveCharacters();

    setTemplates();

    loadCharacterSheets();

    updateCharacterList();
}

CharacterTool::~CharacterTool()
{
    delete ui;
}

// Add all enabled templates to the template combo box
void CharacterTool::setTemplates()
{
    QStringList templates = { "DSA5", "Entaria_v2" };

    for (QString temp : templates)
    {
        ui->comboBox_template->addItem(temp);
    }
}

// Add all character sheets to the stacked widget
void CharacterTool::loadCharacterSheets()
{
    defaultSheet = new DefaultSheet;
    ui->stackedWidget->addWidget(defaultSheet);

    dsa5Sheet = new DSA5Sheet;
    ui->stackedWidget->addWidget(dsa5Sheet);

    entaria2Sheet = new Entaria2Sheet;
    ui->stackedWidget->addWidget(entaria2Sheet);
}

// Get all characters and add them to their correct list
void CharacterTool::updateCharacterList()
{
    QString charPath = settingsManager->getSetting(
        Setting::charactersPath);

    ui->listWidget_activeCharacters->clear();
    ui->listWidget_inactiveCharacters->clear();

    for (QString character : getFiles(charPath))
    {
        if (character.endsWith(".character"))
        {
            QString fileName = character.replace(".character", "");

            QListWidgetItem *item = new QListWidgetItem;
            item->setText(cleanText(fileName));
            item->setWhatsThis(character);

            // Decide if a character is active or inactive
            if (inactiveCharacters.contains(fileName))
            {
                ui->listWidget_inactiveCharacters->addItem(item);
            }
            else
            {
                ui->listWidget_activeCharacters->addItem(item);

                if (fileName == currentCharacter)
                {
                    ui->listWidget_activeCharacters->setCurrentItem(item);
                }
            }
        }
    }
}

// Create a new Character
void CharacterTool::createNewCharacter()
{
    QString characterName = ui->lineEdit_characterName->text();
    QString playerName    = ui->lineEdit_playerName->text();
    QString sheetTemplate = ui->comboBox_template->currentText();

    if (!characterName.isNull() && !characterName.isEmpty())
    {
        QString charPath  = settingsManager->getSetting(Setting::charactersPath);
        QString character = characterName;
        character        = character.replace(" ", "_");
        currentCharacter = character;
        QString filePath = charPath + "/" + character + ".character";

        int counter = 2;

        while (QFile(filePath).exists())
        {
            filePath = charPath + "/" + character + "_" + QString::number(counter) + ".character";
            counter++;
        }

        QSettings settings(filePath, QSettings::IniFormat);
        settings.setValue("character_name", characterName);
        settings.setValue("player_name",    playerName);
        settings.setValue("sheet_template", sheetTemplate);

        ui->lineEdit_characterName->clear();
        ui->lineEdit_playerName->clear();

        delay(1);

        updateCharacterList();
    }
}

// Make a character active
void CharacterTool::makeCharacterActive(int index)
{
    QListWidgetItem *item = ui->listWidget_inactiveCharacters->item(index);

    QString fileName = item->whatsThis().replace(".character", "");

    inactiveCharacters.removeOne(fileName);
}

// Make a character inactive
void CharacterTool::makeCharacterInactive(int index)
{
    QListWidgetItem *item = ui->listWidget_activeCharacters->item(index);

    QString fileName = item->whatsThis().replace(".character", "");

    inactiveCharacters.append(fileName);
}

// Make selected character inactive
void CharacterTool::on_pushButton_makeInactive_clicked()
{
    int index = ui->listWidget_activeCharacters->currentRow();

    if (index > -1)
    {
        makeCharacterInactive(index);
        settingsManager->setInactiveCharacters(inactiveCharacters);
        updateCharacterList();
    }
}

// Make all characters inactive
void CharacterTool::on_pushButton_makeAllInactive_clicked()
{
    for (int i = 0; i < ui->listWidget_activeCharacters->count(); i++)
    {
        makeCharacterInactive(i);
    }
    settingsManager->setInactiveCharacters(inactiveCharacters);
    updateCharacterList();
}

// Make selected character active
void CharacterTool::on_pushButton_makeActive_clicked()
{
    int index = ui->listWidget_inactiveCharacters->currentRow();

    if (index > -1)
    {
        makeCharacterActive(index);
        settingsManager->setInactiveCharacters(inactiveCharacters);
        updateCharacterList();
    }
}

// Make all characters active
void CharacterTool::on_pushButton_makeAllActive_clicked()
{
    for (int i = 0; i < ui->listWidget_inactiveCharacters->count(); i++)
    {
        makeCharacterActive(i);
    }
    settingsManager->setInactiveCharacters(inactiveCharacters);
    updateCharacterList();
}

// Create new character
void CharacterTool::on_pushButton_createNewCharacter_clicked()
{
    createNewCharacter();
}

// Load character images
void CharacterTool::loadCharacterImages(QString path, QStringList files)
{
    ui->listWidget_pages->clear();

    for (QString file : files)
    {
        QString filePath = path + "/" + file;

        QListWidgetItem *item = new QListWidgetItem;
        item->setWhatsThis(filePath);
        item->setText(file);
        ui->listWidget_pages->addItem(item);
    }

    ui->stackedWidget_image_text->setCurrentIndex(1);
}

// Change currently displayed character
void CharacterTool::on_listWidget_activeCharacters_currentItemChanged(QListWidgetItem *item)
{
    if (item != NULL)
    {
        QString charPath      = settingsManager->getSetting(Setting::charactersPath);
        QString characterFile = item->whatsThis();
        QString character     = characterFile.replace(".character", "");

        currentCharacter = character;

        QDir dir(charPath + "/" + character);
        QStringList files;
        bool useImage = ui->checkBox_useImages->isChecked();

        if (useImage && dir.exists())
        {
            files = dir.entryList({ "*.jpg", "*.jpeg", "*.png" });
        }
        else
        {
            ui->stackedWidget_image_text->setCurrentIndex(0);
        }

        if (files.size() > 0)
        {
            loadCharacterImages(charPath + "/" + character, files);
        }
        else
        {
            QSettings settings(charPath + "/" + characterFile + ".character", QSettings::IniFormat);
            QString   sheetTemplate = settings.value("sheet_template", "NONE").toString();
            QString   filePath      = charPath + "/" + characterFile + ".character";

            int index = 0;

            if (sheetTemplate == "Default Sheet")
            {
                index = 1;
                defaultSheet->load(filePath);
            }
            else if (sheetTemplate == "DSA5")
            {
                index = 2;
                dsa5Sheet->load(filePath);
            }
            else if (sheetTemplate == "Entaria_v2")
            {
                index = 3;
                entaria2Sheet->load(filePath);
            }

            ui->stackedWidget->setCurrentIndex(index);
        }
    }
}

// Save character
void CharacterTool::on_pushButton_save_clicked()
{
    if (ui->listWidget_activeCharacters->currentItem() != NULL)
    {
        QString charPath      = settingsManager->getSetting(Setting::charactersPath);
        QString characterFile = ui->listWidget_activeCharacters->currentItem()->whatsThis();
        QString filePath      = charPath + "/" + characterFile + ".character";

        switch (ui->stackedWidget->currentIndex()) {
        case 1: // Default Sheet
            defaultSheet->save(filePath);
            break;

        case 2: // DSA5 Sheet
            dsa5Sheet->save(filePath);
            break;

        case 3: // Entaria v2 Sheet
            entaria2Sheet->save(filePath);
            break;

        default: // No character selected
            break;
        }
    }
}

// Delete Character
void CharacterTool::on_pushButton_delete_clicked()
{
    if (ui->listWidget_activeCharacters->currentItem() != NULL)
    {
        QString charPath      = settingsManager->getSetting(Setting::charactersPath);
        QString characterFile = ui->listWidget_activeCharacters->currentItem()->whatsThis();
        QString filePath      = charPath + "/" + characterFile + ".character";

        qDebug().noquote() << "Deleting character:" << filePath;

        QFile f(filePath);
        f.remove();

        updateCharacterList();
    }
}

void CharacterTool::on_listWidget_pages_currentItemChanged(QListWidgetItem *item)
{
    if (item != NULL)
    {
        QString filePath = item->whatsThis();


        QPixmap pixmap(filePath);
        QGraphicsScene *scene = new QGraphicsScene;
        ui->graphicsView->setScene(scene);

        scene->addPixmap(pixmap);

        ui->graphicsView->fitInView(0, 0, pixmap.width(), pixmap.height(), Qt::KeepAspectRatio);
    }
}

// QGraphicsView zoom in
void CharacterTool::on_pushButton_zoomIn_clicked()
{
    ui->graphicsView->scale(2, 2);
}

void CharacterTool::on_pushButton_zoomOut_clicked()
{
    ui->graphicsView->scale(0.5, 0.5);
}

void CharacterTool::on_pushButton_reset_clicked()
{
    ui->graphicsView->resetTransform();
}

#include "characterviewertool.h"
#include "ui_characterviewertool.h"
#include "functions.h"

#include <QDebug>

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
    ui->listWidget_unactiveChars->clear();

    QString path = settingsManager->getSetting(charactersPath);

    QStringList unactiveCharacters = settingsManager->getUnactiveCharacters();

    for (QString file : getFiles(path))
    {
        if (file.contains(".ini"))
        {
            QSettings settings(path+"/"+file, QSettings::IniFormat);

            // Will look like this: "Brynden Rivers (Peter)"
            QString listText = settings.value("name").toString() + " (" + settings.value("player").toString() + ")";

            QString content = settings.value("content").toString();

            // For display in the normal list widget
            QListWidgetItem *item1 = new QListWidgetItem(listText);
            item1->setWhatsThis(content);
            item1->setToolTip(file.replace(".ini", ""));

            // For active or unactive list
            QListWidgetItem *item2 = new QListWidgetItem(listText);
            item2->setWhatsThis(file.replace(".ini", ""));

            // Add items to lists
            if (!unactiveCharacters.contains(file.replace(".ini", "")))
            {
                ui->listWidget_characters->addItem(item1);
                ui->listWidget_activeChars->addItem(item2);
            }
            else
            {
                ui->listWidget_unactiveChars->addItem(item2);
            }
        }
    }
}

void CharacterViewerTool::on_listWidget_characters_currentItemChanged(QListWidgetItem *current)
{
    if (ui->listWidget_characters->currentRow() > -1)
        ui->textEdit->setHtml(current->whatsThis());
    else
        ui->textEdit->clear();
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
            QSettings temp(settingsManager->getSetting(Setting::charactersPath)+"/templates/"+ui->comboBox_template->currentText()+".ini", QSettings::IniFormat);

            settings.setValue("content", temp.value("content").toString());
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

        QString content = ui->textEdit->document()->toHtml();

        settings.setValue("content", content);

        ui->listWidget_characters->currentItem()->setWhatsThis(content);
    }
}

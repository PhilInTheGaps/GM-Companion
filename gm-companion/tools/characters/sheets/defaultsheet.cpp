#include "defaultsheet.h"
#include "ui_defaultsheet.h"
#include "sheetfunctions.h"

#include <QDebug>

DefaultSheet::DefaultSheet(QWidget *parent) : QWidget(parent), ui(new Ui::DefaultSheet)
{
    ui->setupUi(this);
}

DefaultSheet::~DefaultSheet()
{
    delete ui;
}

void DefaultSheet::save(QString filePath)
{
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat);

    // Character Info
    saveLineEdit(ui->lineEdit_characterName, "character_name",       settings);
    saveLineEdit(ui->lineEdit_playerName,    "player_name",          settings);
    saveLineEdit(ui->lineEdit_level,         "character_level",      settings);
    saveLineEdit(ui->lineEdit_race,          "character_race",       settings);
    saveLineEdit(ui->lineEdit_size,          "character_size",       settings);
    saveLineEdit(ui->lineEdit_gender,        "character_gender",     settings);
    saveLineEdit(ui->lineEdit_age,           "character_age",        settings);
    saveLineEdit(ui->lineEdit_home,          "character_home",       settings);
    saveLineEdit(ui->lineEdit_location,      "character_location",   settings);
    saveLineEdit(ui->lineEdit_height,        "character_height",     settings);
    saveLineEdit(ui->lineEdit_weight,        "character_weight",     settings);
    saveLineEdit(ui->lineEdit_hairColor,     "character_hair_color", settings);
    saveLineEdit(ui->lineEdit_eyeColor,      "character_eye_color",  settings);

    // Bio
    saveTextEdit(ui->textEdit_bio, "bio", settings);

    // Attributes
    saveTableWidget_horizontal(ui->tableWidget_attributes, "attributes", { "attribute", "value" }, settings);

    // Abilities
    saveTableWidget_vertical(ui->tableWidget_abilities_left,  "abilities_left",  { "ability", "value" }, settings);
    saveTableWidget_vertical(ui->tableWidget_abilities_right, "abilities_right", { "ability", "value" }, settings);

    // Inventory
    saveTableWidget_vertical(ui->tableWidget_inventory,       "inventory",       { "item" },             settings);
}

void DefaultSheet::load(QString filePath)
{
    ui->tabWidget->setCurrentIndex(0);

    QSettings *settings = new QSettings(filePath, QSettings::IniFormat);

    // Character Info
    loadLineEdit(ui->lineEdit_characterName, "character_name",       settings);
    loadLineEdit(ui->lineEdit_playerName,    "player_name",          settings);
    loadLineEdit(ui->lineEdit_level,         "character_level",      settings);
    loadLineEdit(ui->lineEdit_race,          "character_race",       settings);
    loadLineEdit(ui->lineEdit_size,          "character_size",       settings);
    loadLineEdit(ui->lineEdit_gender,        "character_gender",     settings);
    loadLineEdit(ui->lineEdit_age,           "character_age",        settings);
    loadLineEdit(ui->lineEdit_home,          "character_home",       settings);
    loadLineEdit(ui->lineEdit_location,      "character_location",   settings);
    loadLineEdit(ui->lineEdit_height,        "character_height",     settings);
    loadLineEdit(ui->lineEdit_weight,        "character_weight",     settings);
    loadLineEdit(ui->lineEdit_hairColor,     "character_hair_color", settings);
    loadLineEdit(ui->lineEdit_eyeColor,      "character_eye_color",  settings);

    // Bio
    loadTextEdit(ui->textEdit_bio, "bio", settings);

    // Attributes
    loadTableWidget_horizontal(ui->tableWidget_attributes, "attributes", { "attribute", "value" }, settings);

    // Abilities
    loadTableWidget_vertical(ui->tableWidget_abilities_left,  "abilities_left",  { "ability", "value" }, settings);
    loadTableWidget_vertical(ui->tableWidget_abilities_right, "abilities_right", { "ability", "value" }, settings);

    // Inventory
    loadTableWidget_vertical(ui->tableWidget_inventory,       "inventory",       { "item" },             settings);
}

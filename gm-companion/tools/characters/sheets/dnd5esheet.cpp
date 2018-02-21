#include "dnd5esheet.h"
#include "ui_dnd5esheet.h"
#include "sheetfunctions.h"

#include <QSettings>

DnD5eSheet::DnD5eSheet(QWidget *parent) : QWidget(parent), ui(new Ui::DnD5eSheet)
{
    ui->setupUi(this);
}

DnD5eSheet::~DnD5eSheet()
{
    delete ui;
}

void DnD5eSheet::save(QString filePath)
{
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat);

    // Top Frame
    saveLineEdit(ui->lineEdit_characterName,     "character_name",     settings);
    saveLineEdit(ui->lineEdit_classLevel,        "class_level",        settings);
    saveLineEdit(ui->lineEdit_background,        "background",         settings);
    saveLineEdit(ui->lineEdit_playerName,        "player_name",        settings);
    saveLineEdit(ui->lineEdit_race,              "race",               settings);
    saveLineEdit(ui->lineEdit_alignment,         "alignment",          settings);
    saveLineEdit(ui->lineEdit_experience,        "experience",         settings);

    // Strength, Dexterity etc.
    saveLineEdit(ui->lineEdit_strengthValue,     "strength_value",     settings);
    saveLineEdit(ui->lineEdit_strengthMod,       "strength_mod",       settings);
    saveLineEdit(ui->lineEdit_dexterityValue,    "dexterity_value",    settings);
    saveLineEdit(ui->lineEdit_dexterityMod,      "dexterity_mod",      settings);
    saveLineEdit(ui->lineEdit_constitutionValue, "constitution_value", settings);
    saveLineEdit(ui->lineEdit_constitutionMod,   "constitution_mod",   settings);
    saveLineEdit(ui->lineEdit_intelligenceValue, "intelligence_value", settings);
    saveLineEdit(ui->lineEdit_intelligenceMod,   "intelligence_mod",   settings);
    saveLineEdit(ui->lineEdit_wisdomValue,       "wisdom_value",       settings);
    saveLineEdit(ui->lineEdit_wisdomMod,         "wisdom_mod",         settings);
    saveLineEdit(ui->lineEdit_charismaValue,     "charisma_value",     settings);
    saveLineEdit(ui->lineEdit_charismaMod,       "charisma_mod",       settings);

    // Left column
    saveLineEdit(ui->lineEdit_inspiration,       "inspiration",        settings);
    saveLineEdit(ui->lineEdit_proficiencyBonus,  "proficiency_bonus",  settings);
    saveLineEdit(ui->lineEdit_passiveWisdom,     "passive_wisdom",     settings);

    saveCheckBox(ui->checkBox_strength,     "strength_saving_checked",     settings);
    saveCheckBox(ui->checkBox_dexterity,    "dexterity_saving_checked",    settings);
    saveCheckBox(ui->checkBox_constitution, "constitution_saving_checked", settings);
    saveCheckBox(ui->checkBox_intelligence, "intelligence_saving_checked", settings);
    saveCheckBox(ui->checkBox_wisdom,       "wisdom_saving_checked",       settings);
    saveCheckBox(ui->checkBox_charisma,     "charisma_saving_checked",     settings);

    saveLineEdit(ui->lineEdit_strengthSave,     "strength_saving",     settings);
    saveLineEdit(ui->lineEdit_dexteritySave,    "dexterity_saving",    settings);
    saveLineEdit(ui->lineEdit_constitutionSave, "constitution_saving", settings);
    saveLineEdit(ui->lineEdit_intelligenceSave, "intelligence_saving", settings);
    saveLineEdit(ui->lineEdit_wisdomSave,       "wisdom_saving",       settings);
    saveLineEdit(ui->lineEdit_charismaSave,     "charisma_saving",     settings);

    // Skills
    saveCheckBox(ui->checkBox_acrobatics,     "acrobatics_checked",      settings);
    saveCheckBox(ui->checkBox_animalHandling, "animal_handling_checked", settings);
    saveCheckBox(ui->checkBox_arcana,         "arcana_checked",          settings);
    saveCheckBox(ui->checkBox_athletics,      "athletics_checked",       settings);
    saveCheckBox(ui->checkBox_deception,      "deception_checked",       settings);
    saveCheckBox(ui->checkBox_history,        "history_checked",         settings);
    saveCheckBox(ui->checkBox_insight,        "insight_checked",         settings);
    saveCheckBox(ui->checkBox_intimidation,   "intimidation_checked",    settings);
    saveCheckBox(ui->checkBox_investigation,  "investigation_checked",   settings);
    saveCheckBox(ui->checkBox_medicine,       "medicine_checked",        settings);
    saveCheckBox(ui->checkBox_nature,         "nature_checked",          settings);
    saveCheckBox(ui->checkBox_perception,     "perception_checked",      settings);
    saveCheckBox(ui->checkBox_performance,    "performance_checked",     settings);
    saveCheckBox(ui->checkBox_persuasion,     "persuasion_checked",      settings);
    saveCheckBox(ui->checkBox_religion,       "religion_checked",        settings);
    saveCheckBox(ui->checkBox_sleightOfHand,  "sleight_of_hand_checked", settings);
    saveCheckBox(ui->checkBox_stealth,        "stealth_checked",         settings);
    saveCheckBox(ui->checkBox_survival,       "survival_checked",        settings);

    saveLineEdit(ui->lineEdit_acrobatics,     "acrobatics",      settings);
    saveLineEdit(ui->lineEdit_animalHandling, "animal_handling", settings);
    saveLineEdit(ui->lineEdit_arcana,         "arcana",          settings);
    saveLineEdit(ui->lineEdit_athletics,      "athletics",       settings);
    saveLineEdit(ui->lineEdit_deception,      "deception",       settings);
    saveLineEdit(ui->lineEdit_history,        "history",         settings);
    saveLineEdit(ui->lineEdit_insight,        "insight",         settings);
    saveLineEdit(ui->lineEdit_intimidation,   "intimidation",    settings);
    saveLineEdit(ui->lineEdit_investigation,  "investigation",   settings);
    saveLineEdit(ui->lineEdit_medicine,       "medicine",        settings);
    saveLineEdit(ui->lineEdit_nature,         "nature",          settings);
    saveLineEdit(ui->lineEdit_perception,     "perception",      settings);
    saveLineEdit(ui->lineEdit_performance,    "performance",     settings);
    saveLineEdit(ui->lineEdit_persuasion,     "persuasion",      settings);
    saveLineEdit(ui->lineEdit_religion,       "religion",        settings);
    saveLineEdit(ui->lineEdit_sleightOfHand,  "sleight_of_hand", settings);
    saveLineEdit(ui->lineEdit_stealth,        "stealth",         settings);
    saveLineEdit(ui->lineEdit_survival,       "survival",        settings);

    // Right column
    saveLineEdit(ui->lineEdit_armorClass,     "armor_class",     settings);
    saveLineEdit(ui->lineEdit_initiative,     "initiative",      settings);
    saveLineEdit(ui->lineEdit_speed,          "speed",           settings);
    saveLineEdit(ui->lineEdit_hpMax,          "hp_maximum",      settings);
    saveLineEdit(ui->lineEdit_hpCurrent,      "hp_current",      settings);

    saveLineEdit(ui->lineEdit_atkName1,       "attack_name_1",   settings);
    saveLineEdit(ui->lineEdit_atkName2,       "attack_name_2",   settings);
    saveLineEdit(ui->lineEdit_atkName3,       "attack_name_3",   settings);

    saveLineEdit(ui->lineEdit_atkBonus1,      "attack_bonus_1",  settings);
    saveLineEdit(ui->lineEdit_atkBonus2,      "attack_bonus_2",  settings);
    saveLineEdit(ui->lineEdit_atkBonus3,      "attack_bonus_3",  settings);

    saveLineEdit(ui->lineEdit_atkDmg1,        "attack_damage_1", settings);
    saveLineEdit(ui->lineEdit_atkDmg2,        "attack_damage_2", settings);
    saveLineEdit(ui->lineEdit_atkDmg3,        "attack_damage_3", settings);

    saveTableWidget_vertical(ui->tableWidget_atksSpells, "attacks_spells", { "spell" }, settings);

    // Second Page, left column
    saveLineEdit(ui->lineEdit_cp, "cp", settings);
    saveLineEdit(ui->lineEdit_sp, "sp", settings);
    saveLineEdit(ui->lineEdit_ep, "ep", settings);
    saveLineEdit(ui->lineEdit_gp, "gp", settings);
    saveLineEdit(ui->lineEdit_pp, "pp", settings);

    saveTableWidget_vertical(ui->tableWidget_equipment,      "equipment",               { "item" },        settings);
    saveTableWidget_vertical(ui->tableWidget_languages,      "proficiencies_languages", { "proficiency" }, settings);

    // Features & Traits
    saveTableWidget_vertical(ui->tableWidget_featuresTraits, "features_traits",         { "feature" },     settings);

    // Right column
    saveTableWidget_vertical(ui->tableWidget_personality,    "personality_traits",      { "trait" },       settings);
    saveTableWidget_vertical(ui->tableWidget_ideals,         "ideals",                  { "ideal" },       settings);
    saveTableWidget_vertical(ui->tableWidget_bonds,          "bonds",                   { "bond" },        settings);
    saveTableWidget_vertical(ui->tableWidget_flaws,          "flaws",                   { "flaw" },        settings);
}

void DnD5eSheet::load(QString filePath)
{
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat);

    // Top Frame
    loadLineEdit(ui->lineEdit_characterName,     "character_name",     settings);
    loadLineEdit(ui->lineEdit_classLevel,        "class_level",        settings);
    loadLineEdit(ui->lineEdit_background,        "background",         settings);
    loadLineEdit(ui->lineEdit_playerName,        "player_name",        settings);
    loadLineEdit(ui->lineEdit_race,              "race",               settings);
    loadLineEdit(ui->lineEdit_alignment,         "alignment",          settings);
    loadLineEdit(ui->lineEdit_experience,        "experience",         settings);

    // Strength, Dexterity etc.
    loadLineEdit(ui->lineEdit_strengthValue,     "strength_value",     settings);
    loadLineEdit(ui->lineEdit_strengthMod,       "strength_mod",       settings);
    loadLineEdit(ui->lineEdit_dexterityValue,    "dexterity_value",    settings);
    loadLineEdit(ui->lineEdit_dexterityMod,      "dexterity_mod",      settings);
    loadLineEdit(ui->lineEdit_constitutionValue, "constitution_value", settings);
    loadLineEdit(ui->lineEdit_constitutionMod,   "constitution_mod",   settings);
    loadLineEdit(ui->lineEdit_intelligenceValue, "intelligence_value", settings);
    loadLineEdit(ui->lineEdit_intelligenceMod,   "intelligence_mod",   settings);
    loadLineEdit(ui->lineEdit_wisdomValue,       "wisdom_value",       settings);
    loadLineEdit(ui->lineEdit_wisdomMod,         "wisdom_mod",         settings);
    loadLineEdit(ui->lineEdit_charismaValue,     "charisma_value",     settings);
    loadLineEdit(ui->lineEdit_charismaMod,       "charisma_mod",       settings);

    // Left column
    loadLineEdit(ui->lineEdit_inspiration,       "inspiration",        settings);
    loadLineEdit(ui->lineEdit_proficiencyBonus,  "proficiency_bonus",  settings);
    loadLineEdit(ui->lineEdit_passiveWisdom,     "passive_wisdom",     settings);

    loadCheckBox(ui->checkBox_strength,     "strength_saving_checked",     settings);
    loadCheckBox(ui->checkBox_dexterity,    "dexterity_saving_checked",    settings);
    loadCheckBox(ui->checkBox_constitution, "constitution_saving_checked", settings);
    loadCheckBox(ui->checkBox_intelligence, "intelligence_saving_checked", settings);
    loadCheckBox(ui->checkBox_wisdom,       "wisdom_saving_checked",       settings);
    loadCheckBox(ui->checkBox_charisma,     "charisma_saving_checked",     settings);

    loadLineEdit(ui->lineEdit_strengthSave,     "strength_saving",     settings);
    loadLineEdit(ui->lineEdit_dexteritySave,    "dexterity_saving",    settings);
    loadLineEdit(ui->lineEdit_constitutionSave, "constitution_saving", settings);
    loadLineEdit(ui->lineEdit_intelligenceSave, "intelligence_saving", settings);
    loadLineEdit(ui->lineEdit_wisdomSave,       "wisdom_saving",       settings);
    loadLineEdit(ui->lineEdit_charismaSave,     "charisma_saving",     settings);

    // Skills
    loadCheckBox(ui->checkBox_acrobatics,     "acrobatics_checked",      settings);
    loadCheckBox(ui->checkBox_animalHandling, "animal_handling_checked", settings);
    loadCheckBox(ui->checkBox_arcana,         "arcana_checked",          settings);
    loadCheckBox(ui->checkBox_athletics,      "athletics_checked",       settings);
    loadCheckBox(ui->checkBox_deception,      "deception_checked",       settings);
    loadCheckBox(ui->checkBox_history,        "history_checked",         settings);
    loadCheckBox(ui->checkBox_insight,        "insight_checked",         settings);
    loadCheckBox(ui->checkBox_intimidation,   "intimidation_checked",    settings);
    loadCheckBox(ui->checkBox_investigation,  "investigation_checked",   settings);
    loadCheckBox(ui->checkBox_medicine,       "medicine_checked",        settings);
    loadCheckBox(ui->checkBox_nature,         "nature_checked",          settings);
    loadCheckBox(ui->checkBox_perception,     "perception_checked",      settings);
    loadCheckBox(ui->checkBox_performance,    "performance_checked",     settings);
    loadCheckBox(ui->checkBox_persuasion,     "persuasion_checked",      settings);
    loadCheckBox(ui->checkBox_religion,       "religion_checked",        settings);
    loadCheckBox(ui->checkBox_sleightOfHand,  "sleight_of_hand_checked", settings);
    loadCheckBox(ui->checkBox_stealth,        "stealth_checked",         settings);
    loadCheckBox(ui->checkBox_survival,       "survival_checked",        settings);

    loadLineEdit(ui->lineEdit_acrobatics,     "acrobatics",      settings);
    loadLineEdit(ui->lineEdit_animalHandling, "animal_handling", settings);
    loadLineEdit(ui->lineEdit_arcana,         "arcana",          settings);
    loadLineEdit(ui->lineEdit_athletics,      "athletics",       settings);
    loadLineEdit(ui->lineEdit_deception,      "deception",       settings);
    loadLineEdit(ui->lineEdit_history,        "history",         settings);
    loadLineEdit(ui->lineEdit_insight,        "insight",         settings);
    loadLineEdit(ui->lineEdit_intimidation,   "intimidation",    settings);
    loadLineEdit(ui->lineEdit_investigation,  "investigation",   settings);
    loadLineEdit(ui->lineEdit_medicine,       "medicine",        settings);
    loadLineEdit(ui->lineEdit_nature,         "nature",          settings);
    loadLineEdit(ui->lineEdit_perception,     "perception",      settings);
    loadLineEdit(ui->lineEdit_performance,    "performance",     settings);
    loadLineEdit(ui->lineEdit_persuasion,     "persuasion",      settings);
    loadLineEdit(ui->lineEdit_religion,       "religion",        settings);
    loadLineEdit(ui->lineEdit_sleightOfHand,  "sleight_of_hand", settings);
    loadLineEdit(ui->lineEdit_stealth,        "stealth",         settings);
    loadLineEdit(ui->lineEdit_survival,       "survival",        settings);

    // Right column
    loadLineEdit(ui->lineEdit_armorClass,     "armor_class",     settings);
    loadLineEdit(ui->lineEdit_initiative,     "initiative",      settings);
    loadLineEdit(ui->lineEdit_speed,          "speed",           settings);
    loadLineEdit(ui->lineEdit_hpMax,          "hp_maximum",      settings);
    loadLineEdit(ui->lineEdit_hpCurrent,      "hp_current",      settings);

    loadLineEdit(ui->lineEdit_atkName1,       "attack_name_1",   settings);
    loadLineEdit(ui->lineEdit_atkName2,       "attack_name_2",   settings);
    loadLineEdit(ui->lineEdit_atkName3,       "attack_name_3",   settings);

    loadLineEdit(ui->lineEdit_atkBonus1,      "attack_bonus_1",  settings);
    loadLineEdit(ui->lineEdit_atkBonus2,      "attack_bonus_2",  settings);
    loadLineEdit(ui->lineEdit_atkBonus3,      "attack_bonus_3",  settings);

    loadLineEdit(ui->lineEdit_atkDmg1,        "attack_damage_1", settings);
    loadLineEdit(ui->lineEdit_atkDmg2,        "attack_damage_2", settings);
    loadLineEdit(ui->lineEdit_atkDmg3,        "attack_damage_3", settings);

    loadTableWidget_vertical(ui->tableWidget_atksSpells, "attacks_spells", { "spell" }, settings);

    // Second Page, left column
    loadLineEdit(ui->lineEdit_cp, "cp", settings);
    loadLineEdit(ui->lineEdit_sp, "sp", settings);
    loadLineEdit(ui->lineEdit_ep, "ep", settings);
    loadLineEdit(ui->lineEdit_gp, "gp", settings);
    loadLineEdit(ui->lineEdit_pp, "pp", settings);

    loadTableWidget_vertical(ui->tableWidget_equipment,      "equipment",               { "item" },        settings);
    loadTableWidget_vertical(ui->tableWidget_languages,      "proficiencies_languages", { "proficiency" }, settings);

    // Features & Traits
    loadTableWidget_vertical(ui->tableWidget_featuresTraits, "features_traits",         { "feature" },     settings);

    // Right column
    loadTableWidget_vertical(ui->tableWidget_personality,    "personality_traits",      { "trait" },       settings);
    loadTableWidget_vertical(ui->tableWidget_ideals,         "ideals",                  { "ideal" },       settings);
    loadTableWidget_vertical(ui->tableWidget_bonds,          "bonds",                   { "bond" },        settings);
    loadTableWidget_vertical(ui->tableWidget_flaws,          "flaws",                   { "flaw" },        settings);
}

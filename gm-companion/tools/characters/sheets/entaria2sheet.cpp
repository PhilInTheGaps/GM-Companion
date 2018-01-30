#include "entaria2sheet.h"
#include "ui_entaria2sheet.h"
#include "sheetfunctions.h"

#include <QSettings>

Entaria2Sheet::Entaria2Sheet(QWidget *parent) : QWidget(parent), ui(new Ui::Entaria2Sheet)
{
    ui->setupUi(this);
}

Entaria2Sheet::~Entaria2Sheet()
{
    delete ui;
}

void Entaria2Sheet::save(QString filePath)
{
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat);

    // Eigenschaften
    saveLineEdit(ui->lineEdit_name,         "character_name", settings);
    saveLineEdit(ui->lineEdit_geschlecht,   "geschlecht",     settings);
    saveLineEdit(ui->lineEdit_beruf,        "beruf",          settings);
    saveLineEdit(ui->lineEdit_spieler,      "player_name",    settings);
    saveLineEdit(ui->lineEdit_geburtsort,   "geburtsort",     settings);
    saveLineEdit(ui->lineEdit_geburtsdatum, "geburtsdatum",   settings);
    saveLineEdit(ui->lineEdit_spezies,      "spezies",        settings);

    saveTableWidget_vertical(ui->tableWidget_eigenschaften_links,    "eigenschaften_links",    { "start", "mod", "aktuell" }, settings);
    saveTableWidget_vertical(ui->tableWidget_eigenschaften_mitte,    "eigenschaften_mitte",    { "wert" },                    settings);
    saveTableWidget_vertical(ui->tableWidget_eigenschaften_rechts,   "eigenschaften_rechts",   { "wert" },                    settings);

    saveTableWidget_vertical(ui->tableWidget_vorteile,               "vorteile",               { "vorteil" },                 settings);
    saveTableWidget_vertical(ui->tableWidget_nachteile,              "nachteile",              { "nachteil" },                settings);
    saveTableWidget_vertical(ui->tableWidget_besondere_faehigkeiten, "besondere_faehigkeiten", { "faehigkeit" },              settings);
    saveTableWidget_vertical(ui->tableWidget_krankheiten,            "krankheiten",            { "krankheit" },               settings);
    saveTableWidget_vertical(ui->tableWidget_spezifische_daten,      "spezifische_daten",      { "wert" },                    settings);

    // Kampfwerte
    saveLineEdit(ui->lineEdit_nahkampfwert,  "nahkampfwert",  settings);
    saveLineEdit(ui->lineEdit_fernkampfwert, "fernkampfwert", settings);

    saveTableWidget_vertical(ui->tableWidget_fernkampf,           "fernkampf",           { "waffe", "typ", "feuerart", "reichweite", "sw", "bem", "tarnstufe", "ladekapazität" }, settings);
    saveTableWidget_vertical(ui->tableWidget_nahkampf,            "nahkampf",            { "waffe", "mind_staerke", "typ", "sw", "tarnstufe" },                                   settings);
    saveTableWidget_vertical(ui->tableWidget_granaten,            "granaten",            { "granate", "wb", "sb", "anzahl", "effekt" },                                           settings);
    saveTableWidget_vertical(ui->tableWidget_ruestung,            "ruestungen",          { "ruestung", "paz", "bew", "ini" },                                                     settings);

    // Fertigkeiten
    saveTableWidget_vertical(ui->tableWidget_fertigkeiten_links,  "fertigkeiten_links",  { "fertigkeit", "attr", "stufe", "ew" },                                                 settings);
    saveTableWidget_vertical(ui->tableWidget_fertigkeiten_mitte,  "fertigkeiten_mitte",  { "fertigkeit", "attr", "stufe", "ew" },                                                 settings);
    saveTableWidget_vertical(ui->tableWidget_fertigkeiten_rechts, "fertigkeiten_rechts", { "fertigkeit", "attr", "stufe", "ew" },                                                 settings);

    // Ausrüstung
    saveTableWidget_vertical(ui->tableWidget_ausruestung_links,   "ausruestung_links",   { "kleidung", "gewicht" },                                                               settings);
    saveTableWidget_vertical(ui->tableWidget_ausruestung_rechts,  "ausruestung_rechts",  { "kleidung", "gewicht" },                                                               settings);

    saveTableWidget_vertical(ui->tableWidget_hosentaschen,        "hosentaschen",        { "gegenstand", "gewicht" },                                                             settings);
    saveTableWidget_vertical(ui->tableWidget_proviant,            "proviant",            { "gegenstand", "anz_anwendungen", "gewicht" },                                          settings);

    saveLineEdit(ui->lineEdit_credits,   "credits",   settings);
    saveLineEdit(ui->lineEdit_sonstiges, "sonstiges", settings);

    saveTableWidget_vertical(ui->tableWidget_rucksack_links,  "rucksack_links",  { "gegenstand", "gewicht" }, settings);
    saveTableWidget_vertical(ui->tableWidget_rucksack_rechts, "rucksack_rechts", { "gegenstand", "gewicht" }, settings);
}

void Entaria2Sheet::load(QString filePath)
{
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat);

    // Eigenschaften
    loadLineEdit(ui->lineEdit_name,         "character_name", settings);
    loadLineEdit(ui->lineEdit_geschlecht,   "geschlecht",     settings);
    loadLineEdit(ui->lineEdit_beruf,        "beruf",          settings);
    loadLineEdit(ui->lineEdit_spieler,      "player_name",    settings);
    loadLineEdit(ui->lineEdit_geburtsort,   "geburtsort",     settings);
    loadLineEdit(ui->lineEdit_geburtsdatum, "geburtsdatum",   settings);
    loadLineEdit(ui->lineEdit_spezies,      "spezies",        settings);

    loadTableWidget_vertical(ui->tableWidget_eigenschaften_links,    "eigenschaften_links",    { "start", "mod", "aktuell" }, settings);
    loadTableWidget_vertical(ui->tableWidget_eigenschaften_mitte,    "eigenschaften_mitte",    { "wert" },                    settings);
    loadTableWidget_vertical(ui->tableWidget_eigenschaften_rechts,   "eigenschaften_rechts",   { "wert" },                    settings);

    loadTableWidget_vertical(ui->tableWidget_vorteile,               "vorteile",               { "vorteil" },                 settings);
    loadTableWidget_vertical(ui->tableWidget_nachteile,              "nachteile",              { "nachteil" },                settings);
    loadTableWidget_vertical(ui->tableWidget_besondere_faehigkeiten, "besondere_faehigkeiten", { "faehigkeit" },              settings);
    loadTableWidget_vertical(ui->tableWidget_krankheiten,            "krankheiten",            { "krankheit" },               settings);
    loadTableWidget_vertical(ui->tableWidget_spezifische_daten,      "spezifische_daten",      { "wert" },                    settings);

    // Kampfwerte
    loadLineEdit(ui->lineEdit_nahkampfwert,  "nahkampfwert",  settings);
    loadLineEdit(ui->lineEdit_fernkampfwert, "fernkampfwert", settings);

    loadTableWidget_vertical(ui->tableWidget_fernkampf,           "fernkampf",           { "waffe", "typ", "feuerart", "reichweite", "sw", "bem", "tarnstufe", "ladekapazität" }, settings);
    loadTableWidget_vertical(ui->tableWidget_nahkampf,            "nahkampf",            { "waffe", "mind_staerke", "typ", "sw", "tarnstufe" },                                   settings);
    loadTableWidget_vertical(ui->tableWidget_granaten,            "granaten",            { "granate", "wb", "sb", "anzahl", "effekt" },                                           settings);
    loadTableWidget_vertical(ui->tableWidget_ruestung,            "ruestungen",          { "ruestung", "paz", "bew", "ini" },                                                     settings);

    // Fertigkeiten
    loadTableWidget_vertical(ui->tableWidget_fertigkeiten_links,  "fertigkeiten_links",  { "fertigkeit", "attr", "stufe", "ew" },                                                 settings);
    loadTableWidget_vertical(ui->tableWidget_fertigkeiten_mitte,  "fertigkeiten_mitte",  { "fertigkeit", "attr", "stufe", "ew" },                                                 settings);
    loadTableWidget_vertical(ui->tableWidget_fertigkeiten_rechts, "fertigkeiten_rechts", { "fertigkeit", "attr", "stufe", "ew" },                                                 settings);

    // Ausrüstung
    loadTableWidget_vertical(ui->tableWidget_ausruestung_links,   "ausruestung_links",   { "kleidung", "gewicht" },                                                               settings);
    loadTableWidget_vertical(ui->tableWidget_ausruestung_rechts,  "ausruestung_rechts",  { "kleidung", "gewicht" },                                                               settings);

    loadTableWidget_vertical(ui->tableWidget_hosentaschen,        "hosentaschen",        { "gegenstand", "gewicht" },                                                             settings);
    loadTableWidget_vertical(ui->tableWidget_proviant,            "proviant",            { "gegenstand", "anz_anwendungen", "gewicht" },                                          settings);

    loadLineEdit(ui->lineEdit_credits,   "credits",   settings);
    loadLineEdit(ui->lineEdit_sonstiges, "sonstiges", settings);

    loadTableWidget_vertical(ui->tableWidget_rucksack_links,  "rucksack_links",  { "gegenstand", "gewicht" }, settings);
    loadTableWidget_vertical(ui->tableWidget_rucksack_rechts, "rucksack_rechts", { "gegenstand", "gewicht" }, settings);
}

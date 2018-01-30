#include "dsa5sheet.h"
#include "ui_dsa5sheet.h"
#include "sheetfunctions.h"

DSA5Sheet::DSA5Sheet(QWidget *parent) : QWidget(parent), ui(new Ui::DSA5Sheet)
{
    ui->setupUi(this);
}

DSA5Sheet::~DSA5Sheet()
{
    delete ui;
}

void DSA5Sheet::save(QString filePath)
{
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat);

    // Persönliche Daten
    saveLineEdit(ui->lineEdit_name,             "character_name",   settings);
    saveLineEdit(ui->lineEdit_familie,          "familie",          settings);
    saveLineEdit(ui->lineEdit_geburtsdatum,     "geburtsdatum",     settings);
    saveLineEdit(ui->lineEdit_spezies,          "spezies",          settings);
    saveLineEdit(ui->lineEdit_haarfarbe,        "haarfarbe",        settings);
    saveLineEdit(ui->lineEdit_kultur,           "kultur",           settings);
    saveLineEdit(ui->lineEdit_titel,            "titel",            settings);
    saveLineEdit(ui->lineEdit_charakteristika,  "charakteristika",  settings);
    saveLineEdit(ui->lineEdit_sonstiges,        "sonstiges",        settings);
    saveLineEdit(ui->lineEdit_spieler,          "player_name",      settings);
    saveLineEdit(ui->lineEdit_gebursort,        "geburtsort",       settings);
    saveLineEdit(ui->lineEdit_alter,            "alter",            settings);
    saveLineEdit(ui->lineEdit_geschlecht,       "geschlecht",       settings);
    saveLineEdit(ui->lineEdit_groesse,          "groesse",          settings);
    saveLineEdit(ui->lineEdit_gewicht,          "gewicht",          settings);
    saveLineEdit(ui->lineEdit_augenfarbe,       "augenfarbe",       settings);
    saveLineEdit(ui->lineEdit_profession,       "profession",       settings);
    saveLineEdit(ui->lineEdit_sozialstatus,     "sozialstatus",     settings);

    saveLineEdit(ui->lineEdit_erfahrungsgrad,   "erfahrungsgrad",   settings);
    saveLineEdit(ui->lineEdit_ap_gesamt,        "ap_gesamt",        settings);
    saveLineEdit(ui->lineEdit_ap_gesammelt,     "ap_gesammelt",     settings);
    saveLineEdit(ui->lineEdit_ap_ausgegeben,    "ap_ausgegeben",    settings);
    saveLineEdit(ui->lineEdit_schicksalspunkte, "schicksalspunkte", settings);

    // Vorteile, Nachteile, Allgemeine Sonderfertigkeiten
    saveTableWidget_vertical(ui->tableWidget_vorteile,                "vorteile",                      { "vorteil" },          settings);
    saveTableWidget_vertical(ui->tableWidget_nachteile,               "nachteile",                     { "nachteil" },         settings);
    saveTableWidget_vertical(ui->tableWidget_allg_sonderfertigkeiten, "allgemeine_sonderfertigkeiten", { "sonderfertigkeit" }, settings);

    // Eigenschaften, Attribute, Fertigkeiten, Sprachen & Schriften
    saveTableWidget_horizontal(ui->tableWidget_eigenschaften, "eigenschaften", { "eigenschaft" }, settings);
    saveTableWidget_vertical(ui->tableWidget_attribute,           "attribute",           { "LE", "AE", "KE", "SK", "ZK", "AW", "INI", "GS" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_fertigkeiten_links,  "fertigkeiten_links",  { "fertigkeitswert", "routine", "anmerkung" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_fertigkeiten_rechts, "fertigkeiten_rechts", { "fertigkeitswert", "routine", "anmerkung" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_sprachen,            "sprachen",            { "sprache" },
                             settings);

    // Kampf
    saveTableWidget_vertical(ui->tableWidget_kampftechniken_links,    "kampftechniken_links",    { "ktw", "at_fk", "pa" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_kampftechniken_rechts,   "kampftechniken_rechts",   { "ktw", "at_fk", "pa" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_kampfsonderfertigkeiten, "kampfsonderfertigkeiten", { "sonderfertigkeit" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_nahkampf,                "nahkampf",                { "waffe", "kampftechnik", "schaden", "tp", "at_pa_mod", "reichweite", "at", "pa", "gewicht" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_fernkampf,               "fernkampf",               { "waffe", "kampftechnik", "ladezeiten", "tp", "reichweite", "fernkampf", "munition", "gewicht" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_ruestung,                "ruestung",                { "ruestung", "rs_be", "zus_abzuege", "gewicht", "reise_schlacht" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_schild,                  "schild",                  { "schild", "strukturpunkte", "at_pa_mod", "gewicht" },
                             settings);

    // Besitz
    saveTableWidget_vertical(ui->tableWidget_geld,               "geld",               { "dukaten", "silbertaler", "heller", "kreuzer", "edelsteine", "schmuck", "sonstiges" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_ausruestung_links,  "ausruestung_links",  { "gegenstand", "gewicht", "wo_getragen" },
                             settings);
    saveTableWidget_vertical(ui->tableWidget_ausruestung_rechts, "ausruestung_rechts", { "gegenstand", "gewicht", "wo_getragen" },
                             settings);
    saveLineEdit(ui->lineEdit_gewicht_links,  "gewicht_links",  settings);
    saveLineEdit(ui->lineEdit_gewicht_rechts, "gewicht_rechts", settings);

    // Zauber & Rituale
    saveTableWidget_vertical(ui->tableWidget_zauber, "zauber", { "zauber", "probe", "fw", "kosten", "zauberdauer", "reichweite", "wirkungsdauer", "merkmal", "sf", "wirkung" }, settings);
    saveLineEdit(ui->lineEdit_leiteigenschaft_magisch, "magische_leiteigenschaft", settings);
    saveLineEdit(ui->lineEdit_merkmal,                 "merkmal",                  settings);
    saveLineEdit(ui->lineEdit_tradition_magisch,       "magische_tradition",       settings);
    saveTableWidget_vertical(ui->tableWidget_magische_sonderfertigkeiten, "magische_sonderfertigkeiten", { "sonderfertigkeit" }, settings);
    saveTableWidget_vertical(ui->tableWidget_zaubertricks,                "zaubertricks",                { "zaubertrick" },      settings);

    // Liturgien & Zeremonien
    saveTableWidget_vertical(ui->tableWidget_liturgien,                   "liturgien",                   { "liturgie", "probe", "fw", "kosten", "liturgiedauer", "reichweite", "wirkungsdauer",
                                                                                                           "aspekt", "sf", "wirkung" }, settings);
    saveLineEdit(ui->lineEdit_leiteigenschaft_klerikal, "klerikale_leiteigenschaft", settings);
    saveLineEdit(ui->lineEdit_aspekt,                   "aspekt",                    settings);
    saveLineEdit(ui->lineEdit_tradition_klerikal,       "klerikale_tradition",       settings);
    saveTableWidget_vertical(ui->tableWidget_klerikale_sonderfertigkeiten, "klerikale_sonderfertigkeiten", { "sonderfertigkeit" }, settings);
    saveTableWidget_vertical(ui->tableWidget_segnungen,                    "segnungen",                    { "segnung" },          settings);

    // Tier
    saveLineEdit(ui->lineEdit_tier_name,     "tier_name",     settings);
    saveLineEdit(ui->lineEdit_tier_typus,    "tier_typus",    settings);
    saveLineEdit(ui->lineEdit_tier_groesse,  "tier_groesse",  settings);
    saveLineEdit(ui->lineEdit_tier_asp,      "tier_asp",      settings);
    saveLineEdit(ui->lineEdit_tier_sk,       "tier_sk",       settings);
    saveLineEdit(ui->lineEdit_tier_zk,       "tier_zk",       settings);

    saveLineEdit(ui->lineEdit_tier_mu,       "tier_mu",       settings);
    saveLineEdit(ui->lineEdit_tier_kl,       "tier_kl",       settings);
    saveLineEdit(ui->lineEdit_tier_in,       "tier_in",       settings);
    saveLineEdit(ui->lineEdit_tier_ch,       "tier_ch",       settings);
    saveLineEdit(ui->lineEdit_tier_ff,       "tier_ff",       settings);
    saveLineEdit(ui->lineEdit_tier_ge,       "tier_ge",       settings);
    saveLineEdit(ui->lineEdit_tier_ko,       "tier_ko",       settings);
    saveLineEdit(ui->lineEdit_tier_kk,       "tier_kk",       settings);
    saveLineEdit(ui->lineEdit_tier_lep,      "tier_lep",      settings);
    saveLineEdit(ui->lineEdit_tier_rs,       "tier_rs",       settings);
    saveLineEdit(ui->lineEdit_tier_ini,      "tier_ini",      settings);
    saveLineEdit(ui->lineEdit_tier_gs,       "tier_gs",       settings);

    saveLineEdit(ui->lineEdit_tier_angriff,  "tier_angriff",  settings);
    saveLineEdit(ui->lineEdit_tier_at,       "tier_at",       settings);
    saveLineEdit(ui->lineEdit_tier_pa,       "tier_pa",       settings);
    saveLineEdit(ui->lineEdit_tier_tp,       "tier_tp",       settings);
    saveLineEdit(ui->lineEdit_tier_rw,       "tier_rw",       settings);
    saveLineEdit(ui->lineEdit_tier_aktionen, "tier_aktionen", settings);
    saveTableWidget_vertical(ui->tableWidget_tier_sonderfertigkeiten, "tier_sonderfertigkeiten", { "sonderfertigkeit" }, settings);
}

void DSA5Sheet::load(QString filePath)
{
    QSettings *settings = new QSettings(filePath, QSettings::IniFormat);

    // Persönliche Daten
    loadLineEdit(ui->lineEdit_name,             "character_name",   settings);
    loadLineEdit(ui->lineEdit_familie,          "familie",          settings);
    loadLineEdit(ui->lineEdit_geburtsdatum,     "geburtsdatum",     settings);
    loadLineEdit(ui->lineEdit_spezies,          "spezies",          settings);
    loadLineEdit(ui->lineEdit_haarfarbe,        "haarfarbe",        settings);
    loadLineEdit(ui->lineEdit_kultur,           "kultur",           settings);
    loadLineEdit(ui->lineEdit_titel,            "titel",            settings);
    loadLineEdit(ui->lineEdit_charakteristika,  "charakteristika",  settings);
    loadLineEdit(ui->lineEdit_sonstiges,        "sonstiges",        settings);
    loadLineEdit(ui->lineEdit_spieler,          "player_name",      settings);
    loadLineEdit(ui->lineEdit_gebursort,        "geburtsort",       settings);
    loadLineEdit(ui->lineEdit_alter,            "alter",            settings);
    loadLineEdit(ui->lineEdit_geschlecht,       "geschlecht",       settings);
    loadLineEdit(ui->lineEdit_groesse,          "groesse",          settings);
    loadLineEdit(ui->lineEdit_gewicht,          "gewicht",          settings);
    loadLineEdit(ui->lineEdit_augenfarbe,       "augenfarbe",       settings);
    loadLineEdit(ui->lineEdit_profession,       "profession",       settings);
    loadLineEdit(ui->lineEdit_sozialstatus,     "sozialstatus",     settings);

    loadLineEdit(ui->lineEdit_erfahrungsgrad,   "erfahrungsgrad",   settings);
    loadLineEdit(ui->lineEdit_ap_gesamt,        "ap_gesamt",        settings);
    loadLineEdit(ui->lineEdit_ap_gesammelt,     "ap_gesammelt",     settings);
    loadLineEdit(ui->lineEdit_ap_ausgegeben,    "ap_ausgegeben",    settings);
    loadLineEdit(ui->lineEdit_schicksalspunkte, "schicksalspunkte", settings);

    // Vorteile, Nachteile, Allgemeine Sonderfertigkeiten
    loadTableWidget_vertical(ui->tableWidget_vorteile,                "vorteile",                      { "vorteil" },          settings);
    loadTableWidget_vertical(ui->tableWidget_nachteile,               "nachteile",                     { "nachteil" },         settings);
    loadTableWidget_vertical(ui->tableWidget_allg_sonderfertigkeiten, "allgemeine_sonderfertigkeiten", { "sonderfertigkeit" }, settings);

    // Eigenschaften, Attribute, Fertigkeiten, Sprachen & Schriften
    loadTableWidget_horizontal(ui->tableWidget_eigenschaften, "eigenschaften", { "eigenschaft" }, settings);
    loadTableWidget_vertical(ui->tableWidget_attribute,           "attribute",           { "LE", "AE", "KE", "SK", "ZK", "AW", "INI", "GS" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_fertigkeiten_links,  "fertigkeiten_links",  { "fertigkeitswert", "routine", "anmerkung" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_fertigkeiten_rechts, "fertigkeiten_rechts", { "fertigkeitswert", "routine", "anmerkung" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_sprachen,            "sprachen",            { "sprache" },
                             settings);

    // Kampf
    loadTableWidget_vertical(ui->tableWidget_kampftechniken_links,    "kampftechniken_links",    { "ktw", "at_fk", "pa" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_kampftechniken_rechts,   "kampftechniken_rechts",   { "ktw", "at_fk", "pa" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_kampfsonderfertigkeiten, "kampfsonderfertigkeiten", { "sonderfertigkeit" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_nahkampf,                "nahkampf",                { "waffe", "kampftechnik", "schaden", "tp", "at_pa_mod", "reichweite", "at", "pa", "gewicht" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_fernkampf,               "fernkampf",               { "waffe", "kampftechnik", "ladezeiten", "tp", "reichweite", "fernkampf", "munition", "gewicht" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_ruestung,                "ruestung",                { "ruestung", "rs_be", "zus_abzuege", "gewicht", "reise_schlacht" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_schild,                  "schild",                  { "schild", "strukturpunkte", "at_pa_mod", "gewicht" },
                             settings);

    // Besitz
    loadTableWidget_vertical(ui->tableWidget_geld,               "geld",               { "dukaten", "silbertaler", "heller", "kreuzer", "edelsteine", "schmuck", "sonstiges" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_ausruestung_links,  "ausruestung_links",  { "gegenstand", "gewicht", "wo_getragen" },
                             settings);
    loadTableWidget_vertical(ui->tableWidget_ausruestung_rechts, "ausruestung_rechts", { "gegenstand", "gewicht", "wo_getragen" },
                             settings);
    loadLineEdit(ui->lineEdit_gewicht_links,  "gewicht_links",  settings);
    loadLineEdit(ui->lineEdit_gewicht_rechts, "gewicht_rechts", settings);

    // Zauber & Rituale
    loadTableWidget_vertical(ui->tableWidget_zauber, "zauber", { "zauber", "probe", "fw", "kosten", "zauberdauer", "reichweite", "wirkungsdauer", "merkmal", "sf", "wirkung" }, settings);
    loadLineEdit(ui->lineEdit_leiteigenschaft_magisch, "magische_leiteigenschaft", settings);
    loadLineEdit(ui->lineEdit_merkmal,                 "merkmal",                  settings);
    loadLineEdit(ui->lineEdit_tradition_magisch,       "magische_tradition",       settings);
    loadTableWidget_vertical(ui->tableWidget_magische_sonderfertigkeiten, "magische_sonderfertigkeiten", { "sonderfertigkeit" },
                                                                                                    settings);
    loadTableWidget_vertical(ui->tableWidget_zaubertricks,                "zaubertricks",                { "zaubertrick" },
                                                                                                    settings);

    // Liturgien & Zeremonien
    loadTableWidget_vertical(ui->tableWidget_liturgien, "liturgien", { "liturgie", "probe", "fw", "kosten", "liturgiedauer", "reichweite", "wirkungsdauer",
                                                                       "aspekt", "sf", "wirkung" }, settings);
    loadLineEdit(ui->lineEdit_leiteigenschaft_klerikal, "klerikale_leiteigenschaft", settings);
    loadLineEdit(ui->lineEdit_aspekt,                   "aspekt",                    settings);
    loadLineEdit(ui->lineEdit_tradition_klerikal,       "klerikale_tradition",       settings);
    loadTableWidget_vertical(ui->tableWidget_klerikale_sonderfertigkeiten, "klerikale_sonderfertigkeiten", { "sonderfertigkeit" }, settings);
    loadTableWidget_vertical(ui->tableWidget_segnungen,                    "segnungen",                    { "segnung" },          settings);

    // Tier
    loadLineEdit(ui->lineEdit_tier_name,     "tier_name",     settings);
    loadLineEdit(ui->lineEdit_tier_typus,    "tier_typus",    settings);
    loadLineEdit(ui->lineEdit_tier_groesse,  "tier_groesse",  settings);
    loadLineEdit(ui->lineEdit_tier_asp,      "tier_asp",      settings);
    loadLineEdit(ui->lineEdit_tier_sk,       "tier_sk",       settings);
    loadLineEdit(ui->lineEdit_tier_zk,       "tier_zk",       settings);

    loadLineEdit(ui->lineEdit_tier_mu,       "tier_mu",       settings);
    loadLineEdit(ui->lineEdit_tier_kl,       "tier_kl",       settings);
    loadLineEdit(ui->lineEdit_tier_in,       "tier_in",       settings);
    loadLineEdit(ui->lineEdit_tier_ch,       "tier_ch",       settings);
    loadLineEdit(ui->lineEdit_tier_ff,       "tier_ff",       settings);
    loadLineEdit(ui->lineEdit_tier_ge,       "tier_ge",       settings);
    loadLineEdit(ui->lineEdit_tier_ko,       "tier_ko",       settings);
    loadLineEdit(ui->lineEdit_tier_kk,       "tier_kk",       settings);
    loadLineEdit(ui->lineEdit_tier_lep,      "tier_lep",      settings);
    loadLineEdit(ui->lineEdit_tier_rs,       "tier_rs",       settings);
    loadLineEdit(ui->lineEdit_tier_ini,      "tier_ini",      settings);
    loadLineEdit(ui->lineEdit_tier_gs,       "tier_gs",       settings);

    loadLineEdit(ui->lineEdit_tier_angriff,  "tier_angriff",  settings);
    loadLineEdit(ui->lineEdit_tier_at,       "tier_at",       settings);
    loadLineEdit(ui->lineEdit_tier_pa,       "tier_pa",       settings);
    loadLineEdit(ui->lineEdit_tier_tp,       "tier_tp",       settings);
    loadLineEdit(ui->lineEdit_tier_rw,       "tier_rw",       settings);
    loadLineEdit(ui->lineEdit_tier_aktionen, "tier_aktionen", settings);
    loadTableWidget_vertical(ui->tableWidget_tier_sonderfertigkeiten, "tier_sonderfertigkeiten", { "sonderfertigkeit" }, settings);
}

import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

import gm.companion.dsa5sheet 1.0
import gm.companion.charactersaveload 1.0
import "./sheet_elements"

Page {
    id: sheet_page

    property string character_name: ""

    function save() {
        save_load.initializeSaving()
    }

    function load() {
        save_load.initializeLoading()
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    DSA5SheetTool {
        id: sheet_tool

        character_name: sheet_page.character_name
    }

    CharacterSaveLoad {
        id: save_load

        onSaving: swipe_view.save()

        onLoading: swipe_view.load()
    }

    Column {
        anchors.fill: parent
        spacing: 5

        TabBar {
            id: swipe_tab_bar
            width: parent.width

            TabButton {
                text: qsTr("Persönliche Daten")
            }

            TabButton {
                text: qsTr("Spielwerte")
            }
            TabButton {
                text: qsTr("Kampf")
            }
            TabButton {
                text: qsTr("Besitz")
            }
            TabButton {
                text: qsTr("Zauber / Rituale")
            }
            TabButton {
                text: qsTr("Liturgien / Zeremonien")
            }
            TabButton {
                text: qsTr("Tier")
            }
        }

        SwipeView {
            id: swipe_view
            clip: true
            width: parent.width
            height: parent.height - swipe_tab_bar.height - parent.spacing

            currentIndex: swipe_tab_bar.currentIndex
            function save() {
                var table_names = new Array(0)
                var table_values = new Array(0)

                // Persönliche Daten
                table_names.push(daten1_table.save()[0])
                table_values.push(daten1_table.save()[1])

                table_names.push(daten2_table.save()[0])
                table_values.push(daten2_table.save()[1])

                table_names.push(daten3_table.save()[0])
                table_values.push(daten3_table.save()[1])

                table_names.push(vorteile_table.save()[0])
                table_values.push(vorteile_table.save()[1])

                table_names.push(nachteile_table.save()[0])
                table_values.push(nachteile_table.save()[1])

                table_names.push(allgemeine_sonderfertigkeiten_table.save()[0])
                table_values.push(allgemeine_sonderfertigkeiten_table.save()[1])

                table_names.push(sprachen_table.save()[0])
                table_values.push(sprachen_table.save()[1])

                // Spielwerte
                table_names.push(eigenschaften_table.save()[0])
                table_values.push(eigenschaften_table.save()[1])

                table_names.push(fertigkeiten1_table.save()[0])
                table_values.push(fertigkeiten1_table.save()[1])

                table_names.push(fertigkeiten2_table.save()[0])
                table_values.push(fertigkeiten2_table.save()[1])

                // Kampf
                table_names.push(kampftechniken1_table.save()[0])
                table_values.push(kampftechniken1_table.save()[1])

                table_names.push(kampftechniken2_table.save()[0])
                table_values.push(kampftechniken2_table.save()[1])

                table_names.push(kampfsonderfertigkeiten_table.save()[0])
                table_values.push(kampfsonderfertigkeiten_table.save()[1])

                table_names.push(nahkampf_table.save()[0])
                table_values.push(nahkampf_table.save()[1])

                table_names.push(fernkampf_table.save()[0])
                table_values.push(fernkampf_table.save()[1])

                table_names.push(rüstung_table.save()[0])
                table_values.push(rüstung_table.save()[1])

                table_names.push(schild_table.save()[0])
                table_values.push(schild_table.save()[1])

                // Besitz
                table_names.push(ausrüstung1_table.save()[0])
                table_values.push(ausrüstung1_table.save()[1])

                table_names.push(ausrüstung2_table.save()[0])
                table_values.push(ausrüstung2_table.save()[1])

                // Zauber / Rituale
                table_names.push(zauber_table.save()[0])
                table_values.push(zauber_table.save()[1])

                table_names.push(zauberer_table.save()[0])
                table_values.push(zauberer_table.save()[1])

                table_names.push(magische_sonderfertigkeiten_table.save()[0])
                table_values.push(magische_sonderfertigkeiten_table.save()[1])

                table_names.push(zaubertricks_table.save()[0])
                table_values.push(zaubertricks_table.save()[1])

                // Liturgien / Zeremonien
                table_names.push(liturgien_table.save()[0])
                table_values.push(liturgien_table.save()[1])

                table_names.push(geweihter_table.save()[0])
                table_values.push(geweihter_table.save()[1])

                table_names.push(klerikale_sonderfertigkeiten_table.save()[0])
                table_values.push(klerikale_sonderfertigkeiten_table.save()[1])

                table_names.push(segnungen_table.save()[0])
                table_values.push(segnungen_table.save()[1])

                // Tier
                table_names.push(tier_allgemein1_table.save()[0])
                table_values.push(tier_allgemein1_table.save()[1])

                table_names.push(tier_allgemein2_table.save()[0])
                table_values.push(tier_allgemein2_table.save()[1])

                table_names.push(tier_kampf1_table.save()[0])
                table_values.push(tier_kampf1_table.save()[1])

                table_names.push(tier_kampf2_table.save()[0])
                table_values.push(tier_kampf2_table.save()[1])

                table_names.push(tier_sonderfertigkeiten_table.save()[0])
                table_values.push(tier_sonderfertigkeiten_table.save()[1])

                save_load.saveCharacter(sheet_page.character_name, table_names,
                                        table_values, [""], [""], [""])
            }

            function load() {
                // Persönliche Daten
                daten1_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "daten1", 1)
                daten1_table.fillList()

                daten2_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "daten2", 1)
                daten2_table.fillList()

                daten3_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "daten3", 1)
                daten3_table.fillList()

                vorteile_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name, "vorteile", 0)
                vorteile_table.fillList()

                nachteile_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name, "nachteile", 0)
                nachteile_table.fillList()

                allgemeine_sonderfertigkeiten_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name,
                            "allgemeine_sonderfertigkeiten", 0)
                allgemeine_sonderfertigkeiten_table.fillList()

                sprachen_table.col1_list = save_load.loadTable(
                            sheet_tool.character_name, "sprachen", 0)
                sprachen_table.fillList()

                // Spielwerte
                eigenschaften_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "eigenschaften", 1)
                eigenschaften_table.fillList()

                fertigkeiten1_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "fertigkeiten1", 1)
                fertigkeiten1_table.fillList()

                fertigkeiten2_table.col2_list = save_load.loadTable(
                            sheet_tool.character_name, "fertigkeiten2", 1)
                fertigkeiten2_table.fillList()
            }

            // Personal Data
            Column {
                spacing: 5

                Row {
                    width: parent.width - parent.leftPadding * 2
                    height: (parent.height - parent.topPadding * 2 - parent.spacing) / 2

                    EditableTable {
                        id: daten1_table
                        height: parent.height
                        width: (parent.width - parent.spacing * 2
                                - parent.leftPadding * 2 - daten3_table.width) / 2

                        character_name: sheet_tool.character_name
                        table_name: "daten1"

                        row_count: 9
                        columns_visible: false
                        col1_list: ["Name", "Familie", "Geburtsdatum", "Spezies", "Haarfarbe", "Kultur", "Titel", "Charakteristika", "Sonstiges"]
                        col2_list: [sheet_tool.character_name]

                        col1_width_divisor: 3
                    }

                    EditableTable {
                        id: daten2_table
                        height: parent.height
                        width: (parent.width - parent.spacing * 2
                                - parent.leftPadding * 2 - daten3_table.width) / 2

                        character_name: sheet_tool.character_name
                        table_name: "daten2"

                        row_count: 9
                        columns_visible: false
                        col1_list: ["Spieler", "Geburtsort", "Alter", "Geschlecht", "Größe", "Gewicht", "Augenfarbe", "Profession", "Sozialstatus"]
                        col2_list: [sheet_tool.player_name]
                        col1_width_divisor: 3
                    }

                    EditableTable {
                        id: daten3_table
                        height: parent.height
                        width: (parent.width - parent.spacing * 2 - parent.leftPadding * 2) / 5

                        character_name: sheet_tool.character_name
                        table_name: "daten3"

                        row_count: 5
                        columns_visible: false
                        col1_list: ["Erfahrungsgrad", "AP Gesamt", "AP Gesammelt", "AP Ausgegeben", "Schicksalspunkte"]
                        col1_width_divisor: 2
                    }
                }

                Row {
                    width: parent.width - parent.leftPadding * 2
                    height: (parent.height - parent.topPadding * 2 - parent.spacing) / 2
                    spacing: 5

                    EditableTable {
                        id: vorteile_table
                        height: parent.height
                        width: (parent.width - parent.spacing * 3 - parent.leftPadding * 2) / 4

                        table_name: "vorteile"
                        character_name: sheet_tool.character_name

                        column_count: 1
                        col1_name: "Vorteile"
                        col1_width_divisor: 1
                    }

                    EditableTable {
                        id: nachteile_table
                        height: parent.height
                        width: (parent.width - parent.spacing * 3 - parent.leftPadding * 2) / 4

                        table_name: "nachteile"
                        character_name: sheet_tool.character_name

                        column_count: 1
                        col1_name: "Nachteile"
                        col1_width_divisor: 1
                    }

                    EditableTable {
                        id: allgemeine_sonderfertigkeiten_table
                        height: parent.height
                        width: (parent.width - parent.spacing * 3 - parent.leftPadding * 2) / 4

                        table_name: "allgemeine_sonderfertigkeiten"
                        character_name: sheet_tool.character_name

                        column_count: 1
                        col1_name: "Allgemeine Sonderfertigkeiten"
                        col1_width_divisor: 1
                    }

                    EditableTable {
                        id: sprachen_table
                        width: (parent.width - parent.spacing * 3 - parent.leftPadding * 2) / 4
                        height: parent.height

                        table_name: "sprachen"
                        character_name: sheet_tool.character_name

                        //                        row_count: 5
                        column_count: 1
                        col1_name: "Sprachen"
                        col1_width_divisor: 1
                    }
                }
            }

            // Playing Values
            Row {
                spacing: 5

                EditableTable {
                    id: eigenschaften_table
                    height: parent.height
                    width: (parent.width - parent.spacing * 2) / 6

                    table_name: "eigenschaften"
                    character_name: sheet_tool.character_name

                    col1_width_divisor: 2
                    columns_visible: false
                    col1_list: ["MU", "KL", "IN", "CH", "FF", "GE", "KO", "KK", "LE", "AE", "KE", "SK", "ZK", "AW", "INI", "GS"]
                    row_count: 16
                }

                EditableTable {
                    id: fertigkeiten1_table
                    width: (parent.width - eigenschaften_table.width - parent.spacing * 2) / 2
                    height: parent.height

                    table_name: "fertigkeiten1"
                    character_name: sheet_tool.character_name

                    col1_list: ["Körpertalente", "Fliegen", "Gaukeleien", "Klettern", "Körperbeherrschung", "Kraftakt", "Reiten", "Schwimmen", "Selbstbeherrschung", "Singen", "Sinnesschärfe", "Tanzen", "Taschendiebstahl", "Verbergen", "Zechen", "Gesellschaftstalente", "Bekehren & Überzeugen", "Betören", "Einschüchtern", "Etikette", "Gassenwissen", "Menschenkenntnis", "Überreden", "Verkleiden", "Willenskraft", "Naturtalente", "Fährtensuchen", "Fesseln", "Fischen & Angeln", "Orientierung", "Pflanzenkunde", "Tierkunde", "Wildnisleben"]
                    col1_name: "Fertigkeit"
                    col1_width_divisor: 3
                    col2_name: "Fertigkeitswert"
                }

                EditableTable {
                    id: fertigkeiten2_table
                    width: (parent.width - eigenschaften_table.width - parent.spacing * 2) / 2
                    height: parent.height

                    table_name: "fertigkeiten2"
                    character_name: sheet_tool.character_name

                    col1_name: "Fertigkeit"
                    col1_width_divisor: 3
                    col1_list: ["Wissenstalente", "Brett & Glücksspiel", "Geographie", "Geschichtswissen", "Götter & Kulte", "Kriegskunst", "Magiekunde", "Mechanik", "Rechnen", "Rechtskunde", "Sagen & Legenden", "Sphärenkunde", "Sternkunde", "Handwerkskunde", "Alchimie", "Boote & Schiffe", "Fahrzeuge", "Handel", "Heilkunde Gift", "Heilkunde Krankheiten", "Heilkunde Seele", "Heilkunde Wunden", "Holzbearbeitung", "Lebensmittelbearbeitung", "Lederbearbeitung", "Malen & Zeichnen", "Metallbearbeitung", "Musizieren", "Schlösserknacken", "Steinbearbeitung", "Stoffbearbeitung"]
                    col2_name: "Fertigkeitswert"
                }
            }

            // Fight
            Column {
                spacing: 5

                Row {
                    id: fight1_row
                    width: parent.width
                    height: (parent.height - parent.spacing * 3) / 4
                    spacing: 5

                    EditableTable {
                        id: kampftechniken1_table
                        width: (parent.width - parent.spacing * 2) / 3
                        height: parent.height
                        column_count: 4
                        table_name: "kampftechniken1"
                        character_name: sheet_tool.character_name

                        row_count: 6
                        col1_name: "Kampftechnik"
                        col1_list: ["Armbrüste", "Bögen", "Dolche", "Fechtwaffen", "Hiebwaffen", "Kettenwaffen"]
                        col2_name: "KTW"
                        col3_name: "AT / FK"
                        col4_name: "PA"
                        col4_list: ["X", "X", "", "", "", "X"]
                    }

                    EditableTable {
                        id: kampftechniken2_table
                        width: (parent.width - parent.spacing * 2) / 3
                        height: parent.height
                        column_count: 4
                        table_name: "kampftechniken2"
                        character_name: sheet_tool.character_name

                        row_count: 7
                        col1_name: "Kampftechnik"
                        col1_list: ["Raufen", "Schilde", "Schwerter", "Stangenwaffen", "Wurfwaffen", "Zweihandhiebwaffen", "Zweihandschwerter"]
                        col2_name: "KTW"
                        col3_name: "AT / FK"
                        col4_name: "PA"
                        col4_list: ["", "", "", "", "X"]
                    }

                    EditableTable {
                        id: kampfsonderfertigkeiten_table
                        width: (parent.width - parent.spacing * 2) / 3
                        height: parent.height
                        column_count: 1
                        col1_name: "Kampfsonderfertigkeiten"

                        table_name: "kampfsonderfertigkeiten"
                        character_name: sheet_tool.character_name
                    }
                }

                EditableTable {
                    id: nahkampf_table
                    width: parent.width
                    height: (parent.height - parent.spacing * 3) / 4

                    column_count: 9
                    row_count: 5
                    table_name: "nahkampf"
                    character_name: sheet_tool.character_name

                    col1_name: "Nahkampfwaffe"
                    col2_name: "Kampftechnik"
                    col3_name: "Schadensb."
                    col4_name: "TP"
                    col5_name: "AT / PA Mod."
                    col6_name: "Reichweite"
                    col7_name: "AT"
                    col8_name: "PA"
                    col9_name: "Gewicht"
                }

                EditableTable {
                    id: fernkampf_table
                    width: parent.width
                    height: (parent.height - parent.spacing * 3) / 4

                    column_count: 8
                    row_count: 5
                    table_name: "fernkampf"
                    character_name: sheet_tool.character_name

                    col1_name: "Fernkampfwaffe"
                    col2_name: "Kampftechnik"
                    col3_name: "Ladezeiten."
                    col4_name: "TP"
                    col5_name: "Reichweite"
                    col6_name: "Fernkampf"
                    col7_name: "Munition"
                    col8_name: "Gewicht"
                }

                Row {
                    id: rüstung_schild_row
                    width: parent.width
                    height: (parent.height - parent.spacing * 3) / 4
                    spacing: 5

                    EditableTable {
                        id: rüstung_table
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height

                        column_count: 5
                        row_count: 5
                        table_name: "ruestung"
                        character_name: sheet_tool.character_name

                        col1_name: "Rüstung"
                        col2_name: "RS / BE"
                        col3_name: "Zus. Abzüge"
                        col4_name: "Gewicht"
                        col5_name: "Reise, Schlacht ..."
                    }

                    EditableTable {
                        id: schild_table
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height

                        column_count: 4
                        row_count: 5
                        table_name: "schilde"
                        character_name: sheet_tool.character_name

                        col1_name: "Schild / Parierwaffe"
                        col2_name: "Strukturp."
                        col3_name: "AT / PA Mod."
                        col4_name: "Gewicht"
                    }
                }
            }

            // Inventory
            Row {
                spacing: 5

                EditableTable {
                    id: ausrüstung1_table
                    width: (parent.width - parent.spacing * 2 - geldbeutel_table.width) / 2
                    height: parent.height

                    table_name: "ausruestung1"
                    character_name: sheet_tool.character_name

                    column_count: 3
                    row_count: 30

                    col1_name: "Gegenstand"
                    col2_name: "Gewicht"
                    col2_width_divisor: 5
                    col3_name: "Wo getragen?"
                }

                EditableTable {
                    id: ausrüstung2_table
                    width: (parent.width - parent.spacing * 2 - geldbeutel_table.width) / 2
                    height: parent.height

                    table_name: "ausruestung2"
                    character_name: sheet_tool.character_name

                    column_count: 3
                    row_count: 30

                    col1_name: "Gegenstand"
                    col2_name: "Gewicht"
                    col2_width_divisor: 5
                    col3_name: "Wo getragen?"
                }

                EditableTable {
                    id: geldbeutel_table
                    width: parent.width / 6
                    height: parent.height

                    table_name: "geldbeutel"
                    character_name: sheet_tool.character_name

                    columns_visible: false
                    column_count: 2

                    col1_list: ["Dukaten", "Silbertaler", "Heller", "Kreuzer", "Edelsteine", "Schmuck", "Sonstiges"]
                }
            }

            // Magic
            Column {
                spacing: 5

                EditableTable {
                    id: zauber_table
                    width: parent.width
                    height: (parent.height - parent.spacing) / 2

                    table_name: "zauber"
                    character_name: sheet_tool.character_name
                    column_count: 10
                    row_count: 20

                    col1_name: "Zauber / Ritual"
                    col2_name: "Probe"
                    col3_name: "FW"
                    col4_name: "Kosten"
                    col5_name: "Zauberdauer"
                    col6_name: "Reichweite"
                    col7_name: "Wirkungsdauer"
                    col8_name: "Merkmal"
                    col9_name: "SF."
                    col10_name: "Wirkung"
                }

                Row {
                    width: parent.width
                    height: (parent.height - parent.spacing) / 2
                    spacing: 5

                    EditableTable {
                        id: zauberer_table
                        width: (parent.width - parent.spacing * 2) / 4
                        height: parent.height

                        table_name: "zauberer"
                        character_name: sheet_tool.character_name

                        column_count: 2
                        columns_visible: false
                        row_count: 3

                        col1_list: ["Leiteigenschaft", "Merkmal", "Tradition"]
                    }

                    EditableTable {
                        id: magische_sonderfertigkeiten_table
                        width: (parent.width - parent.spacing * 2 - zauberer_table.width) / 2
                        height: parent.height

                        table_name: "magische_sonderfertigkeiten"
                        character_name: sheet_tool.character_name
                        column_count: 1
                        row_count: 15

                        col1_name: "Magische Sonderfertigkeiten"
                    }

                    EditableTable {
                        id: zaubertricks_table
                        width: (parent.width - parent.spacing * 2 - zauberer_table.width) / 2
                        height: parent.height

                        table_name: "zaubertricks"
                        character_name: sheet_tool.character_name
                        column_count: 1
                        row_count: 15

                        col1_name: "Zaubertricks"
                    }
                }
            }

            // Clerical
            Column {
                spacing: 5

                EditableTable {
                    id: liturgien_table
                    width: parent.width
                    height: (parent.height - parent.spacing) / 2

                    table_name: "liturgien"
                    character_name: sheet_tool.character_name
                    column_count: 10
                    row_count: 20

                    col1_name: "Liturgie / Zeremonie"
                    col2_name: "Probe"
                    col3_name: "FW"
                    col4_name: "Kosten"
                    col5_name: "Liturgiedauer"
                    col6_name: "Reichweite"
                    col7_name: "Wirkungsdauer"
                    col8_name: "Aspekt"
                    col9_name: "SF."
                    col10_name: "Wirkung"
                }

                Row {
                    width: parent.width
                    height: (parent.height - parent.spacing) / 2
                    spacing: 5

                    EditableTable {
                        id: geweihter_table
                        width: (parent.width - parent.spacing * 2) / 4
                        height: parent.height

                        table_name: "geweihter"
                        character_name: sheet_tool.character_name
                        column_count: 2
                        columns_visible: false
                        row_count: 3

                        col1_list: ["Leiteigenschaft", "Aspekt", "Tradition"]
                    }

                    EditableTable {
                        id: klerikale_sonderfertigkeiten_table
                        width: (parent.width - parent.spacing * 2 - geweihter_table.width) / 2
                        height: parent.height

                        table_name: "klerikale_sonderfertigkeiten"
                        character_name: sheet_tool.character_name
                        column_count: 1
                        row_count: 15

                        col1_name: "Klerikale Sonderfertigkeiten"
                    }

                    EditableTable {
                        id: segnungen_table
                        width: (parent.width - parent.spacing * 2 - geweihter_table.width) / 2
                        height: parent.height

                        table_name: "segnungen"
                        character_name: sheet_tool.character_name
                        column_count: 1
                        row_count: 15

                        col1_name: "Segnungen"
                    }
                }
            }

            // Pet
            Column {
                spacing: 5

                Text {
                    id: tier_allgemein_text
                    text: "Allgemeine Daten"
                    color: color_scheme.textColor
                }

                Row {
                    id: tier_allgemein_row
                    width: parent.width
                    height: (parent.height - parent.spacing * 4 - tier_allgemein_text.height
                             - tier_kampf_text.height - tier_kampf_row.height) / 2

                    EditableTable {
                        id: tier_allgemein1_table
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height

                        table_name: "tier_allgemein1"
                        character_name: sheet_tool.character_name
                        column_count: 2
                        columns_visible: false

                        col1_list: ["Name", "Typus", "Größenkategorie", "LeP", "ASP", "SK", "ZK", "RS", "INI", "GS"]
                    }

                    EditableTable {
                        id: tier_allgemein2_table
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height

                        table_name: "tier_allgemein2"
                        character_name: sheet_tool.character_name
                        column_count: 2
                        columns_visible: false
                        row_count: 8

                        col1_list: ["MU", "KL", "IN", "CH", "FF", "GE", "KO", "KK"]
                    }
                }

                Text {
                    id: tier_kampf_text
                    text: "Kampf"
                    color: color_scheme.textColor
                }

                Row {
                    id: tier_kampf_row
                    width: parent.width
                    height: (parent.height - parent.spacing * 4
                             - tier_allgemein_text.height - tier_kampf_text.height) / 5
                    spacing: 5

                    EditableTable {
                        id: tier_kampf1_table
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height

                        table_name: "tier_kampf1"
                        character_name: sheet_tool.character_name
                        column_count: 2
                        columns_visible: false
                        row_count: 4

                        col1_list: ["Angriff", "Aktionen"]
                    }

                    EditableTable {
                        id: tier_kampf2_table
                        width: (parent.width - parent.spacing) / 2
                        height: parent.height

                        table_name: "tier_kampf2"
                        character_name: sheet_tool.character_name
                        column_count: 2
                        columns_visible: false
                        row_count: 4

                        col1_list: ["AT", "PA", "TP", "RW"]
                    }
                }

                EditableTable {
                    id: tier_sonderfertigkeiten_table
                    width: parent.width
                    height: (parent.height - parent.spacing * 4 - tier_allgemein_text.height
                             - tier_kampf_text.height - tier_kampf_row.height) / 2

                    table_name: "tier_sonderfertigkeiten"
                    character_name: sheet_tool.character_name
                    row_count: 10
                    column_count: 1
                    col1_name: "Sonderfertigkeiten"
                }
            }
        }
    }
}

import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../components"
import "./dsa5"
import FontAwesome 2.0

Page {
    id: root

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: Rectangle {
        visible: character_dsa5_viewer.subCategories.length > 0
        color: color_scheme.menuColor
        height: color_scheme.toolbarHeight

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Repeater {
                model: character_dsa5_viewer.subCategories

                ToolBarTextButton {
                    labeltext: modelData
                    onClicked: character_dsa5_viewer.setCurrentSubCategory(
                                   index)

                    Rectangle {
                        visible: index === character_dsa5_viewer.subCategoryIndex

                        color: color_scheme.toolbarTextColor
                        height: 2
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 5
                    }
                }
            }
        }
    }

    Connections {
        target: character_tool

        function onCategoryIndexChanged() {
            switch (character_tool.categoryIndex) {
            case 0:
                page_stack.replace(profile_page)
                break
            case 1:
                page_stack.replace(attributes_page)
                break
            case 2:
                page_stack.replace(advantages_page)
                break
            case 3:
                page_stack.replace(abilities_page)
                break
            case 4:
                page_stack.replace(belongings_page)
                break
            }
        }
    }

    StackView {
        id: page_stack
        anchors.fill: parent

        //        currentIndex: character_tool.categoryIndex
        initialItem: profile_page
        clip: true

        replaceEnter: Transition {}

        replaceExit: Transition {}

        StackView {
            id: profile_page
            clip: true
            initialItem: profile_overview
            visible: page_stack.currentItem === this

            replaceEnter: Transition {}

            replaceExit: Transition {}

            Connections {
                target: character_dsa5_viewer
                function onSubCategoryIndexChanged() {
                    if (character_tool.categoryIndex == 0) {
                        switch (character_dsa5_viewer.subCategoryIndex) {
                        case 0:
                            profile_page.replace(profile_overview)
                            break
                        case 1:
                            profile_page.replace(profile_rules)
                            break
                        }
                    }
                }
            }

            ProfileOverview {
                id: profile_overview
                visible: profile_page.currentItem === this
            }

            ProfileRules {
                id: profile_rules
                visible: profile_page.currentItem === this
            }
        }

        // Attributes
        Attributes {
            id: attributes_page
            visible: page_stack.currentItem === this
        }

        // Advantages / Disadvantages
        Advantages {
            id: advantages_page
            visible: page_stack.currentItem === this
        }

        // Abilities
        StackView {
            id: abilities_page
            clip: true
            initialItem: talents_page
            visible: page_stack.currentItem === this

            replaceEnter: Transition {}

            replaceExit: Transition {}

            Connections {
                target: character_dsa5_viewer
                function onSubCategoryIndexChanged() {
                    if (character_tool.categoryIndex == 3) {
                        switch (character_dsa5_viewer.subCategoryIndex) {
                        case 0:
                            abilities_page.replace(talents_page)
                            break
                        case 1:
                            abilities_page.replace(combat_page)
                            break
                        case 2:
                            abilities_page.replace(special_page)
                            break
                        case 3:
                            abilities_page.replace(spells_page)
                            break
                        case 4:
                            abilities_page.replace(liturgies_page)
                            break
                        }
                    }
                }
            }

            AbilitiesTalents {
                id: talents_page
                visible: abilities_page.currentItem === this
            }

            AbilitiesCombat {
                id: combat_page
                visible: abilities_page.currentItem === this
            }

            AbilitiesSpecial {
                id: special_page
                visible: abilities_page.currentItem === this
            }

            AbilitiesSpells {
                id: spells_page
                visible: abilities_page.currentItem === this
            }

            AbilitiesLiturgies {
                id: liturgies_page
                visible: abilities_page.currentItem === this
            }
        }

        // Belongings
        StackView {
            id: belongings_page
            clip: true
            initialItem: items_page
            visible: page_stack.currentItem === this

            replaceEnter: Transition {}

            replaceExit: Transition {}

            BelongingsItems {
                id: items_page
                visible: belongings_page.currentItem === this
            }

            BelongingsPets {
                id: pets_page
                visible: belongings_page.currentItem === this
            }
        }
    }
}

import QtQuick 2.9
import QtQuick.Controls 2.2

import "./characters"
import "../components"
import FontAwesome 2.0

Page {
    id: root

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: Rectangle {
        id: top_rect
        height: color_scheme.toolbarHeight
        color: color_scheme.menuColor

        Row {
            id: top_left_row
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            padding: 10
            spacing: 10

            // Active
            ToolBarIconButton {
                fa_icon: FontAwesome.user
                onClicked: character_tool.displayActiveCharacters(true)
            }

            // Inactive
            ToolBarIconButton {
                fa_icon: FontAwesome.userSlash
                onClicked: character_tool.displayActiveCharacters(false)
            }
        }

        // Image buttons
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Repeater {
                model: character_tool.categories

                ToolBarTextButton {
                    labeltext: modelData
                    onClicked: character_tool.setCurrentCategory(index)

                    Rectangle {
                        visible: index === character_tool.categoryIndex

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

    // Left bar
    ScrollView {
        id: character_scrollview
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 5

        width: 175
        clip: true
        contentWidth: -1
        contentHeight: active_column.implicitHeight

        Column {
            id: active_column
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 5

            Repeater {
                model: character_tool.characters

                CharacterButton {
                    character_name: modelData
                    active: character_tool.active
                    onClicked: character_tool.setCurrentCharacter(index)
                    onToggle_active_clicked: character_tool.toggleCharacterActive(
                                                 index)
                }
            }
        }
    }

    SwipeView {
        anchors.left: character_scrollview.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5
        clip: true

        currentIndex: character_tool.pageIndex
        interactive: false

        Loader {
            id: classic_page
            source: "characters/ClassicPage.qml"
            asynchronous: true
            active: true
        }

        //        Loader {
        //            id: dsa5_page
        //            source: "characters/DSA5Page.qml"
        //            asynchronous: true
        //            active: true
        //        }
    }
}

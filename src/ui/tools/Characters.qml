import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "./characters"
import "../defines.js" as Defines

Page {
    id: root

    Component.onCompleted: character_tool.loadData()

    header: Rectangle {
        id: top_rect
        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase

        Row {
            id: top_left_row
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            padding: 10
            spacing: 10

            CustomCheckBox {
                text: qsTr("Active Characters")
                onClicked: character_tool.displayActiveCharacters(checked)
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        // Image buttons
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Repeater {
                model: character_tool.categories

                CustomToolBarButton {
                    buttonText: modelData
                    onClicked: character_tool.setCurrentCategory(index)
                    usesFixedWidth: false
                    pointSize: 12

                    Rectangle {
                        visible: index === character_tool.categoryIndex

                        color: palette.text
                        height: 1
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 4
                    }
                }
            }
        }
    }

    // Left bar
    CharacterList {
        id: character_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 175
    }

    SwipeView {
        anchors.left: character_list.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5
        clip: true

        currentIndex: character_tool.pageIndex
        interactive: false

        Loader {
            id: classic_page
            source: "characters/classic/ClassicPage.qml"
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

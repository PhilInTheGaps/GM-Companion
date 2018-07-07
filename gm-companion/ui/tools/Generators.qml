import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.colorscheme 1.0
import "./generators"

Page {
    id: generators_page

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Column {
        anchors.fill: parent

        TabBar {
            id: tab_bar
            height: color_scheme.toolbarHeight
            anchors.left: parent.left
            anchors.right: parent.right
            background: Rectangle {
                color: color_scheme.toolbarColor
            }

            TabButton {
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Row {
                    id: name_generator_tab_row
                    padding: 10
                    anchors.centerIn: parent

                    Label {
                        text: qsTr("Name Generator")
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }
                }

                background: Rectangle {
                    color: parent.pressed ? "black" : color_scheme.toolbarColor
                }
            }
        }

        SwipeView {
            id: swipe_view
            width: parent.width
            height: parent.height - tab_bar.height
            currentIndex: tab_bar.currentIndex

            NameGenerator {
                id: name_generator
            }
        }
    }
}

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
            width: parent.width

            TabButton {
                text: qsTr("Name Generator")
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

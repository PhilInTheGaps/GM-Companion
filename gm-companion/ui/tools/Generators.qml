import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import "./generators"

Page {
    id: generators_page

    TabView {
        id: generators_tab_view
        anchors.fill: parent

        Tab {
            title: qsTr("Name Generator")

            NameGenerator {
                id: name_generator
                anchors.fill: parent
            }
        }

        Tab {
            title: "SIFRP - House"
        }
    }
}

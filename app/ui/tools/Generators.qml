import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import "../defines.js" as Defines
import "./generators"

Page {
    id: generators_page

    header: Rectangle {
        id: tab_bar
        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase

        Row {
            anchors.fill: parent
            padding: 10
            spacing: 10

            CustomToolBarButton {
                buttonText: qsTr("Name Generator")
                usesFixedWidth: false
            }
        }
    }

    contentItem: NameGenerator {
        id: name_generator_page
    }
}

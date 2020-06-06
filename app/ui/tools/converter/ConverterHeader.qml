import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines

Rectangle {
    id: header_rect
    color: palette.alternateBase
    height: Defines.TOOLBAR_HEIGHT

    Row {
        anchors.fill: parent

        padding: 10
        spacing: 10

        CustomToolBarButton {
            iconText: FontAwesome.plus
            buttonText: qsTr("Add")
            onClicked: add_unit_dialog.open()
            usesFixedWidth: false
        }
    }
}

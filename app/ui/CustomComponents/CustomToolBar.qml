import QtQuick 2.6
import QtQuick.Controls 2.2
import FontAwesome 2.0
import "../defines.js" as Defines

Rectangle {
    id: tool_bar

    property alias button_row: row

    property bool enableBack: false
    signal backClicked
    property bool enableAdd: false
    signal addClicked
    property bool enableSave: false
    property bool isSaved: true
    signal saveClicked
    property bool enableExport: false
    signal exportClicked

    height: Defines.TOOLBAR_HEIGHT
    color: palette.alternateBase

    Row {
        id: row
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        // Back
        CustomToolBarButton {
            visible: enableBack
            iconText: FontAwesome.arrowAltCircleLeft
            onClicked: backClicked()
            pointSize: 18
        }

        // Add
        CustomToolBarButton {
            visible: enableAdd
            iconText: FontAwesome.fileMedical
            onClicked: addClicked()
            pointSize: 18
        }

        // Save
        CustomToolBarButton {
            visible: enableSave
            iconText: FontAwesome.fileDownload
            onClicked: saveClicked()
            pointSize: 18

            Text {
                visible: !isSaved
                text: FontAwesome.asterisk
                color: "darkred"
                font.family: FontAwesome.familySolid
                anchors.right: parent.right
                y: parent.height - height * 1.5
            }
        }

        // Export
        CustomToolBarButton {
            visible: enableExport
            iconText: FontAwesome.fileExport
            onClicked: exportClicked()
            pointSize: 18
        }
    }
}

import QtQuick
import IconFonts
import common

Rectangle {
    id: root

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

    height: CCSizes.toolbarHeight
    color: palette.alternateBase

    Rectangle {
        color: palette.dark
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 1
    }

    Row {
        id: row
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        // Back
        CustomToolBarButton {
            visible: root.enableBack
            iconText: FontAwesome.circleArrowLeft
            toolTipText: qsTr("Back")
            onClicked: root.backClicked()
            pointSize: 18
        }

        // Add
        CustomToolBarButton {
            visible: root.enableAdd
            iconText: FontAwesome.fileMedical
            toolTipText: qsTr("Add")
            onClicked: root.addClicked()
            pointSize: 18
        }

        // Save
        CustomToolBarButton {
            visible: root.enableSave
            iconText: FontAwesome.fileArrowDown
            toolTipText: qsTr("Save")
            onClicked: root.saveClicked()
            pointSize: 18

            Text {
                visible: !root.isSaved
                text: FontAwesome.asterisk
                color: SettingsManager.colors.error
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                anchors.right: parent.right
                y: parent.height - height * 1.5
            }
        }

        // Export
        CustomToolBarButton {
            visible: root.enableExport
            iconText: FontAwesome.fileExport
            toolTipText: qsTr("Export")
            onClicked: root.exportClicked()
            pointSize: 18
        }
    }
}

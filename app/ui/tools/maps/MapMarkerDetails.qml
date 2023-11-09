import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."

Item {
    id: root

    required property MapMarkerEditor markerEditor

    signal back
    signal deleteButtonClicked

    Column {
        id: marker_menu_column
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        // Back
        Item {
            height: Sizes.toolbarHeight
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5

            CustomToolBarButton {
                id: back_button
                iconText: FontAwesome.chevronLeft
                onClicked: root.back()
                anchors.margins: 7
            }

            Label {
                text: qsTr("Marker List")
                font.bold: true
                font.pointSize: 16
                verticalAlignment: Text.AlignVCenter

                anchors.left: back_button.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                elide: Label.ElideRight

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.back()
                }
            }
        }

        // Name and Icon
        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 5
            spacing: 10
            padding: 5

            Label {
                id: marker_icon
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                width: height
                font.pixelSize: parent.height - 15
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter

                text: MapTool.currentMarker ? MapTool.currentMarker.icon : ""
                color: MapTool.currentMarker ? MapTool.currentMarker.color : ""
            }

            Label {
                id: marker_name
                font.pointSize: 14
                width: parent.width - parent.padding - x
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                text: MapTool.currentMarker ? MapTool.currentMarker.name : ""
            }
        }
    }

    // Description
    ScrollView {
        anchors.top: marker_menu_column.bottom
        anchors.bottom: edit_marker_button.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        contentWidth: -1
        clip: true

        TextEdit {
            id: marker_description
            anchors.left: parent.left
            anchors.right: parent.right
            text: MapTool.currentMarker ? MapTool.currentMarker.description : ""

            font.pointSize: 11
            selectByMouse: true
            wrapMode: Text.WordWrap
            readOnly: true

            color: palette.text
            selectionColor: palette.highlight
            selectedTextColor: palette.highlightedText
        }
    }

    // Edit
    CustomButton {
        id: edit_marker_button

        buttonText: qsTr("Edit")
        iconText: FontAwesome.pen
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: delete_marker_button.top
        anchors.margins: 10
        anchors.bottomMargin: 0
        padding: 10

        onClicked: {
            root.markerEditor.markerName = marker_name.text
            root.markerEditor.markerDescription = marker_description.text
            root.markerEditor.markerIcon = marker_icon.text
            root.markerEditor.markerColor = marker_icon.color
            root.markerEditor.open()
        }
    }

    // Delete
    CustomButton {
        id: delete_marker_button

        buttonText: qsTr("Delete")
        iconText: FontAwesome.trash
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        padding: 10

        onClicked: {
            root.deleteButtonClicked()
        }
    }
}

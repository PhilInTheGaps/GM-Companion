import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines

Item {
    id: root

    signal back
    signal deleteButtonClicked

    Column {
        id: marker_menu_column
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        // Back
        Row {
            height: Defines.TOOLBAR_HEIGHT
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5

            CustomToolBarButton {
                iconText: FontAwesome.chevronLeft
                onClicked: back()
                anchors.margins: 7
            }

            Label {
                text: qsTr("Marker List")
                font.bold: true
                font.pointSize: 16
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: back()
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
                font.family: FontAwesome.familySolid
                width: height
                font.pixelSize: parent.height - 15
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter

                text: map_tool.currentMarker ? map_tool.currentMarker.icon : ""
                color: map_tool.currentMarker ? map_tool.currentMarker.color : ""
            }

            Label {
                id: marker_name
                font.pointSize: 14
                width: parent.width - parent.padding - x
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                text: map_tool.currentMarker ? map_tool.currentMarker.name : ""
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
            text: map_tool.currentMarker ? map_tool.currentMarker.description : ""

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
            marker_editor.markerName = marker_name.text
            marker_editor.markerDescription = marker_description.text
            marker_editor.markerIcon = marker_icon.text
            marker_editor.markerColor = marker_icon.color
            marker_editor.open()
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
            deleteButtonClicked()
        }
    }
}

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."

Item {
    id: root

    signal markerSelected(MapMarker data)

    Column {
        id: marker_list_column
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Label {
            id: markers_title_text
            text: qsTr("Markers")
            padding: 10
            font.bold: true
            font.pointSize: 16
            anchors.left: parent.left
            anchors.right: parent.right

            // Add Marker
            CustomToolBarButton {
                iconText: FontAwesome.plus

                height: Sizes.toolbarHeight - 15
                anchors.top: undefined
                anchors.bottom: undefined
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                onClicked: {
                    MapTool.addMarker()
                }
            }
        }
    }

    // List of Markers
    ScrollView {
        id: marker_list_scroll
        anchors.top: marker_list_column.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        clip: true
        contentWidth: -1

        Column {
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: MapTool.markerModel

                CustomButton {
                    required property MapMarker modelData
                    required property int index

                    iconText: modelData.icon
                    buttonText: modelData.name

                    iconColor: modelData.color

                    anchors.left: parent ? parent.left : undefined
                    anchors.right: parent ? parent.right : undefined

                    onClicked: {
                        MapTool.markerIndex = index
                        root.markerSelected(modelData)
                    }
                }
            }
        }
    }
}

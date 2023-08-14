import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts
import "../../defines.js" as Defines

Item {
    id: root

    signal markerSelected(var data)

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

                height: Defines.TOOLBAR_HEIGHT - 15
                anchors.top: undefined
                anchors.bottom: undefined
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                onClicked: {
                    map_tool.addMarker()
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
                model: mapMarkerModel

                CustomButton {
                    iconText: modelData.icon
                    buttonText: modelData.name

                    iconColor: modelData.color

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: {
                        map_tool.markerIndex = index
                        markerSelected(modelData)
                    }
                }
            }
        }
    }
}

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

    Label {
        id: markers_title_text
        text: qsTr("Markers")
        font.bold: true
        font.pointSize: 16

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        // Add Marker
        CustomToolBarButton {
            iconText: FontAwesome.plus

            height: Sizes.toolbarHeight - 15
            anchors.top: undefined
            anchors.bottom: undefined
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            onClicked: {
                MapTool.addMarker();
            }
        }
    }

    // List of Markers
    Flickable {
        id: flickable
        anchors.top: markers_title_text.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        anchors.rightMargin: 0

        clip: true
        contentHeight: marker_list_column.height
        contentWidth: -1

        ScrollBar.vertical: ScrollBar {
            anchors.right: parent.right
            visible: flickable.contentHeight > flickable.height
        }

        Column {
            id: marker_list_column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 10

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
                        MapTool.markerIndex = index;
                        root.markerSelected(modelData);
                    }
                }
            }
        }
    }
}

import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

Item {
    id: root
    property string name: qsTr("New Location")
    property string description
    property string color: "red"
    property string icon: FontAwesome.mapMarkerAlt
    property int markerIndex: -1
    property bool showDelete: false

    scale: 1 / image.scale

    Rectangle {
        width: marker_text.width
        height: marker_text.height

        x: -width / 2
        y: -height

        color: "transparent"

        Text {
            id: marker_text
            font.family: FontAwesome.familySolid
            text: icon
            anchors.centerIn: parent
            style: Text.Outline

            color: root.color
            styleColor: "black"

            font.pointSize: 30
        }

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            hoverEnabled: true

            drag.target: root
            drag.smoothed: false
            preventStealing: true

            onContainsMouseChanged: {
                if (containsMouse) {
                    marker_name_label.text = root.name
                }

                marker_name_label.visible = containsMouse
            }

            onClicked: {
                left_swipe_view.currentIndex = 2
                marker_name.text = root.name
                marker_description.text = root.description
                marker_icon.text = root.icon
                marker_icon.color = root.color

                map_tool.markerIndex = root.markerIndex
            }

            drag.onActiveChanged: {
                if (!drag.active)
                    map_tool.setMarkerPosition(markerIndex, root.x, root.y)
            }
        }

        // For right-clicks
        MouseArea {
            id: mouse_area2
            anchors.fill: parent
            propagateComposedEvents: true
            acceptedButtons: Qt.RightButton

            onClicked: {
                marker_delete_rect.markerIndex = root.markerIndex
                marker_delete_rect.visible = true
            }
        }
    }
}
